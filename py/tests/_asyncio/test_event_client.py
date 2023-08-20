import asyncio

import pytest
from farm_ng.core.event_client import EventClient
from farm_ng.core.event_service import EventServiceGrpc
from google.protobuf.wrappers_pb2 import Int32Value


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
