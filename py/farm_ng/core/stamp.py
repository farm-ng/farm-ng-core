import time
from dataclasses import dataclass
from farm_ng.core.timestamp_pb2 import Timestamp
from farm_ng.core.uri import get_host_name


@dataclass
class StampSemantics:
    """
    Recommended semantics for timestamp_pb2.Timestamp timestamps.

    Example usage for when a device driver (service) receives a message from a device (e.g., a camera):
        stamp = get_monotonic_now(StampSemantics.DRIVER_RECEIVE)
    """

    CLIENT_RECEIVE = "client/receive"
    CLIENT_SEND = "client/send"
    DEVICE_SAMPLE = "device/sample"
    DRIVER_RECEIVE = "driver/receive"
    DRIVER_SEND = "driver/send"
    FILE_WRITE = "file/write"


def get_monotonic_now(semantics: str) -> Timestamp:
    return Timestamp(
        stamp=time.monotonic(),
        clock_name=get_host_name() + "/monotonic",
        semantics=semantics,
    )


def get_system_clock_now(semantics: str) -> Timestamp:
    return Timestamp(
        stamp=time.time(),
        clock_name=get_host_name() + f"/system_clock/{time.localtime().tm_zone}",
        semantics=semantics,
    )


def timestamp_from_monotonic(semantics: str, stamp: float) -> Timestamp:
    # stamp should be from previous time.monotonic() call
    return Timestamp(
        stamp=stamp,
        clock_name=get_host_name() + "/monotonic",
        semantics=semantics,
    )
