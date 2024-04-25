import asyncio
from typing import AsyncGenerator

import grpc
import pytest
from farm_ng.core.event_client import EventClient
from farm_ng.core.event_service import (
    EventServiceConfig,
    EventServiceConfigList,
    EventServiceGrpc,
)
from farm_ng.core.event_service_recorder import EventServiceRecorder

from .event_common import event_service_config_list as _event_service_config_list

pytestmark = pytest.mark.anyio


@pytest.fixture(scope="session")
def anyio_backend():
    return "asyncio"


@pytest.fixture(scope="session")
def event_service_config_list() -> EventServiceConfigList:
    return _event_service_config_list()


@pytest.fixture(scope="session")
def event_service_config(
    event_service_config_list: EventServiceConfigList,
) -> EventServiceConfig:
    return event_service_config_list.configs[0]


@pytest.fixture(scope="module")
async def event_service(
    event_service_config: EventServiceConfig,
) -> AsyncGenerator[EventServiceGrpc, None]:
    # create the server
    server = grpc.aio.server()

    # create a servicer
    servicer = EventServiceGrpc(server=server, config=event_service_config)

    # create a task to run the servicer
    task = asyncio.create_task(servicer.serve())

    yield servicer

    # cancel the task once the test is done
    if not task.done():
        task.cancel()
        try:
            await task
        except asyncio.CancelledError:
            pass

    # make sure to stop the server
    try:
        await server.stop(0.5)
    except asyncio.CancelledError:
        pass

    assert not server._server.is_running()


@pytest.fixture(scope="module")
def recorder_service(
    event_service_config_list: EventServiceConfigList,
) -> EventServiceRecorder:
    return EventServiceRecorder(
        service_name="record_default",
        config_list=event_service_config_list,
    )


@pytest.fixture()
def event_client(event_service_config: EventServiceConfig) -> EventClient:
    return EventClient(event_service_config)
