import pytest
from farm_ng.core.event_client import EventClient
from farm_ng.core.event_service import EventServiceConfig

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

    # TODO: mock the server
    # @pytest.mark.asyncio
    # async def test_uris(self, client: EventClient) -> None:
    #    await asyncio.sleep(0.1)
    #    uris = await client.list_uris()
    #    assert uris is not None
    #    assert len(uris) == 0
