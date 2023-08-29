from __future__ import annotations

from collections import deque

from google.protobuf.struct_pb2 import Struct

__all__ = ["EventServiceHealthMetrics"]


class Singleton(type):
    _instances: dict[type, type] = {}  # noqa: RUF012

    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super().__call__(*args, **kwargs)
        return cls._instances[cls]


class EventServiceHealthMetrics(metaclass=Singleton):
    MAX_QUEUE_SIZE: int = 50  # size of the circular buffer
    MIN_NUM_ELEMENTS: int = 2  # minimum number of elements to compute the metrics

    def __init__(self) -> None:
        # initialize the data with the default values
        self.data = Struct()
        self.stamps_buffer: dict[str, deque[float]] = {}

    # public methods

    def update_data(self, uri_path: str, value: float) -> None:
        if uri_path not in self.data:
            self.data[uri_path] = 0.0

        self.data.update({uri_path: value})

    def get(self, uri_path: str):
        if uri_path not in self.data:
            return 0

        return self.data[uri_path]

    def update_stamps(self, uri_path: str, stamp: float) -> None:
        if uri_path not in self.stamps_buffer:
            self.stamps_buffer[uri_path] = deque(maxlen=self.MAX_QUEUE_SIZE)

        self.stamps_buffer[uri_path].append(stamp)

    def compute_data(self) -> Struct:
        """Compute the metrics data that requires time based computation."""

        # iterate over the stamps buffer
        for topic, stamps in self.stamps_buffer.items():
            # compute rate
            self.data[f"{topic}/rate"] = self._compute_avg_rate(stamps)
            # compute rate iter, need to be checked
            # self.data[f"{topic}/rate"] = self._compute_avg_rate_iter(stamps)

        return self.data

    def reset_data(self) -> None:
        self.data = Struct()
        self.stamps_buffer = {}

    # private methods

    def _compute_avg_rate_iter(self, stamps: deque[float]) -> float:
        if len(stamps) < self.MIN_NUM_ELEMENTS:
            return 0.0

        latencies: list[float] = [
            stamps[i] - stamps[i - 1] for i in range(1, len(stamps))
        ]

        return len(latencies) / sum(latencies)

    def _compute_avg_rate(self, stamps: deque[float]) -> float:
        if len(stamps) < self.MIN_NUM_ELEMENTS:
            return 0.0

        duration = stamps[-1] - stamps[0]
        return len(stamps) / duration if duration > 0 else 0.0
