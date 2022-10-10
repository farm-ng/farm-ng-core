import time
from farm_ng.core.timestamp_pb2 import Timestamp
from farm_ng.core.uri import get_host_name


def get_monotonic_now(semantics: str) -> Timestamp:
    return Timestamp(
        stamp=time.monotonic(),
        clock_name=get_host_name() + "/monotonic",
        semantics=semantics,
    )
