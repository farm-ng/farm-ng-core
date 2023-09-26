"""
# Will run an test_service from the config.
python -m farm_ng.core.event_service --service-config config.json --service-name test_service
"""
from __future__ import annotations

import argparse
import asyncio
import inspect
import logging
import time
from dataclasses import dataclass
from typing import TYPE_CHECKING, AsyncIterator, Callable

import grpc
from farm_ng.core import event_service_pb2_grpc

# pylint can't find Event or Uri in protobuf generated files
# https://github.com/protocolbuffers/protobuf/issues/10372
from farm_ng.core.event_pb2 import Event
from farm_ng.core.event_service_pb2 import (
    EventServiceConfig,
    EventServiceConfigList,
    ListUrisReply,
    ListUrisRequest,
    RequestReplyReply,
    RequestReplyRequest,
    SubscribeReply,
    SubscribeRequest,
)
from farm_ng.core.events_file_reader import payload_to_protobuf, proto_from_json_file
from farm_ng.core.stamp import (
    StampSemantics,
    get_monotonic_now,
    get_stamp_by_semantics_and_clock_type,
    get_system_clock_now,
)
from farm_ng.core.uri import make_proto_uri
from google.protobuf.empty_pb2 import Empty
from google.protobuf.wrappers_pb2 import Int32Value

if TYPE_CHECKING:
    from farm_ng.core.timestamp_pb2 import Timestamp
    from farm_ng.core.uri_pb2 import Uri
    from google.protobuf.message import Message

