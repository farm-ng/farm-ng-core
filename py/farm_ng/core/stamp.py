import time
from farm_ng.core.timestamp_pb2 import Timestamp
import platform

_g_host_name = platform.node()


def set_host_name(name: str) -> None:
    global _g_host_name
    _g_host_name = name


def get_host_name() -> str:
    return _g_host_name


def get_monotonic_now(semantics: str) -> Timestamp:
    return Timestamp(
        stamp=time.monotonic(),
        clock_name=get_host_name() + "/monotonic",
        semantics=semantics,
    )
