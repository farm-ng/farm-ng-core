from __future__ import annotations

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

    # When a gRPC client receives a message from a gRPC server.
    CLIENT_RECEIVE = "client/receive"
    # When a gRPC client sends a message to a gRPC server.
    CLIENT_SEND = "client/send"
    # When a device (e.g., camera) takes a measurement.
    DEVICE_SAMPLE = "device/sample"
    # When a device driver (gRPC service) receives a measurement/message from a device.
    DRIVER_RECEIVE = "driver/receive"
    # When a device driver (gRPC service) sends a message to a device.
    DRIVER_TRANSMIT = "driver/transmit"
    # When a message is written to a file on disk.
    FILE_WRITE = "file/write"
    # When a gRPC service (which may be a device driver) receives a message
    # from a gRPC client or another gRPC service. This is distinct from DRIVER_RECEIVE,
    # which is when a device driver receives a message from a device.
    SERVICE_RECEIVE = "service/receive"
    # When a gRPC service (which may be a device driver) sends a message
    SERVICE_SEND = "service/send"


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


def get_stamp_by_semantics(event, semantics: str) -> float | None:
    """
    Utility function to get the stamp of a Timestamp by semantics.

    Args:
        event: farm-ng protobuf event
        semantics: semantics of the timestamp to get (see StampSemantics)
    """
    for timestamp in event.timestamps:
        if timestamp.semantics == semantics:
            return timestamp.stamp
    return None
