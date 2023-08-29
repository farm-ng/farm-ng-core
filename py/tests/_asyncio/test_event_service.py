from __future__ import annotations

import asyncio
from typing import TYPE_CHECKING

import pytest
from farm_ng.core.event_client import EventClient
from farm_ng.core.event_service_pb2 import SubscribeRequest
from farm_ng.core.uri_pb2 import Uri
from google.protobuf.wrappers_pb2 import Int32Value, StringValue

if TYPE_CHECKING:
    from farm_ng.core.event_service import EventServiceGrpc


class TestEventServiceGrpc:
    @pytest.mark.anyio()
    async def test_smoke(self, event_service: EventServiceGrpc) -> None:
        # reset the counts
        event_service.reset()
        assert event_service is not None
        assert event_service.QUEUE_MAX_SIZE == 10
        assert event_service.server is not None
        assert event_service.logger is not None
        assert event_service.logger.name == "test_service"
        assert event_service.time_started > 0.0
        assert not event_service.uris
        assert event_service.request_reply_handler is None

    @pytest.mark.anyio()
    async def test_publish(self, event_service: EventServiceGrpc) -> None:
        # reset the counts
        event_service.reset()

        # publish a message
        res = await event_service.publish(path="/foo", message=Int32Value(value=0))
        assert event_service.metrics.data["/foo/send_count"] == 1
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
        assert event_service.metrics.data["/foo/send_count"] == 3
        assert event_service.uris["/foo"] == message_uri

        # add to another path
        res = await event_service.publish(path="/bar", message=Int32Value(value=1))
        assert res.sequence_number == 0
        assert res.number_clients == 0
        assert event_service.metrics.data["/bar/send_count"] == 1
        assert event_service.uris["/bar"].query == message_uri.query

    @pytest.mark.anyio()
    async def test_publish_error(self, event_service: EventServiceGrpc) -> None:
        # reset the counts
        event_service.reset()

        # publish a message
        await event_service.publish(path="/foo", message=StringValue(value="foo"))

        # publish a message with a different type
        with pytest.raises(
            TypeError,
            match="Message type mismatch: StringValue != Int32Value",
        ):
            await event_service.publish(path="/foo", message=Int32Value(value=0))

    @pytest.mark.anyio()
    async def test_multiple_publishers(self, event_service: EventServiceGrpc) -> None:
        async def _publish_message(
            event_service: EventServiceGrpc,
            path: str,
            num_messages: int,
            delay: float,
        ) -> bool:
            """Publishes a message to the event service."""
            for i in range(num_messages):
                await event_service.publish(path=path, message=Int32Value(value=i))
                await asyncio.sleep(delay)
            return True

        # reset the counts
        event_service.reset()

        # create multiple publishers
        async_tasks = []
        async_tasks.append(_publish_message(event_service, "/foo", 2, 0.2))
        async_tasks.append(_publish_message(event_service, "/bar", 3, 0.1))

        res = await asyncio.gather(*async_tasks)
        assert res == [True, True]
        assert event_service.metrics.data["/foo/send_count"] == 2
        assert event_service.metrics.data["/bar/send_count"] == 3

    @pytest.mark.anyio()
    async def test_latch(self, event_service: EventServiceGrpc) -> None:
        # reset the counts
        event_service.reset()

        # send a latched message
        await event_service.publish(
            path="/latchy",
            message=Int32Value(value=42),
            latch=True,
        )

        client: EventClient = EventClient(config=event_service.config)

        async for _, message in client.subscribe(
            request=SubscribeRequest(
                uri=Uri(path="/latchy"),
                every_n=1,
            ),
            decode=True,
        ):
            assert isinstance(message, Int32Value)
            assert message.value == 42
            break

        # test "*" subscriber
        async for _, message in client.subscribe(
            request=SubscribeRequest(
                uri=Uri(path="*"),
                every_n=1,
            ),
            decode=True,
        ):
            assert isinstance(message, Int32Value)
            assert message.value == 42
            break

    @pytest.mark.skip(reason="TODO: implement me")
    def test_list_uris(self) -> None:
        pass
