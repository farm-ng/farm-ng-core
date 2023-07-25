import pytest
import asyncio
import grpc

from google.protobuf.wrappers_pb2 import Int32Value, StringValue
from google.protobuf.message import Message

from farm_ng.core.events_file_reader import payload_to_protobuf
from farm_ng.core.event_service_pb2 import (
    EventServiceConfig,
    RequestReplyRequest,
)
from farm_ng.core.event_service import EventServiceGrpc, EventServiceConfig
from .event_common import event_service_config


async def request_reply_handler(
    event_service: EventServiceGrpc, request: RequestReplyRequest
) -> Message:
    message = payload_to_protobuf(request.event, request.payload)
    event_service.logger.info(
        f"Received: {request.event.uri.path} {request.event.sequence} {message}".rstrip()
    )
    return message  # echo message back


class TestEventServiceGrpc:
    def test_smoke(self) -> None:
        config: EventServiceConfig = event_service_config()
        servicer: EventServiceGrpc = EventServiceGrpc(grpc.aio.server(), config)
        assert servicer is not None
        assert servicer.server is not None
        assert servicer.config == config
        assert servicer.logger is not None
        assert servicer.logger.name == "test_service"
        assert servicer.time_started > 0.0
        assert servicer._client_queues == {}
        assert servicer._uris == {}
        assert servicer._counts == {}
        assert servicer._request_reply_handler is None

    @pytest.mark.asyncio
    async def test_publish(self) -> None:
        # create a service
        event_service = EventServiceGrpc(grpc.aio.server(), event_service_config())

        # start the server
        asyncio.create_task(event_service.serve())

        assert not event_service.counts
        assert not event_service.uris

        # publish a message
        res = await event_service.publish(path="/foo", message=Int32Value(value=0))
        assert event_service.counts["/foo"] == 1
        assert res.sequence_number == 0
        assert res.number_clients == 0

        # check that the class name is in the uri and store the uri for later
        assert "Int32Value" in event_service.uris["/foo"].query
        message_uri = event_service.uris["/foo"]

        # add more messages
        await event_service.publish(path="/foo", message=Int32Value(value=-3))
        res = await event_service.publish(path="/foo", message=Int32Value(value=4))
        assert res.sequence_number == 2
        assert res.number_clients == 0
        assert event_service.counts["/foo"] == 3
        assert event_service.uris["/foo"] == message_uri

        # add to another path
        res = await event_service.publish(path="/bar", message=Int32Value(value=1))
        assert res.sequence_number == 0
        assert res.number_clients == 0
        assert event_service.counts["/bar"] == 1
        assert event_service.uris["/bar"].query == message_uri.query

    @pytest.mark.asyncio
    async def test_publish_error(self) -> None:
        # create a service
        event_service = EventServiceGrpc(grpc.aio.server(), event_service_config())

        # start the server
        asyncio.create_task(event_service.serve())

        # publish a message
        await event_service.publish(path="/foo", message=StringValue(value="foo"))

        # publish a message with a different type
        with pytest.raises(
            TypeError, match="Message type mismatch: StringValue != Int32Value"
        ):
            await event_service.publish(path="/foo", message=Int32Value(value=0))
