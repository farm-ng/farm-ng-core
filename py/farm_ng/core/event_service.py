"""
# Will run an test_service from the config.
python -m farm_ng.core.event_service --service-config config.json --service-name test_service
"""
from __future__ import annotations
import argparse
import asyncio
import logging
import time
from typing import AsyncIterator

import grpc
from farm_ng.core import event_service_pb2_grpc
from farm_ng.core.events_file_reader import proto_from_json_file, payload_to_protobuf

# pylint can't find Event or Uri in protobuf generated files
# https://github.com/protocolbuffers/protobuf/issues/10372
from farm_ng.core.event_pb2 import Event
from farm_ng.core.event_service_pb2 import (
    EventServiceConfig,
    EventServiceConfigList,
    ListUrisReply,
    ListUrisRequest,
    SubscribeReply,
    SubscribeRequest,
    ReqRepRequest,
    ReqRepReply,
)
from farm_ng.core.stamp import StampSemantics, get_monotonic_now, get_system_clock_now
from farm_ng.core.timestamp_pb2 import Timestamp
from farm_ng.core.uri import make_proto_uri
from farm_ng.core.uri_pb2 import Uri
from google.protobuf.message import Message
from google.protobuf.wrappers_pb2 import Int32Value


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

        # the time when the service was started
        self.time_started: float = time.monotonic()

        # the queues for the clients connected to the service
        self._client_queues: dict[
            str, list[tuple[SubscribeRequest, asyncio.Queue]]
        ] = {}

        # the URIs of the service
        self._uris: dict[str, str] = {}

        # the counts of the URIs
        self._counts: dict[str, int] = {}

        # the request/reply handler
        self._request_reply_handler: callable | None = None

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

    @property
    def request_reply_handler(self) -> callable | None:
        """Returns the request/reply handler."""
        return self._request_reply_handler

    @request_reply_handler.setter
    def request_reply_handler(self, handler: callable) -> None:
        """Sets the request/reply handler."""
        self._request_reply_handler = handler

    async def serve(self) -> None:
        """Starts the service.

        This method is called by the service manager.
        """
        self.server.add_insecure_port(f"[::]:{self.config.port}")

        self.logger.info("Starting server on port %i", self.config.port)
        await self.server.start()

        self.logger.info("Server started")
        await self.server.wait_for_termination()

    # pylint: disable=invalid-name
    async def listUris(
        self,
        request: ListUrisRequest,  # pylint: disable=unused-argument
        context: grpc.aio.ServicerContext,  # pylint: disable=unused-argument
    ) -> ListUrisReply:
        """List the URIs of the service.

        Args:
            request (service_pb2.ListUrisRequest): The request.
            context (grpc.aio.ServicerContext): The context.

        Returns:
            service_pb2.ListUrisReply: The reply.
        """
        return ListUrisReply(uris=[uri for _, uri in self._uris.items()])

    async def subscribe(
        self,
        request: SubscribeRequest,
        context: grpc.aio.ServicerContext,
    ) -> AsyncIterator[SubscribeReply]:
        """Impleemntation of grpc rpc subscribe"""

        # create a queue for this client
        # TODO: make a small `RequestQueue` class with `request` and `queue for better readability
        request_queue: tuple[SubscribeRequest, asyncio.Queue] = (
            request,
            asyncio.Queue(maxsize=self.QUEUE_MAX_SIZE),
        )

        # add the queue to the list of queues
        # TODO: use defaultdict from collections
        if request.uri.path not in self._client_queues:
            self._client_queues[request.uri.path] = []

        self._client_queues[request.uri.path].append(request_queue)

        def delete_queue(_) -> None:
            # remove the queue from the list of queues
            self._client_queues[request.uri.path].remove(request_queue)

        # register the callback to delete the queue when the client disconnects
        context.add_done_callback(delete_queue)

        while True:
            yield await request_queue[1].get()

    def _publish_event_payload(self, event: Event, payload: bytes) -> None:
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
        count: int = self._counts.get(uri.path, 0)

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
        self._publish_event_payload(event, payload)

    async def reqRep(
        self,
        request: ReqRepRequest,
        context: grpc.aio.ServicerContext,  # pylint: disable=unused-argument
    ) -> ReqRepReply:
        # adds the timestamps to the event as it passes through the service
        request.event.timestamps.append(
            get_monotonic_now(StampSemantics.CLIENT_RECEIVE)
        )

        # call the request/reply handler
        if self._request_reply_handler is not None:
            return await self._request_reply_handler(self, request)

        return ReqRepReply()

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


async def test_req_rep_handler_smoke(
    event_service: EventServiceGrpc, request: ReqRepRequest
) -> ReqRepReply:
    message = payload_to_protobuf(request.event, request.payload)
    event_service.logger.info(
        f"Received: {request.event.uri.path} {request.event.sequence} {message}".rstrip()
    )
    return ReqRepReply(event=request.event, payload=request.payload)


# main function to run the service and all the async tasks
async def test_main(event_service: EventServiceGrpc) -> None:
    # define the async tasks
    async_tasks: list[asyncio.Task] = []

    event_service.setReqRepHandler(test_req_rep_handler_smoke)

    # add the service task
    async_tasks.append(event_service.serve())
    async_tasks.append(test_send_smoke(event_service))
    async_tasks.append(test_send_smoke2(event_service))

    # run the tasks
    await asyncio.gather(*async_tasks)


def add_service_parser(parser):
    parser.add_argument("--service-config", type=str, required=True)
    parser.add_argument("--service-name", type=str, required=True)


def load_service_config(_args):
    _config_list = proto_from_json_file(_args.service_config, EventServiceConfigList())
    _service_config = None
    for config in _config_list.configs:
        if config.name == args.service_name:
            _service_config = config
    assert (
        _service_config is not None
    ), f"service {args.service_name} not found in config list {_config_list}"
    return _config_list, _service_config


if __name__ == "__main__":
    argparse = argparse.ArgumentParser()
    add_service_parser(argparse)
    args = argparse.parse_args()
    config_list, service_config = load_service_config(args)

    # create the gRPC server and initialize the service
    _event_service: EventServiceGrpc = EventServiceGrpc(
        grpc.aio.server(), service_config
    )
    _event_service.logger.setLevel(logging.DEBUG)
    _event_service.logger.addHandler(logging.StreamHandler())
    loop = asyncio.get_event_loop()
    loop.run_until_complete(test_main(event_service=_event_service))
