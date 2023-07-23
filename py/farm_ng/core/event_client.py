"""
# This will connect to the test_service and print out the messages it receives
python -m farm_ng.core.event_client --service-config config.json --service-name test_service
"""
from __future__ import annotations

from typing import AsyncIterator
import argparse
import asyncio
import logging
import grpc

from farm_ng.core import event_service_pb2_grpc
from farm_ng.core.stamp import get_monotonic_now, get_system_clock_now, StampSemantics
from farm_ng.core.uri_pb2 import Uri
from farm_ng.core.event_service_pb2 import (
    EventServiceConfig,
    SubscribeReply,
    SubscribeRequest,
    ListUrisRequest,
    ListUrisReply,
    RequestReplyReply,
    RequestReplyRequest,
)
from farm_ng.core.events_file_reader import payload_to_protobuf
from farm_ng.core.event_pb2 import Event
from farm_ng.core.timestamp_pb2 import Timestamp
from farm_ng.core.events_file_writer import make_proto_uri
from farm_ng.core.event_service import load_service_config, add_service_parser
from google.protobuf.message import Message

logging.basicConfig(level=logging.INFO)


class EventClient:
    """Generic client class to connect with the Amiga brain services.

    Internally implements an `asyncio` gRPC channel that is designed to be imported
    by service specific clients.
    """

    def __init__(self, config: EventServiceConfig) -> None:
        """Initializes the client.

        Args:
            config (EventServiceConfig): the grpc configuration data structure.

        Raises:
            ValueError: if the port or host are invalid.
        """
        if config.port == 0:
            raise ValueError(
                f"Invalid port: {config}, are you sure this is a client config?"
            )

        if config.host == "":
            raise ValueError(
                f"Invalid host: {config}, are you sure this is a client config?"
            )

        # the configuration data structure
        self._config: EventServiceConfig = config

        # the client logger
        self._logger: logging.Logger = logging.getLogger(f"{self.config.name}/client")

        # the number of messages sent
        self._counts: dict[str, int] = {}

        # the gRPC channel and stub
        self.channel: grpc.aio.Channel | None = None
        self.stub: event_service_pb2_grpc.EventServiceStub | None = None

    @property
    def config(self) -> EventServiceConfig:
        """Returns the configuration data structure."""
        return self._config

    @property
    def logger(self) -> logging.Logger:
        """Returns the logger."""
        return self._logger

    @property
    def server_address(self) -> str:
        """Returns the composed address and port."""
        return f"{self.config.host}:{self.config.port}"

    async def _try_connect(self) -> bool:
        """Tries to connect to the server.

        Returns:
            bool: True if the connection was successful, False otherwise.
        """
        if self.stub is not None and await self.channel.channel_ready():
            self.logger.debug("Already connected to %s", self.server_address)
            return True

        try:
            # create an async connection with the server
            self.channel = grpc.aio.insecure_channel(self.server_address)
            await self.channel.channel_ready()
            self.stub = event_service_pb2_grpc.EventServiceStub(self.channel)

        except OSError as exc:
            self.logger.warning("Could not connect to %s: %s", self.server_address, exc)
            await asyncio.sleep(1)
            return False

        return True

    async def subscribe(
        self, request: SubscribeRequest, decode: bool = True
    ) -> tuple[Event, Message | bytes]:
        """Subscribes to the server.

        Args:
            request (SubscribeRequest): the subscription request.
            decode (bool, optional): if True, the payload will be decoded. Defaults to True.

        Yields:
            tuple[Event, Message | bytes]: the event and the payload.
        """
        # the streaming object
        response_stream: AsyncIterator[SubscribeReply] | None = None

        while True:
            # check if we are connected to the server
            if not await self._try_connect():
                if response_stream is not None:
                    response_stream.cancel()
                    response_stream = None
                self.logger.warning(
                    "%s is not streaming or ready to stream", self.config
                )
                continue

            if response_stream is None:
                # get the streaming object
                response_stream = self.stub.subscribe(request)

            try:
                # try/except so app doesn't crash on killed service
                response: SubscribeReply = await response_stream.read()
                response.event.timestamps.append(
                    get_monotonic_now(StampSemantics.DRIVER_RECEIVE)
                )
                assert response and response != grpc.aio.EOF, "End of stream"
            except grpc.RpcError as exc:
                self.logger.warning("here %s", exc)
                response_stream.cancel()
                response_stream = None
                continue
            except AssertionError as exc:
                self.logger.warning("End of stream %s", exc)
                response_stream.cancel()
                response_stream = None
                continue

            # decode the payload if requested
            if decode:
                yield response.event, payload_to_protobuf(
                    response.event, response.payload
                )
            else:
                yield response.event, response.payload

    async def list_uris(self) -> list[Uri]:
        """Returns the list of uris.

        Returns:
            list[Uri]: the list of uris.
        """
        # try to connect to the server, if it fails return an empty list
        if not await self._try_connect():
            self.logger.warning("Could not list uris: %s", self.server_address)
            return []

        try:
            response: ListUrisReply = await self.stub.listUris(ListUrisRequest())
            return response.uris
        except grpc.RpcError as err:
            self.logger.warning(
                "Could not list uris: %s\n err=%s", self.server_address, err
            )
            return []

    # TODO: rename to `request_reply`
    async def request_reply(
        self, path: str, message: Message, timestamps: list[Timestamp] | None = None
    ) -> RequestReplyReply:
        """Sends a request and waits for a reply.

        Args:
            path (str): the path of the request.
            message (Message): the message to send.
            timestamps (list[Timestamp], optional): the timestamps to add to the event.

        Returns:
            ReqRepReply: the reply.
        """
        # try to connect to the server, if it fails return an emmpty response
        if not await self._try_connect():
            self.logger.warning("Could not send: %s", self.server_address)
            return RequestReplyReply()

        # get the current count and increment it
        count: int = self._counts.get(path, 0)
        self._counts[path] = count + 1

        # add the timestamps before sending the message
        timestamps = timestamps or []
        timestamps.append(get_monotonic_now(semantics=StampSemantics.CLIENT_SEND))
        timestamps.append(get_system_clock_now(semantics=StampSemantics.CLIENT_SEND))

        # create the request, serialize the message and send it
        uri: Uri = make_proto_uri(path=path, message=message)
        uri.query += f"&service_name={self.config.name}"

        payload = message.SerializeToString()
        request = RequestReplyRequest(
            event=Event(
                uri=uri,
                timestamps=timestamps,
                payload_length=len(payload),
                sequence=count,
            ),
            payload=payload,
        )

        # send the request and wait for the reply
        reply: RequestReplyReply = await self.stub.requestReply(request)
        reply.event.timestamps.append(
            get_monotonic_now(semantics=StampSemantics.CLIENT_SEND)
        )
        return reply


async def test_subscribe(client: EventClient, uri: Uri):
    print(uri)
    # return
    async for event, message in client.subscribe(SubscribeRequest(uri=uri, every_n=1)):

        if not uri.path.startswith("/request") and not uri.path.startswith("/reply"):
            reply = await client.request_reply(event.uri.path, message)
            print("reply:", reply)
            # print(event.uri.path, event.sequence, message, reply.event.sequence)
        else:
            print(event.uri.path, event.sequence)


async def test_smoke():
    parser = argparse.ArgumentParser()
    add_service_parser(parser)
    args = parser.parse_args()
    config_list, service_config = load_service_config(args)
    client = EventClient(service_config)
    async_subscriptions: list[asyncio.Task] = []

    uris = []
    while True:
        uris = await client.list_uris()
        if len(uris) > 1:
            break
        await asyncio.sleep(1)

    for uri in uris:
        print(uri.path)
        async_subscriptions.append(asyncio.create_task(test_subscribe(client, uri)))

    await asyncio.gather(*async_subscriptions)


if __name__ == "__main__":

    asyncio.run(test_smoke())
