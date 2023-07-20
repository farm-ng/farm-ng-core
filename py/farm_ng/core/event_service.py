"""
# Will run an test_service from the config.
python -m farm_ng.core.event_service --service-config config.json --service-name test_service
"""
from __future__ import annotations
import argparse
import asyncio
import logging
import os
import time
import importlib
from typing import AsyncIterator, Type

import grpc
from farm_ng.core import event_service_pb2_grpc
from farm_ng.core.events_file_reader import _parse_protobuf_descriptor

# pylint can't find Event or Uri in protobuf generated files
# https://github.com/protocolbuffers/protobuf/issues/10372
from farm_ng.core.event_pb2 import Event
from farm_ng.core.event_service_pb2 import (
    EventServiceConfig,
    EventServiceConfigList,
    GetServiceStateReply,
    GetServiceStateRequest,
    ListUrisReply,
    ListUrisRequest,
    ServiceState,
    SubscribeReply,
    SubscribeRequest,
    SendRequest,
    SendReply,
)
from farm_ng.core.stamp import StampSemantics, get_monotonic_now, get_system_clock_now
from farm_ng.core.timestamp_pb2 import Timestamp
from farm_ng.core.uri import make_proto_uri
from farm_ng.core.uri_pb2 import Uri
from google.protobuf.message import Message
from google.protobuf.wrappers_pb2 import Int32Value
from farm_ng.core.events_file_reader import proto_from_json_file


