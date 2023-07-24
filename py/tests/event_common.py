import pytest
from farm_ng.core.event_service import EventServiceConfig, SubscribeRequest
from farm_ng.core.uri_pb2 import Uri


@pytest.fixture(scope="module", name="config")
def event_service_config() -> EventServiceConfig:
    return EventServiceConfig(
        name="test_service",
        port=50051,
        host="localhost",
        subscriptions=[
            SubscribeRequest(
                uri=Uri(path="/test", query="service_name=bar"),
                every_n=1,
            ),
        ],
        python_module="farm_ng.core.event_service",
        args=["--my-arg=my-value", "foo"],
        log_level=EventServiceConfig.LogLevel.DEBUG,
    )
