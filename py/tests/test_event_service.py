import pytest
import asyncio
import grpc

from farm_ng.core import event_service_pb2
from farm_ng.core.event_service_pb2_grpc import EventServiceStub
from farm_ng.core.event_service_pb2_grpc import add_EventServiceServicer_to_server
from farm_ng.core.event_service import EventServiceGrpc, EventServiceConfig
from .event_common import event_service_config


@pytest.fixture(scope="module")
def grpc_add_to_server():
    return add_EventServiceServicer_to_server


@pytest.fixture(scope="module", name="servicer")
def grpc_servicer(_grpc_server, config: EventServiceConfig) -> EventServiceGrpc:
    return EventServiceGrpc(_grpc_server, config)


@pytest.fixture(scope="module")
def grpc_stub_cls(grpc_channel):

    return EventServiceStub


@pytest.mark.asyncio
@pytest.fixture(scope="module")
async def grpc_server(grpc_servicer: EventServiceGrpc) -> grpc.aio.Server:
    await grpc_servicer.serve()
    yield grpc_servicer.server
    await grpc_servicer.server.stop(None)


class TestEventServiceGrpc:
    def test_smoke(
        self, config: EventServiceConfig, servicer: EventServiceGrpc
    ) -> None:
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
    async def test_list_uris(self, grpc_stub) -> None:
        import pdb

        pdb.set_trace()
        pass
        request = event_service_pb2.ListUrisRequest()
        response = await grpc_stub.ListUris(request)
        pass
