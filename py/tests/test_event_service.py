from __future__ import annotations

import asyncio

import grpc
import pytest
from farm_ng.core.event_client import EventClient
from farm_ng.core.event_service import EventServiceGrpc
from farm_ng.core.event_service_pb2 import EventServiceConfig, SubscribeRequest
from farm_ng.core.uri_pb2 import Uri
from google.protobuf.wrappers_pb2 import Int32Value, StringValue

from .event_common import event_service_config


class TestEventServiceGrpc:
    @pytest.fixture(scope="module")
    async def test_smoke(self) -> None:
        server = grpc.aio.server()
        # create a service
        servicer = EventServiceGrpc(server=server, config=event_service_config())

        config: EventServiceConfig = servicer.config

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

        # await server.stop(grace=0.5)

    @pytest.fixture(scope="module")
    async def test_publish(self) -> None:
        server = grpc.aio.server()
        # create a service
        event_service = EventServiceGrpc(server=server, config=event_service_config())

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

        # await server.stop(grace=0.5)

    @pytest.fixture(scope="module")
    async def test_publish_error(self) -> None:
        server = grpc.aio.server()
        # create a service
        event_service = EventServiceGrpc(server=server, config=event_service_config())

        # start the server
        asyncio.create_task(event_service.serve())

        # publish a message
        await event_service.publish(path="/foo", message=StringValue(value="foo"))

        # publish a message with a different type
        with pytest.raises(
            TypeError,
            match="Message type mismatch: StringValue != Int32Value",
        ):
            await event_service.publish(path="/foo", message=Int32Value(value=0))

        # await server.stop(grace=0.5)

    @pytest.fixture(scope="module")
    async def test_multiple_publishers(self) -> None:
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

        server = grpc.aio.server()

        # create a service
        event_service = EventServiceGrpc(server=server, config=event_service_config())

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

        # await server.stop(grace=0.5)

    @pytest.fixture(scope="module")
    async def test_latch(self) -> None:
        await asyncio.sleep(0.1)
        print("starting test_latch")

        config = EventServiceConfig(
            name="test_latch",
            port=50051,
            host="localhost",
            log_level=EventServiceConfig.LogLevel.DEBUG,
        )

        server = grpc.aio.server()
        # create a service
        event_service = EventServiceGrpc(server, config=config)

        # start the server
        asyncio.create_task(event_service.serve())

        await asyncio.sleep(0.1)

        await event_service.publish(
            path="/latchy",
            message=Int32Value(value=42),
            latch=True,
        )
        print("published latch")

        await asyncio.sleep(0.1)
        client: EventClient = EventClient(config=config)

        async for _, message in client.subscribe(
            request=SubscribeRequest(
                uri=Uri(path="/latchy"),
                every_n=1,
            ),
            decode=True,
        ):
            assert isinstance(message, Int32Value)
            print(message)
            assert message.value == 42
            break

        # await server.stop(grace=0.5)

    @pytest.mark.skip(reason="TODO: implement me")
    def test_list_uris(self) -> None:
        pass
