from farm_ng.core import timestamp_pb2
from farm_ng.core.stamp import get_monotonic_now, get_system_clock_now


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
