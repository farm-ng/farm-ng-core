from farm_ng.core.event_service_metrics import EventServiceHealthMetrics


class TestEventServiceHealthMetrics:
    def test_singleton(self):
        a = EventServiceHealthMetrics()
        b = EventServiceHealthMetrics()
        assert a is b

    def test_data(self):
        a = EventServiceHealthMetrics()
        assert a.data is not None

        a.data.update({"key": "value"})
        assert a.data["key"] == "value"

        a.data.update({"key": "value2"})
        assert a.data["key"] == "value2"

        b = EventServiceHealthMetrics()
        assert b.data["key"] == "value2"

    def test_count(self):
        a = EventServiceHealthMetrics()
        a.data["/foo/send_count"] = 0

        for _ in range(10):
            a.data["/foo/send_count"] += 1

        assert a.data["/foo/send_count"] == 10