class EventServiceGrpc:
    """Base class for a gRPC service."""

    # the maximum size of the queue for received messages
    QUEUE_MAX_SIZE: int = 10

    def __init__(
        self,
        server: grpc.aio.Server,
        config: EventServiceConfig,
        logger: logging.Logger | None = None,
    ) -> None:
        """Initializes a new service.

        Args:
            server: The gRPC server instance.
            config: The service configuration.
            logger: The logger instance.
        """
        self._server = server
        self._config = config

        # create a logger for the service
        self._logger: logging.Logger = logger or logging.getLogger(config.name)

        # the initial state when doing nothing must be IDLE
        self.state: ServiceState = ServiceState.IDLE

        # the time when the service was started
        self.time_started: float = time.monotonic()

        # the queue for received messages
        self._recv_queue = asyncio.Queue(maxsize=self.QUEUE_MAX_SIZE)

        # the queues for the clients connected to the service
        self._client_queues: dict[
            str, list[tuple[SubscribeRequest, asyncio.Queue]]
        ] = dict()

        # the URIs of the service
        self._uris = dict()

        # the counts of the URIs
        self._counts = dict()

        # add the service to the asyncio server
        event_service_pb2_grpc.add_EventServiceServicer_to_server(self, server)

    @property
    def server(self) -> grpc.aio.Server:
        """Returns the server instance."""
        return self._server

    @property
    def config(self) -> EventServiceConfig:
        """Returns the service configuration."""
        return self._config

    @property
    def logger(self) -> logging.Logger:
        """Returns the service logger."""
        return self._logger

    async def serve(self) -> None:
        """Starts the service.

        This method is called by the service manager.
        """
        self.server.add_insecure_port(f"[::]:{self.config.port}")

        self.logger.info("Starting server on port %i", self.config.port)
        await self.server.start()

        self.logger.info("Server started")
        await self.server.wait_for_termination()

    def getServiceState(
        self,
        request: GetServiceStateRequest,
        context: grpc.aio.ServicerContext,
    ) -> GetServiceStateReply:
        """Get the service state.

        Args:
            request (service_pb2.GetServiceStateRequest): The request.
            context (grpc.aio.ServicerContext): The context.

        Returns:
            service_pb2.GetServiceStateReply: The reply.
        """
        return GetServiceStateReply(
            state=self.state,
            pid=os.getpid(),
            uptime=time.monotonic() - self.time_started,
            timestamps=[],  # TODO
        )

    async def listUris(
        self,
        request: ListUrisRequest,
        context: grpc.aio.ServicerContext,
    ) -> ListUrisReply:
        """List the URIs of the service.

        Args:
            request (service_pb2.ListUrisRequest): The request.
            context (grpc.aio.ServicerContext): The context.

        Returns:
            service_pb2.ListUrisReply: The reply.
        """
        return ListUrisReply(uris=[uri for _, uri in self._uris.items()])

    # NOTE: private method?
    def client_count(self) -> int:
        """Returns the number of clients connected to the service."""
        client_count: int = 0

        # iterate over all the queues and count the clients
        queues: list[asyncio.Queue]
        for _, queues in self._client_queues.items():
            client_count += len(queues)

        return client_count

    # NOTE: private method?
    def update_state_idle(self) -> None:
        """Updates the state to idle if there are no more clients connected."""
        if self.client_count() > 0:
            return

        # setting the state of the service to idle
        self.logger.info(f"Setting {self._config.name} to idle state")
        self.state = ServiceState.IDLE

    async def subscribe(
        self,
        request: SubscribeRequest,
        context: grpc.aio.ServicerContext,
    ) -> AsyncIterator[SubscribeReply]:
        """Subscribe to the service.

        Args:
            request (service_pb2.SubscribeRequest): The request.
            context (grpc.aio.ServicerContext): The context.

        Yields:
            AsyncIterator[service_pb2.SubscribeReply]: The reply.
        """
        # setting the state of the service to running
        if self.state != ServiceState.RUNNING:
            self.logger.info(f"Setting {self._config.name} state to running")
            self.state = ServiceState.RUNNING

        # create a queue for this client
        # TODO: make a small `RequestQueue` class with `request` and `queue for better readability
        request_queue: tuple[SubscribeRequest, asyncio.Queue] = (
            request,
            asyncio.Queue(maxsize=self.QUEUE_MAX_SIZE),
        )

        # add the queue to the list of queues
        # TODO: use defaultdict from collections
        if request.uri.path not in self._client_queues.keys():
            self._client_queues[request.uri.path] = []

        self._client_queues[request.uri.path].append(request_queue)

        def delete_queue(_) -> None:
            """Callback to delete the queue when the client disconnects."""
            # remove the queue from the list of queues
            self._client_queues[request.uri.path].remove(request_queue)

            # check that there are no more clients connected
            self.update_state_idle()

        # register the callback to delete the queue when the client disconnects
        context.add_done_callback(delete_queue)

        while True:
            yield await request_queue[1].get()

    def _send_event_payload(self, event: Event, payload: bytes) -> None:
        """Send an event and payload to the clients.

        Args:
            event (Event): The event.
            payload (bytes): The payload.
        """
        uri: Uri = event.uri

        # register the URI of the message
        self._uris[uri.path] = uri

        reply = SubscribeReply(
            event=event,
            payload=payload,
        )

        # get client queues for the URI
        client_queues: list[tuple[SubscribeRequest, asyncio.Queue]]
        client_queues = self._client_queues.get(uri.path, [])
        client_queues = self._client_queues.get("*", []) + client_queues

        queues: list[asyncio.Queue] = [
            queue
            for request, queue in client_queues
            if request.every_n == 0 or reply.event.sequence % request.every_n == 0
        ]
        self.logger.debug(
            f"Sending {uri.path}: {reply.event.sequence} to {len(queues)} clients"
        )

        # send the reply to the clients
        for queue in queues:
            try:
                queue.put_nowait(reply)
            except asyncio.QueueFull:
                # NOTE: don't we want to drop the oldest message?
                pass

    def _send_raw(
        self, uri: Uri, message: Message, timestamps: list[Timestamp]
    ) -> None:
        """Send a message to the service.

        Args:
            uri (Uri): The URI of the message.
            message (Message): The message.
            timestamps (list[Timestamp]): The timestamps.
        """
        # register the URI of the message
        self._uris[uri.path] = uri

        # get the count of the URI, or 0 if it doesn't exist
        count = self._counts.get(uri.path, 0)

        # increment the count of the URI
        self._counts[uri.path] = count + 1

        # create the event and send
        payload: bytes = message.SerializeToString()

        event = Event(
            uri=uri,
            timestamps=timestamps,
            payload_length=len(payload),
            sequence=count,
        )
        self._send_event_payload(event, payload)

    # TODO: i'd rename this to `publish` to match with ROS
    async def send(
        self,
        request: SendRequest,
        context: grpc.aio.ServicerContext,
    ) -> SendReply:
        """Send a message to the service.

        Args:
            request (service_pb2.SendRequest): The request.
            context (grpc.aio.ServicerContext): The context.

        Returns:
            service_pb2.SendReply: The reply.
        """
        # adds the timestamps to the event as it passes through the service
        request.event.timestamps.append(
            get_monotonic_now(StampSemantics.CLIENT_RECEIVE)
        )
        # self._send_event_payload(request.event, request.payload)
        try:
            # NOTE: why not ?
            # await self._recv_queue.put(request)
            self._recv_queue.put_nowait(request)
        except asyncio.QueueFull:
            # NOTE: don't we want to drop the oldest message?
            pass

        return SendReply()

    async def recv(
        self, decode: bool = True
    ) -> AsyncIterator[tuple[Event, Message | bytes]]:
        """Receive a message from the service.

        Args:
            decode (bool, optional): Whether to decode the message. Defaults to True.

        Yields:
            AsyncIterator[tuple[Event, Message | bytes]]: The message and the event.
        """
        while True:
            # wait for a message to be received
            request: SendRequest = await self._recv_queue.get()

            # decode the message if requested
            if decode:
                name: str
                package: str
                name, package = _parse_protobuf_descriptor(request.event.uri)
                message_cls: Type[Message] = getattr(
                    importlib.import_module(package), name
                )

                message: Message = message_cls()
                message.ParseFromString(request.payload)
                yield request.event, message

            # yield event and payload message
            yield request.event, request.payload

    # TODO: i'd rename this to send
    def publish(
        self, path: str, message: Message, timestamps: list[Timestamp] | None = None
    ) -> None:
        """Publish a message to the service.

        Args:
            path (str): The path of the message.
            message (Message): The message.
            timestamps (list[Timestamp], optional): The timestamps. Defaults to None.
        """
        # add the timestamps to the event as it passes through the service
        timestamps = timestamps or []
        timestamps.append(get_monotonic_now(semantics=StampSemantics.DRIVER_SEND))
        timestamps.append(get_system_clock_now(semantics=StampSemantics.DRIVER_SEND))

        # create the URI of the message
        uri: Uri = make_proto_uri(path=path, message=message)
        uri.query += f"&service_name={self.config.name}"

        # send the message to the service
        self._send_raw(uri=uri, message=message, timestamps=timestamps)


