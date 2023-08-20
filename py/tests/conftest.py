import asyncio
from typing import AsyncGenerator

import grpc
import pytest
from farm_ng.core.event_service import EventServiceGrpc

from .event_common import event_service_config


@pytest.fixture(scope="session")
def anyio_backend():
    return "asyncio"


@pytest.fixture(scope="module")
async def server() -> AsyncGenerator[grpc.aio.Server, None]:
    print("creating grpc server")

    server = grpc.aio.server()
    yield server

    try:
        await server.stop(0.5)
    except asyncio.CancelledError:
        pass

    assert not server._server.is_running()
    print("server stopped")


@pytest.mark.anyio()
@pytest.fixture(scope="module")
async def event_service(
    server: grpc.aio.Server,
) -> AsyncGenerator[EventServiceGrpc, None]:
    # create a servicer
    servicer = EventServiceGrpc(server=server, config=event_service_config())

    task = asyncio.create_task(servicer.serve())

    yield servicer

    # cancel the task once the test is done
    if not task.done():
        task.cancel()
        try:
            await task
        except asyncio.CancelledError:
            pass
