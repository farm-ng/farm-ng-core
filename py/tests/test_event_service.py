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

        assert event_service._counts == {}
        assert event_service._uris == {}

        # publish a message
        await event_service.publish(path="/test", message=Int32Value(value=0))
        assert event_service._counts["/test"] == 1
        assert "Int32Value" in event_service._uris["/test"].query

        # store the uri for later
        message_uri = event_service._uris["/test"]

        # add more messages
        await event_service.publish(path="/test", message=Int32Value(value=-3))
        await event_service.publish(path="/test", message=Int32Value(value=4))
        assert event_service._counts["/test"] == 3
        assert event_service._uris["/test"] == message_uri

        # add to another path
        await event_service.publish(path="/test2", message=Int32Value(value=1))
        assert event_service._counts["/test2"] == 1
        assert event_service._uris["/test"] == message_uri

        # add to initial path another type of message
        # TODO: this test should fail because the message type is different
        await event_service.publish(path="/test", message=StringValue(value="foo"))
        assert event_service._counts["/test"] == 4
        assert event_service._uris["/test"] != message_uri
