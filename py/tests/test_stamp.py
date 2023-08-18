from farm_ng.core import timestamp_pb2
from farm_ng.core.event_pb2 import Event
from farm_ng.core.stamp import (
    StampSemantics,
    get_monotonic_now,
    get_stamp_by_semantics_and_clock_type,
    get_system_clock_now,
    timestamp_from_monotonic,
)


def test_monotonic():
    stamp = get_monotonic_now(semantics="test/monotonic")
    assert isinstance(stamp, timestamp_pb2.Timestamp)
    assert isinstance(stamp.stamp, float)
    assert "monotonic" in stamp.clock_name
    assert stamp.semantics == "test/monotonic"


def test_system_clock():
    stamp = get_system_clock_now(semantics="test/system_clock")
    assert isinstance(stamp, timestamp_pb2.Timestamp)
    assert isinstance(stamp.stamp, float)
    assert "system_clock" in stamp.clock_name
    assert stamp.semantics == "test/system_clock"


def test_filter_stamp():
    timestamps = [
        timestamp_from_monotonic(semantics=StampSemantics.DRIVER_RECEIVE, stamp=1.23),
        timestamp_from_monotonic(semantics=StampSemantics.SERVICE_SEND, stamp=2.34),
        get_system_clock_now(semantics=StampSemantics.SERVICE_SEND),
        timestamp_from_monotonic(semantics=StampSemantics.CLIENT_RECEIVE, stamp=3.45),
        get_system_clock_now(semantics=StampSemantics.CLIENT_RECEIVE),
    ]
    event = Event(timestamps=timestamps)

    assert (
        get_stamp_by_semantics_and_clock_type(
            event,
            StampSemantics.DRIVER_RECEIVE,
            "monotonic",
        )
        == 1.23
    )
    assert (
        get_stamp_by_semantics_and_clock_type(
            event,
            StampSemantics.SERVICE_SEND,
            "monotonic",
        )
        == 2.34
    )
    assert (
        get_stamp_by_semantics_and_clock_type(
            event,
            StampSemantics.CLIENT_RECEIVE,
            "monotonic",
        )
        == 3.45
    )
    assert (
        get_stamp_by_semantics_and_clock_type(
            event,
            StampSemantics.DRIVER_TRANSMIT,
            "monotonic",
        )
        is None
    )
    assert (
        get_stamp_by_semantics_and_clock_type(
            event,
            StampSemantics.SERVICE_RECEIVE,
            "system_clock",
        )
        is None
    )
