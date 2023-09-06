from __future__ import annotations

import asyncio
import time
from collections import defaultdict
from typing import TYPE_CHECKING, Callable

from farm_ng.core import stamp as stamp_module
from farm_ng.core.event_client import make_event_and_payload
from farm_ng.core.event_service_metrics import Singleton
from farm_ng.core.events_file_reader import payload_to_protobuf
from farm_ng.core.stamp import StampSemantics
from farm_ng.core.timestamp_pb2 import Timestamp

if TYPE_CHECKING:
    from farm_ng.core.event_pb2 import Event
    from farm_ng.core.event_service_pb2 import RequestReplyReply
    from google.protobuf.message import Message


class MockEventClient:
    def __init__(self, req_rep_handler):
        self._req_rep_handler = req_rep_handler

    async def request_reply(
        self,
        path: str,
        message: Message,
        timestamps: list[Timestamp] | None = None,
    ) -> RequestReplyReply:
        return await self._req_rep_handler(path, message, timestamps)


class MockEventClientReceiver:
    def __init__(self):
        self._message_queue: asyncio.Queue | None = None

    async def send(self, event: Event, payload: bytes):
        if self._message_queue is not None:
            await self._message_queue.put((event, payload))

    async def receive(self, decode=True):
        self._message_queue = asyncio.Queue()
        while True:
            event, payload = await self._message_queue.get()
            if decode:
                yield event, payload_to_protobuf(event, payload=payload)
            else:
                yield event, payload


class MockEventService:
    def __init__(self, service_name: str, receivers: list[MockEventClientReceiver]):
        self._sequence_tracker: dict[str, int] = defaultdict(lambda: 0)
        self._service_name: str = service_name
        self._receivers = receivers
        self._request_reply_handler: Callable | None = None

    @property
    def request_reply_handler(self) -> Callable | None:
        """Returns the request/reply handler."""
        return self._request_reply_handler

    @request_reply_handler.setter
    def request_reply_handler(self, handler: Callable) -> None:
        """Sets the request/reply handler."""
        self._request_reply_handler = handler

    async def publish(self, path, message: Message):
        timestamps = []
        timestamps.append(
            SimTime().get_monotonic_now(semantics=StampSemantics.SERVICE_SEND),
        )
        timestamps.append(
            SimTime().get_system_clock_now(semantics=StampSemantics.SERVICE_SEND),
        )
        seq = self._sequence_tracker[path]
        self._sequence_tracker[path] = seq + 1
        event, payload = make_event_and_payload(
            path,
            message,
            self._service_name,
            seq,
            timestamps,
        )
        for receiver in self._receivers:
            await receiver.send(event, payload)


class SimTime(metaclass=Singleton):
    def __init__(self):
        self._monotonic = 0
        self._system = time.time()
        self._increment_event = asyncio.Event()
        self._timeslice_lock = asyncio.Lock()

    @property
    def lock(self):
        return self._timeslice_lock

    async def tick(self, dt):
        async with self._timeslice_lock:
            self._monotonic += dt
            self._system += dt
            self._increment_event.set()
            self._increment_event.clear()
        await asyncio.sleep(0)

    def monotonic(self):
        return self._monotonic

    def time(self):
        return self._system

    def localtime(self):
        return time.localtime(self._system)

    async def sleep_until(self, deadline) -> asyncio.Lock:
        while self._monotonic < deadline:
            await self._increment_event.wait()

        return self._timeslice_lock

    def get_monotonic_now(self, semantics: str) -> Timestamp:
        return Timestamp(
            stamp=self.monotonic(),
            clock_name=stamp_module.get_host_name() + "/monotonic",
            semantics=semantics,
        )

    def get_system_clock_now(self, semantics: str) -> Timestamp:
        return Timestamp(
            stamp=self.time(),
            clock_name=stamp_module.get_host_name()
            + f"/system_clock/{self.localtime().tm_zone}",
            semantics=semantics,
        )
