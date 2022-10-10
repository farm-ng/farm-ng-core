from farm_ng.core import timestamp_pb2
from farm_ng.core.stamp import get_monotonic_now


def test_monotonic():
    stamp = get_monotonic_now(semantics="test/monotonic")
    assert isinstance(stamp, timestamp_pb2.Timestamp)
    assert isinstance(stamp.stamp, float)
    assert "monotonic" in stamp.clock_name
    assert stamp.semantics == "test/monotonic"
