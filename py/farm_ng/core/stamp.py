import time
from farm_ng.core.timestamp_pb2 import Timestamp
from farm_ng.core.uri import get_host_name


def get_monotonic_now(semantics: str) -> Timestamp:
    return Timestamp(
        stamp=time.monotonic(),
        clock_name=get_host_name() + "/monotonic",
        semantics=semantics,
    )


def timestamp_from_monotonic(semantics: str, stamp: float) -> Timestamp:
    # stamp should be from previous time.monotonic() call
    return Timestamp(
        stamp=stamp,
        clock_name=get_host_name() + "/monotonic",
        semantics=semantics,
    )
