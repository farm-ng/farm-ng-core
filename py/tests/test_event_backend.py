import pytest
from farm_ng.core.event_service_backend import EventServiceBackend
from farm_ng.core.event_service_pb2 import (
    EventServiceConfig,
    EventServiceConfigList,
)


@pytest.fixture()
def event_backend(
    event_service_config: EventServiceConfig,
    event_service_config_list: EventServiceConfigList,
) -> EventServiceBackend:
    return EventServiceBackend(event_service_config, event_service_config_list)


class TestEventServiceBackend:
    def test_smoke(self) -> None:
        event_backend = EventServiceBackend(
            EventServiceConfig(),
            EventServiceConfigList(),
        )
        assert event_backend is not None
        assert event_backend._service_config is not None
        assert event_backend._clients == {}
        assert event_backend._subscriptions == {}

    def test_initialize_clients(self, event_backend: EventServiceBackend) -> None:
        assert len(event_backend._clients) == 1
        assert "test_service" in event_backend._clients
        assert event_backend._clients["test_service"].config.name == "test_service"
        assert event_backend._clients["test_service"].config.port == 5001
        assert event_backend._clients["test_service"].config.host == "localhost"

    def test_register_topic(self, event_backend: EventServiceBackend) -> None:
        # register a topic
        assert event_backend._try_register_topic("test_service", "/foo")
        assert "test_service/foo" in event_backend._subscriptions
        assert event_backend._subscriptions["test_service/foo"].uri.path == "/foo"
        assert (
            event_backend._subscriptions["test_service/foo"].uri.query
            == "service_name=test_service"
        )
        assert event_backend._subscriptions["test_service/foo"].every_n == 1

        # register a second topic
        assert event_backend._try_register_topic("test_service", "/bar")
        assert "test_service/bar" in event_backend._subscriptions
        assert event_backend._subscriptions["test_service/bar"].uri.path == "/bar"
        assert (
            event_backend._subscriptions["test_service/bar"].uri.query
            == "service_name=test_service"
        )
        assert event_backend._subscriptions["test_service/bar"].every_n == 1

        # register a topic for a service that doesn't exist
        assert not event_backend._try_register_topic("unexisting_service", "/bar")

        # register a topic that already exists
        assert not event_backend._try_register_topic("test_service", "/foo")

    def test_get_all_uris(self, event_backend: EventServiceBackend) -> None:
        assert event_backend._try_register_topic("test_service", "/foo")
        config_list = event_backend.get_all_uris_config_list()

        assert len(config_list.configs) == 1
        assert config_list.configs[0].name == "test_service_default"
        assert len(config_list.configs[0].subscriptions) == 1
        assert config_list.configs[0].subscriptions[0].uri.path == "/foo"
        assert (
            config_list.configs[0].subscriptions[0].uri.query
            == "service_name=test_service"
        )
        assert config_list.configs[0].subscriptions[0].every_n == 1
