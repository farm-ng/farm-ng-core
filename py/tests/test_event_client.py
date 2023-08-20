import asyncio

import pytest
from farm_ng.core.event_client import EventClient
from farm_ng.core.event_service import EventServiceConfig, EventServiceGrpc
from google.protobuf.wrappers_pb2 import Int32Value

from .event_common import event_service_config


def event_client() -> EventClient:
    config: EventServiceConfig = event_service_config()
    return EventClient(config)


class TestEventClient:
    def test_smoke(self) -> None:
        client: EventClient = event_client()
        assert client is not None
        assert client.config is not None
        assert client.logger.name == "test_service/client"
        assert client.server_address == "localhost:50051"

    @pytest.mark.anyio()
    async def test_publish_subscribe(self, event_service: EventServiceGrpc) -> None:
        async def subscribe_callback(client: EventClient, queue: asyncio.Queue):
            async for _, message in client.subscribe(
                request=client.config.subscriptions[0],
                decode=True,
            ):
                assert isinstance(message, Int32Value)
                await queue.put(message.value + 1)

        # reset the counts
        event_service.reset()

        client: EventClient = event_client()

        # create a queue to collect messages
        queue: asyncio.Queue[int] = asyncio.Queue()

        task = asyncio.create_task(subscribe_callback(client, queue))
        await asyncio.sleep(0.1)

        assert "/foo" in event_service._client_queues

        res = await event_service.publish(path="/foo", message=Int32Value(value=3))
        assert res.number_clients == 1

        assert await queue.get() == 4

        # cancel the task once the test is done
        if not task.done():
            task.cancel()
            try:
                await task
            except asyncio.CancelledError:
                pass
        assert task.done()
