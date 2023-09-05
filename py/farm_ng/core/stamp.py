from __future__ import annotations

import time
from dataclasses import dataclass
from typing import TYPE_CHECKING

from farm_ng.core.timestamp_pb2 import Timestamp
from farm_ng.core.uri import get_host_name

if TYPE_CHECKING:
    from farm_ng.core.event_pb2 import Event


time_source = time


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
        stamp=time_source.monotonic(),
        clock_name=get_host_name() + "/monotonic",
        semantics=semantics,
    )


def get_system_clock_now(semantics: str) -> Timestamp:
    return Timestamp(
        stamp=time_source.time(),
        clock_name=get_host_name() + f"/system_clock/{time_source.localtime().tm_zone}",
        semantics=semantics,
    )


def timestamp_from_monotonic(semantics: str, stamp: float) -> Timestamp:
    return Timestamp(
        stamp=stamp,
        clock_name=get_host_name() + "/monotonic",
        semantics=semantics,
    )


def get_stamp_by_semantics_and_clock_type(
    event: Event,
    semantics: str,
    clock_type: str,
) -> float | None:
    """
    Utility function to get the stamp of a Timestamp by semantics.

    Args:
        event: farm-ng protobuf event
        semantics: semantics of the timestamp to get (see StampSemantics)
        clock_type: clock type of the timestamp to get (e.g., monotonic, system_clock)
    """
    for timestamp in event.timestamps:
        if timestamp.semantics == semantics and clock_type in timestamp.clock_name:
            return timestamp.stamp
    return None
