from __future__ import annotations

import asyncio

import grpc
import pytest
from farm_ng.core.event_service import EventServiceConfig, EventServiceGrpc
from google.protobuf.wrappers_pb2 import Int32Value, StringValue

from .event_common import event_service_config


class TestEventServiceGrpc:
    def test_smoke(self) -> None:
        config: EventServiceConfig = event_service_config()
        servicer: EventServiceGrpc = EventServiceGrpc(grpc.aio.server(), config)
        assert servicer is not None
        assert servicer.QUEUE_MAX_SIZE == 10
        assert servicer.server is not None
        assert servicer.config == config
        assert servicer.logger is not None
        assert servicer.logger.name == "test_service"
        assert servicer.time_started > 0.0
        assert not servicer.uris
        assert not servicer.counts
        assert servicer.request_reply_handler is None

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

    @pytest.mark.asyncio
    async def test_multiple_publishers(self) -> None:
        async def _publish_message(
            event_service: EventServiceGrpc, path: str, num_messages: int, delay: float
        ) -> bool:
            """Publishes a message to the event service."""
            for i in range(num_messages):
                await event_service.publish(path=path, message=Int32Value(value=i))
                await asyncio.sleep(delay)
            return True

        # create a service
        event_service = EventServiceGrpc(grpc.aio.server(), event_service_config())

        # start the server
        asyncio.create_task(event_service.serve())

        # create multiple publishers
        async_tasks = []
        async_tasks.append(_publish_message(event_service, "/foo", 2, 0.2))
        async_tasks.append(_publish_message(event_service, "/bar", 3, 0.1))

        res = await asyncio.gather(*async_tasks)
        assert res == [True, True]
        assert event_service.counts["/foo"] == 2
        assert event_service.counts["/bar"] == 3

    @pytest.mark.skip(reason="TODO: implement me")
    def test_list_uris(self) -> None:
        pass
