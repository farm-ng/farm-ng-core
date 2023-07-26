from farm_ng.core.event_service import (
    EventServiceConfig,
    EventServiceConfigList,
    SubscribeRequest,
)
from farm_ng.core.uri_pb2 import Uri


def event_service_config() -> EventServiceConfig:
    return EventServiceConfig(
        name="test_service",
        port=50051,
        host="localhost",
        subscriptions=[
            SubscribeRequest(
                uri=Uri(path="/foo", query="service_name=bar"),
                every_n=1,
            ),
        ],
        args=["--my-arg=my-value", "foo"],
        log_level=EventServiceConfig.LogLevel.DEBUG,
    )


def event_service_config_list() -> EventServiceConfigList:
    return EventServiceConfigList(
        configs=[
            EventServiceConfig(
                name="test_service",
                port=5001,
                host="localhost",
                subscriptions=[
                    SubscribeRequest(
                        uri=Uri(path="/foo", query="service_name=bar"),
                        every_n=1,
                    ),
                ],
                args=["--my-arg=my-value", "foo"],
                log_level=EventServiceConfig.LogLevel.DEBUG,
            ),
            EventServiceConfig(
                name="record_default",
                subscriptions=[
                    SubscribeRequest(
                        uri=Uri(path="/foo", query="service_name=test_service"),
                        every_n=1,
                    ),
                    SubscribeRequest(
                        uri=Uri(path="/bar", query="service_name=test_service"),
                        every_n=1,
                    ),
                ],
            ),
        ]
    )
