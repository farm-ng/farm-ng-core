from __future__ import annotations

__all__ = ["EventServiceHealthMetrics"]


class Singleton(type):
    _instances = {}  # noqa: RUF012

    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super().__call__(*args, **kwargs)
        return cls._instances[cls]


class EventServiceHealthMetrics(metaclass=Singleton):
    def __init__(self) -> None:
        self.service_counts: dict[str, int] = {}
        self.client_counts: dict[str, int] = {}
