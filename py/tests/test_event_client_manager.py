import pytest
from farm_ng.core import uri_pb2
from farm_ng.core.event_client_manager import EventClientSubscriptionManager
from farm_ng.core.event_service_pb2 import (
    EventServiceConfigList,
)


@pytest.fixture()
def event_manager(
    event_service_config_list: EventServiceConfigList,
) -> EventClientSubscriptionManager:
    config_list = EventServiceConfigList()
    config_list.configs.append(event_service_config_list.configs[0])
    return EventClientSubscriptionManager(config_list)


class TestEventServiceBackend:
    def test_smoke(self) -> None:
        event_manager = EventClientSubscriptionManager(
            EventServiceConfigList(),
        )
        assert event_manager is not None
        assert event_manager._clients == {}
        assert event_manager._subscriptions == {}

    def test_initialize_clients(
        self,
        event_manager: EventClientSubscriptionManager,
    ) -> None:
        assert len(event_manager._clients) == 1
        assert "test_service" in event_manager._clients
        assert event_manager._clients["test_service"].config.name == "test_service"
        assert event_manager._clients["test_service"].config.port == 5001
        assert event_manager._clients["test_service"].config.host == "localhost"

    def test_register_topic(
        self,
        event_manager: EventClientSubscriptionManager,
    ) -> None:
        # register a topic
        uri_foo: uri_pb2.Uri = uri_pb2.Uri(
            scheme="protobuf",
            authority="localhost",
            path="/foo",
            query="type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto&service_name=test_service",
        )
        assert event_manager._try_register_topic("test_service", uri=uri_foo)
        assert "test_service/foo" in event_manager._subscriptions
        assert event_manager._subscriptions["test_service/foo"].uri.path == "/foo"
        assert (
            event_manager._subscriptions["test_service/foo"].uri.query
            == "service_name=test_service"
        )
        assert event_manager._subscriptions["test_service/foo"].every_n == 1

        # register a second topic, on a sub-service
        uri_bar: uri_pb2.Uri = uri_pb2.Uri(
            path="/bar",
            query="type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto&service_name=sub_service",
        )
        assert event_manager._try_register_topic("test_service", uri_bar)
        assert "sub_service/bar" in event_manager._subscriptions
        assert event_manager._subscriptions["sub_service/bar"].uri.path == "/bar"
        assert (
            event_manager._subscriptions["sub_service/bar"].uri.query
            == "service_name=sub_service"
        )
        assert event_manager._subscriptions["sub_service/bar"].every_n == 1

        # register a topic for a service that doesn't exist
        assert not event_manager._try_register_topic("unexisting_service", uri_bar)

        # register a topic that already exists
        assert not event_manager._try_register_topic("test_service", uri_foo)

    def test_get_all_uris(self, event_manager: EventClientSubscriptionManager) -> None:
        uri: uri_pb2.Uri = uri_pb2.Uri(
            scheme="protobuf",
            authority="localhost",
            path="/foo",
            query="type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto&service_name=sub_service",
        )
        assert event_manager._try_register_topic("test_service", uri)
        config_list = event_manager.get_all_uris_config_list(
            config_name="record_default",
        )

        assert len(config_list.configs) == 2
        assert config_list.configs[1].name == "record_default"
        assert len(config_list.configs[1].subscriptions) == 1
        assert config_list.configs[1].subscriptions[0].uri.path == "/foo"
        assert (
            config_list.configs[1].subscriptions[0].uri.query
            == "service_name=sub_service"
        )
        assert config_list.configs[1].subscriptions[0].every_n == 1
