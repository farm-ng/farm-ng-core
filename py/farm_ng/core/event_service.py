"""
# Will run an test_service from the config.
python -m farm_ng.core.event_service --service-config config.json --service-name test_service
"""
from __future__ import annotations
import argparse
import asyncio
from collections import defaultdict
from dataclasses import dataclass
import logging
import time
from typing import AsyncIterator, Callable
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
    RequestReplyRequest,
    RequestReplyReply,
)
from farm_ng.core.stamp import StampSemantics, get_monotonic_now, get_system_clock_now
from farm_ng.core.timestamp_pb2 import Timestamp
from farm_ng.core.uri import make_proto_uri
from farm_ng.core.uri_pb2 import Uri
from google.protobuf.message import Message
from google.protobuf.wrappers_pb2 import Int32Value
from google.protobuf.empty_pb2 import Empty

# public members

__all__ = [
    "EventServiceGrpc",
    "PublishResult",
]


@dataclass(frozen=True)
class PublishResult:
    """The result of publishing a message to the service."""

    # the number of clients the message was sent to
    number_clients: int
    # the sequence number of the message
    sequence_number: int


# TODO: iterate this data structure to report dropped messages and other stats
@dataclass(frozen=False)
class _UriStats:
    """The stats of a URI."""

    # the number of messages dropped
    dropped: int = 0
    # the not sent messages
    not_sent: int = 0


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

        self._logger.setLevel(config.log_level)
        self._logger.addHandler(logging.StreamHandler())

        # the time when the service was started
        self.time_started: float = time.monotonic()

        # the queues for the clients connected to the service
        self._client_queues: dict[
            str, list[tuple[SubscribeRequest, asyncio.Queue]]
        ] = {}

        # TODO: merge into a single dict containing a data structure with the uri, the counts
        # the URIs of the service
        self._uris: dict[str, Uri] = {}

        # the counts of the URIs
        self._counts: dict[str, int] = {}

        # the stats of the URIs, to keep track of dropped messages and other stats
        self._uris_stats: dict[str, _UriStats] = defaultdict(_UriStats)

        # the request/reply handler
        self._request_reply_handler: Callable | None = None

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
    def request_reply_handler(self) -> Callable | None:
        """Returns the request/reply handler."""
        return self._request_reply_handler

    @request_reply_handler.setter
    def request_reply_handler(self, handler: Callable) -> None:
        """Sets the request/reply handler."""
        self._request_reply_handler = handler

    @property
    def counts(self) -> dict[str, int]:
        """Returns the counts of the URIs."""
        return self._counts

    @property
    def uris(self) -> dict[str, Uri]:
        """Returns the URIs of the service."""
        return self._uris

    # public methods

    async def serve(self) -> None:
        """Starts the service.

        This method is called by the service manager.
        """
        self.server.add_insecure_port(f"[::]:{self.config.port}")

        self.logger.info("Starting server on port %i", self.config.port)
        await self.server.start()

        self.logger.info("Server started")
        await self.server.wait_for_termination()

    async def publish(
        self, path: str, message: Message, timestamps: list[Timestamp] | None = None
    ) -> PublishResult:
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
        result = await self._send_raw(uri=uri, message=message, timestamps=timestamps)

        return PublishResult(**result)

    # public gRPC methods

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
        """Implementation of grpc rpc subscribe"""

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

    async def requestReply(
        self,
        request: RequestReplyRequest,
        context: grpc.aio.ServicerContext,  # pylint: disable=unused-argument
    ) -> RequestReplyReply:
        # adds the timestamps to the event as it passes through the service
        recv_stamp = get_monotonic_now(StampSemantics.SERVICE_RECEIVE)
        request.event.timestamps.append(recv_stamp)
        event = Event()
        event.CopyFrom(request.event)
        event.uri.path = "/request" + event.uri.path
        # self._publish_event_payload(event, request.payload)

        reply_message: Message
        if self._request_reply_handler is not None:
            reply_message = await self._request_reply_handler(self, request)
            if reply_message is None:
                self.logger.error(
                    "Request invalid, please check your request channel and packet ",
                    request,
                )
        else:
            reply_message = Empty()

        reply_uri: Uri = make_proto_uri(
            path="/reply" + event.uri.path, message=reply_message
        )
        reply_uri.query += f"&service_name={self.config.name}"

        # create the event and send
        reply_payload: bytes = reply_message.SerializeToString()

        timestamps = []
        timestamps.append(get_monotonic_now(semantics=StampSemantics.SERVICE_SEND))
        timestamps.append(get_system_clock_now(semantics=StampSemantics.SERVICE_SEND))

        event = Event(
            uri=reply_uri,
            timestamps=timestamps,
            payload_length=len(reply_payload),
            sequence=event.sequence,
        )
        reply = RequestReplyReply(event=event, payload=reply_payload)
        # self._publish_event_payload(reply.event, reply.payload)
        return reply

    # private methods

    async def _publish_event_payload(self, event: Event, payload: bytes) -> int:
        """Send an event and payload to the clients.

        Args:
            event (Event): The event.
            payload (bytes): The payload.

        Returns:
            int: The number of clients the message was sent to.
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
                # keep tracked of dropped messages in a data structure
                # to be able to report them to the user later
                stats = self._uris_stats[uri.path]
                stats.dropped += 1
                self.logger.warning(
                    "dropped %d sequence %d path: %s",
                    stats.dropped,
                    reply.event.sequence,
                    uri.path,
                )

        # wait for the queues to be emptied
        await asyncio.sleep(0)

        # return the number of clients the message was sent to
        return len(queues)

    async def _send_raw(
        self, uri: Uri, message: Message, timestamps: list[Timestamp]
    ) -> dict[str, int]:
        """Send a message to the service.

        Args:
            uri (Uri): The URI of the message.
            message (Message): The message.
            timestamps (list[Timestamp]): The timestamps.

        Returns:
            dict[str, int]: The sequence number of the message and the number of clients it was sent to.
        """
        # check that the message type is the same as the previous messages
        if uri.path in self._uris:
            # get the URI of the previous message
            previous_uri: Uri = self._uris[uri.path]

            # check that the message type is the same
            if previous_uri.query != uri.query:
                type1: str = previous_uri.query.split("&")[0].split(".")[-1]
                type2: str = uri.query.split("&")[0].split(".")[-1]
                raise TypeError(f"Message type mismatch: {type1} != {type2}")

        # register the URI of the message
        self._uris[uri.path] = uri

        # get the count of the URI, or 0 if it doesn't exist
        count: int = self._counts.get(uri.path, 0)

        # increment the count of the URI
        # NOTE: even if the message is not sent, the count is incremented
        # so that the client can know that the message was not sent by checking
        # the count of the URI.
        self._counts[uri.path] = count + 1

        # create the event and send
        payload: bytes = message.SerializeToString()

        event = Event(
            uri=uri,
            timestamps=timestamps,
            payload_length=len(payload),
            sequence=count,
        )

        return {
            "sequence_number": count,
            "number_clients": await self._publish_event_payload(event, payload),
        }


def add_service_parser(parser):
    parser.add_argument("--service-config", type=str, required=True)
    parser.add_argument("--service-name", type=str, required=True)


def load_service_config(_args):
    _config_list = proto_from_json_file(_args.service_config, EventServiceConfigList())
    _service_config = None
    for config in _config_list.configs:
        if config.name == _args.service_name:
            _service_config = config
    assert (
        _service_config is not None
    ), f"service {_args.service_name} not found in config list {_config_list}"
    return _config_list, _service_config


def event_service_main_args():
    parser = argparse.ArgumentParser()
    add_service_parser(parser)
    args = parser.parse_args()
    return load_service_config(args)


async def test_send_smoke(event_service: EventServiceGrpc) -> None:
    count = 0
    while True:
        await asyncio.sleep(1)
        await event_service.publish("/test", Int32Value(value=count))
        count += 1


async def test_send_smoke2(event_service: EventServiceGrpc) -> None:
    count = 0
    while True:
        await asyncio.sleep(0.5)
        await event_service.publish("/test2", Int32Value(value=count))
        count += 1


async def test_req_rep_handler_smoke(
    event_service: EventServiceGrpc, request: RequestReplyRequest
) -> Message:
    message = payload_to_protobuf(request.event, request.payload)
    event_service.logger.info(
        f"Received: {request.event.uri.path} {request.event.sequence} {message}".rstrip()
    )
    return message  # echo message back


# main function to run the service and all the async tasks
async def test_main(args, event_service: EventServiceGrpc) -> None:
    # define the async tasks
    async_tasks = []

    event_service.request_reply_handler = test_req_rep_handler_smoke

    # add the service task
    async_tasks.append(event_service.serve())
    async_tasks.append(test_send_smoke(event_service))
    async_tasks.append(test_send_smoke2(event_service))

    # run the tasks
    await asyncio.gather(*async_tasks)


if __name__ == "__main__":
    config_list: EventServiceConfigList
    service_config: EventServiceConfig
    config_list, service_config = event_service_main_args()
    parser = argparse.ArgumentParser()
    parser.add_argument("--my-arg", type=str, required=True)
    parser.add_argument("foo", type=str, nargs="?")
    args = parser.parse_args(service_config.args)

    # create the gRPC server and initialize the service
    event_service: EventServiceGrpc = EventServiceGrpc(
        grpc.aio.server(), service_config
    )
    loop = asyncio.get_event_loop()
    loop.run_until_complete(test_main(args, event_service))