async def test_send_smoke(event_service: EventServiceGrpc) -> None:
    count = 0
    while True:
        await asyncio.sleep(1)
        event_service.publish("/test", Int32Value(value=count))
        count += 1


async def test_send_smoke2(event_service: EventServiceGrpc) -> None:
    count = 0
    while True:
        await asyncio.sleep(0.5)
        event_service.publish("/test2", Int32Value(value=count))
        count += 1


async def test_recv_smoke(event_service: EventServiceGrpc) -> None:
    async for event, message in event_service.recv():
        event_service.logger.info(
            f"Received: {event.uri.path} {event.sequence} {message}".rstrip()
        )


# main function to run the service and all the async tasks
async def test_main(event_service: EventServiceGrpc) -> None:
    # define the async tasks
    async_tasks: list[asyncio.Task] = []

    async_tasks.append(event_service.serve())
    async_tasks.append(test_recv_smoke(event_service))
    async_tasks.append(test_send_smoke(event_service))
    async_tasks.append(test_send_smoke2(event_service))

    # run the tasks
    await asyncio.gather(*async_tasks)


def add_service_parser(parser):
    parser.add_argument("--service-config", type=str, required=True)
    parser.add_argument("--service-name", type=str, required=True)


def load_service_config(args):
    config_list = proto_from_json_file(args.service_config, EventServiceConfigList())
    service_config = None
    for config in config_list.configs:
        if config.name == args.service_name:
            service_config = config
    assert (
        service_config is not None
    ), f"service {args.service_name} not found in config list {config_list}"
    return config_list, service_config


if __name__ == "__main__":
    argparse = argparse.ArgumentParser()
    add_service_parser(argparse)
    args = argparse.parse_args()
    config_list, service_config = load_service_config(args)

    # create the gRPC server and initialize the service
    server = grpc.aio.server()
    event_service: EventServiceGrpc = EventServiceGrpc(server, service_config)
    event_service.logger.setLevel(logging.DEBUG)
    event_service.logger.addHandler(logging.StreamHandler())
    loop = asyncio.get_event_loop()
    loop.run_until_complete(test_main(event_service=event_service))
