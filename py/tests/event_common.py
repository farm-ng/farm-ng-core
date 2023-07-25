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
                name="recorder_default",
                port=50051,
                host="localhost",
                subscriptions=[
                    SubscribeRequest(
                        uri=Uri(path="/foo", query="service_name=recorder_default"),
                        every_n=1,
                    ),
                ],
                args=["--my-arg=my-value", "foo"],
                log_level=EventServiceConfig.LogLevel.DEBUG,
            ),
        ]
    )
