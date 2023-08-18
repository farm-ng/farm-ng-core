from farm_ng.core import timestamp_pb2
from farm_ng.core.stamp import (
    get_monotonic_now,
    get_stamp_by_semantics,
    get_system_clock_now,
    StampSemantics,
    timestamp_from_monotonic,
)
from farm_ng.core.event_pb2 import Event


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


def test_get_semantics():
    timestamps = [
        timestamp_from_monotonic(semantics=StampSemantics.DRIVER_RECEIVE, stamp=1.23),
        timestamp_from_monotonic(semantics=StampSemantics.SERVICE_SEND, stamp=2.34),
        timestamp_from_monotonic(semantics=StampSemantics.CLIENT_RECEIVE, stamp=3.45),
    ]
    event = Event(timestamps=timestamps)

    assert get_stamp_by_semantics(event, StampSemantics.DRIVER_RECEIVE) == 1.23
    assert get_stamp_by_semantics(event, StampSemantics.SERVICE_SEND) == 2.34
    assert get_stamp_by_semantics(event, StampSemantics.CLIENT_RECEIVE) == 3.45
    assert get_stamp_by_semantics(event, StampSemantics.DRIVER_TRANSMIT) == None
