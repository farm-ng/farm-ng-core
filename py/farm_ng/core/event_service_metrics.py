from __future__ import annotations

from google.protobuf.struct_pb2 import Struct

__all__ = ["EventServiceHealthMetrics"]


class Singleton(type):
    _instances = {}  # noqa: RUF012

    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super().__call__(*args, **kwargs)
        return cls._instances[cls]


class EventServiceHealthMetrics(metaclass=Singleton):
    def __init__(self) -> None:
        # initialize the data with the default values
        self.data = Struct()
        self.data_tmp = {}

    def get_data(self) -> Struct:
        # update the topics rates
        for topic, times in self.data_tmp.items():
            if len(times) == 0:
                duration = 0
            else:
                duration = times[-1] - times[0]
            rate = len(times) / duration if duration > 0 else 0
            self.data[f"{topic}/send_rate"] = rate
            self.data_tmp[topic] = []  # reset the list of times

        return self.data
