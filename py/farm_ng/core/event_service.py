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
from typing import Any, AsyncIterator

import grpc
from farm_ng.core import event_service_pb2_grpc

# pylint can't find Event or Uri in protobuf generated files
# https://github.com/protocolbuffers/protobuf/issues/10372
from farm_ng.core.event_pb2 import Event
from farm_ng.core.event_service_pb2 import (
    EventServiceConfig,
    GetServiceStateReply,
    GetServiceStateRequest,
    ListUrisReply,
    ListUrisRequest,
    ServiceState,
    SubscribeReply,
    SubscribeRequest,
)
from farm_ng.core.stamp import StampSemantics, get_monotonic_now, get_system_clock_now
from farm_ng.core.timestamp_pb2 import Timestamp
from farm_ng.core.uri import make_proto_uri
from farm_ng.core.uri_pb2 import Uri
from google.protobuf.message import Message
from google.protobuf.wrappers_pb2 import Int32Value
from farm_ng.core.events_file_reader import proto_from_json_file
from farm_ng.core.event_service_pb2 import EventServiceConfigList


class EventServiceGrpc:
    """Base class for a gRPC service."""

    def __init__(
        self,
        server: grpc.aio.Server,
        config: EventServiceConfig,
        logger: logging.Logger = None,
    ) -> None:
        """Initializes a new service.

        Args:
            server: The gRPC server instance.
            config: The service configuration.
        """
        self._server = server
        self._config = config

        # create a logger for the service
        self._logger: logging.Logger
        if logger is not None:
            self._logger = logger
        else:
            self._logger = logging.getLogger(config.name)

        # the initial state when doing nothing must be IDLE
        self.state: ServiceState = ServiceState.IDLE

        # the time when the service was started
        self.time_started: float = time.monotonic()

        self._client_queues: dict(str, list[asyncio.Queue]) = dict()

        self._uris = dict()
        self._counts = dict()
        # add the service to the server
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

    async def subscribe(
        self,
        request: SubscribeRequest,
        context: grpc.aio.ServicerContext,
    ) -> AsyncIterator[SubscribeReply]:

        if self.state != ServiceState.RUNNING:
            self.logger.info(f"Setting {self._config.name} state to running")
            self.state = ServiceState.RUNNING

        # create a queue for this client
        req_queue = (request, asyncio.Queue(maxsize=10))
        if request.uri.path not in self._client_queues.keys():
            self._client_queues[request.uri.path] = []

        self._client_queues[request.uri.path].append(req_queue)

        def delete_queue(_) -> None:
            # remove the queue from the list of queues
            self._client_queues[request.uri.path].remove(req_queue)

            # check that there are no more clients connected
            client_count = 0
            for _, queues in self._client_queues.items():
                client_count += len(queues)

            if client_count == 0:
                self.logger.info(f"Setting {self._config.name} to idle state")
                self.state = ServiceState.IDLE

        context.add_done_callback(delete_queue)

        while True:
            yield await req_queue[1].get()

    def _send_raw(
        self, uri: Uri, message: Message, timestamps: list[Timestamp]
    ) -> None:
        self._uris[uri.path] = uri
        count = self._counts.get(uri.path, 0)

        payload = message.SerializeToString()
        reply = SubscribeReply(
            event=Event(
                uri=uri,
                timestamps=timestamps,
                payload_length=len(payload),
                sequence=count,
            ),
            payload=payload,
        )
        client_queues = self._client_queues.get(uri.path, [])
        client_queues = self._client_queues.get("*", []) + client_queues

        queues = [
            queue
            for request, queue in client_queues
            if request.every_n == 0 or count % request.every_n == 0
        ]
        self.logger.debug(
            f"Sending {uri.path}: {reply.event.sequence} to {len(queues)} clients"
        )
        for queue in queues:
            try:
                queue.put_nowait(reply)
            except asyncio.QueueFull:
                pass

        self._counts[uri.path] = count + 1

    def publish(
        self, path: str, message: Message, timestamps: list[Timestamp] | None = None
    ) -> None:

        if timestamps is None:
            timestamps = []
        timestamps.append(get_monotonic_now(semantics=StampSemantics.DRIVER_SEND))
        timestamps.append(get_system_clock_now(semantics=StampSemantics.DRIVER_SEND))
        uri = make_proto_uri(path=path, message=message)
        uri.query += f"&service_name={self.config.name}"
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


# main function to run the service and all the async tasks
async def test_main(event_service: EventServiceGrpc) -> None:
    # define the async tasks
    async_tasks: list[asyncio.Task] = []

    async_tasks.append(event_service.serve())
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