from .event_service_metrics import EventServiceHealthMetrics

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
            str,
            list[tuple[SubscribeRequest, asyncio.Queue]],
        ] = {}

        # the URIs of the service
        self._uris: dict[str, Uri] = {}

        # the stats of the URIs, to keep track of dropped messages and other stats
        self._latched_events: dict[str, SubscribeReply] = {}

        self._metrics = EventServiceHealthMetrics()

        # the request/reply handler
        self._request_reply_handler: Callable | None = None

        # NOTE: experimental
        self._decode_request_reply_handler_message: bool = False

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
    def metrics(self) -> EventServiceHealthMetrics:
        """Returns the service metrics."""
        return self._metrics

    @property
    def request_reply_handler(self) -> Callable | None:
        """Returns the request/reply handler."""
        return self._request_reply_handler

    @request_reply_handler.setter
    def request_reply_handler(self, handler: Callable) -> None:
        """Sets the request/reply handler."""
        self.add_request_reply_handler(handler)

    def add_request_reply_handler(self, handler: Callable) -> None:
        """Sets the request/reply handler."""

        params = inspect.signature(handler).parameters

        if len(params) not in (1, 2):
            msg = "Request/reply handler must have one or two parameters"
            raise ValueError(
                msg,
            )

        if len(params) == 2:  # noqa: PLR2004
            self._decode_request_reply_handler_message = True

        # is safe to set the handler
        self._request_reply_handler = handler

    @property
    def uris(self) -> dict[str, Uri]:
        """Returns the URIs of the service."""
        return self._uris

    # public methods

    def reset(self) -> None:
        """Resets the service."""
        self._client_queues = {}
        self._uris = {}
        self._latched_events = {}
        self._request_reply_handler = None
        self._metrics.reset_data()

    async def serve(self) -> None:
        """Starts the service.

        This method is called by the service manager.
        """
        self.server.add_insecure_port(f"[::]:{self.config.port}")

        self.logger.info("Starting server on port %i", self.config.port)
        await self.server.start()

        self.logger.info("Server started")

        await asyncio.gather(
            self.server.wait_for_termination(),
            self.publish_health_metrics(),
        )

    async def publish_health_metrics(self) -> None:
        """Publishes the health metrics of the service."""
        while True:
            # publish the health metrics and wait
            await self.publish(path="/health", message=self._metrics.compute_data())
            await asyncio.sleep(1.0)

    async def publish(
        self,
        path: str,
        message: Message,
        timestamps: list[Timestamp] | None = None,
        latch: bool = False,
    ) -> PublishResult:
        """Publish a message to the service.

        Args:
            path (str): The path of the message.
            message (Message): The message.
            timestamps (list[Timestamp], optional): The timestamps. Defaults to None.
        """
        # add the timestamps to the event as it passes through the service
        timestamps = timestamps or []
        timestamps.append(get_monotonic_now(semantics=StampSemantics.SERVICE_SEND))
        timestamps.append(get_system_clock_now(semantics=StampSemantics.SERVICE_SEND))

        # create the URI of the message
        uri: Uri = make_proto_uri(
            path=path,
            message=message,
            service_name=self.config.name,
        )

        # send the message to the service
        result = await self._send_raw(
            uri=uri,
            message=message,
            timestamps=timestamps,
            latch=latch,
        )

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

        if request.uri.path in self._latched_events:
            yield self._latched_events[request.uri.path]
        elif request.uri.path == "*":
            # send all latched items to the special subscriber "*"
            for reply in self._latched_events.values():
                yield reply

        # create a queue for this client
        request_queue: tuple[SubscribeRequest, asyncio.Queue] = (
            request,
            asyncio.Queue(maxsize=self.QUEUE_MAX_SIZE),
        )

        # add the queue to the list of queues
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

        # metadata to return with the reply
        event = Event()
        event.CopyFrom(request.event)
        event.uri.path = "/request" + event.uri.path

        reply_message: Message
        if self._request_reply_handler is not None:
            # decode the requested message to satisfy the handler signature
            if self._decode_request_reply_handler_message:
                message = payload_to_protobuf(request.event, request.payload)
                reply_message = await self._request_reply_handler(
                    request.event,
                    message,
                )
            else:
                reply_message = await self._request_reply_handler(request)

            if reply_message is None:
                self.logger.error(
                    "Request invalid, please check your request channel and packet %s",
                    request,
                )
        else:
            reply_message = Empty()

        reply_uri: Uri = make_proto_uri(
            path="/reply" + event.uri.path,
            message=reply_message,
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
        return RequestReplyReply(event=event, payload=reply_payload)

    # private methods

    async def _publish_event_payload(
        self,
        event: Event,
        payload: bytes,
        latch=False,
    ) -> int:
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
        if latch:
            self._latched_events[uri.path] = reply

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
            "Sending %s: %d to %d clients",
            uri.path,
            reply.event.sequence,
            len(queues),
        )

        # send the reply to the clients
        for queue in queues:
            try:
                queue.put_nowait(reply)
            except asyncio.QueueFull:
                # keep tracked of dropped messages in a data structure
                # to be able to report them to the user later
                uri_path_dropped: str = f"{uri.path}/dropped"

                num_dropped: int = int(self._metrics.get(uri_path_dropped))
                num_dropped += 1  # increment the count of dropped messages

                # update the metrics with the count of the dropped messages
                self._metrics.update_data(uri_path_dropped, num_dropped)

                self.logger.warning(
                    "dropped %d sequence %d path: %s",
                    num_dropped,
                    reply.event.sequence,
                    uri.path,
                )

        # wait for the queues to be emptied
        await asyncio.sleep(0)

        # return the number of clients the message was sent to
        return len(queues)

    async def _send_raw(
        self,
        uri: Uri,
        message: Message,
        timestamps: list[Timestamp],
        latch: bool,
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
                msg = f"Message type mismatch: {type1} != {type2}"
                raise TypeError(msg)

        # register the URI of the message
        self._uris[uri.path] = uri

        # get the count of the URI
        uri_path_send_count: str = f"{uri.path}/send_count"

        count: int = int(self._metrics.get(uri_path_send_count))

        # create the event and send
        payload: bytes = message.SerializeToString()

        event = Event(
            uri=uri,
            timestamps=timestamps,
            payload_length=len(payload),
            sequence=count,
        )

        num_clients: int = await self._publish_event_payload(
            event=event,
            payload=payload,
            latch=latch,
        )

        # update the metrics with the count of the URI
        # NOTE: even if the message is not sent, the count is incremented
        # so that the client can know that the message was not sent by checking
        # the count of the URI.
        self._metrics.update_data(uri_path_send_count, count + 1)

        # update the metrics with the count of the clients the message was sent to
        self._metrics.update_data(f"{uri.path}/send_clients", num_clients)

        # update the metrics with the timestamp of the last message
        # to compute the rate of the messages.
        stamp: float | None = get_stamp_by_semantics_and_clock_type(
            event=event,
            semantics=StampSemantics.SERVICE_SEND,
            clock_type="monotonic",
        )
        if stamp is not None:
            self._metrics.update_stamps(uri.path, stamp)

        return {
            "sequence_number": count,
            "number_clients": num_clients,
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
    if _service_config is None:
        msg = f"service {_args.service_name} not found in config list {_config_list}"
        raise ValueError(
            msg,
        )
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
    request: RequestReplyRequest,
) -> Message:
    message = payload_to_protobuf(request.event, request.payload)
    print(
        f"Received: {request.event.uri.path} {request.event.sequence} {message}".rstrip(),
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
        grpc.aio.server(),
        service_config,
    )
    loop = asyncio.get_event_loop()
    loop.run_until_complete(test_main(args, event_service))
