import asyncio

import pytest
from farm_ng.core.event_client import EventClient
from farm_ng.core.event_service import EventServiceGrpc
from farm_ng.core.event_service_pb2 import (
    RequestReplyRequest,
)
from google.protobuf.empty_pb2 import Empty
from google.protobuf.message import Message
from google.protobuf.wrappers_pb2 import Int32Value, StringValue


class TestEventClient:
    def test_smoke(self, event_client: EventClient) -> None:
        assert event_client is not None
        assert event_client.config is not None
        assert event_client.logger.name == "test_service/client"
        assert event_client.server_address == "localhost:5001"

    @pytest.mark.anyio()
    async def test_publish_subscribe(
        self,
        event_service: EventServiceGrpc,
        event_client: EventClient,
    ) -> None:
        async def subscribe_callback(client: EventClient, queue: asyncio.Queue):
            async for _, message in client.subscribe(
                request=client.config.subscriptions[0],
                decode=True,
            ):
                assert isinstance(message, Int32Value)
                await queue.put(message.value + 1)

        # reset the counts
        event_service.reset()

        # create a queue to collect messages
        queue: asyncio.Queue[int] = asyncio.Queue()

        task = asyncio.create_task(subscribe_callback(event_client, queue))
        await asyncio.sleep(0.1)

        assert "/foo" in event_service._client_queues

        # make sure the message is in the queue and decoded properly
        res = await event_service.publish(path="/foo", message=Int32Value(value=3))
        assert res.number_clients == 1
        assert res.sequence_number == 0

        # make sure the message is in the queue and decoded properly
        res = await event_service.publish(path="/foo", message=Int32Value(value=4))
        assert res.number_clients == 1
        assert res.sequence_number == 1

        assert await queue.get() == 4
        assert await queue.get() == 5

        # cancel the task once the test is done
        if not task.done():
            task.cancel()
            try:
                await task
            except asyncio.CancelledError:
                pass
        assert task.done()

    @pytest.mark.anyio()
    async def test_request_reply(
        self,
        event_service: EventServiceGrpc,
        event_client: EventClient,
    ) -> None:
        async def request_reply_handler(
            request: RequestReplyRequest,
        ) -> Message:
            if request.event.uri.path == "/get_foo":
                return StringValue(value="foo")
            if request.event.uri.path == "/get_bar":
                return StringValue(value="bar")
            if request.event.uri.path == "/await":
                await asyncio.sleep(0.1)
                return Empty()
            return None

        # reset the counts
        event_service.reset()
        event_service.request_reply_handler = request_reply_handler

        # get decoded response
        res = await event_client.request_reply("/get_foo", Empty(), decode=True)
        assert res.value == "foo"

        res = await event_client.request_reply("/get_bar", Empty(), decode=True)
        assert res.value == "bar"

        res = await event_client.request_reply("/await", Empty(), decode=True)
        assert isinstance(res, Empty)

        # get raw response
        res = await event_client.request_reply("/get_foo", Empty(), decode=False)
        assert res.event.uri.path == "/reply/request/get_foo"
        assert "StringValue" in res.event.uri.query
        assert res.payload == b"\n\x03foo"
