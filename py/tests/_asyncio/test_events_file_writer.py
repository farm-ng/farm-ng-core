from __future__ import annotations

import asyncio
import threading
from typing import TYPE_CHECKING

import pytest
from farm_ng.core.event_pb2 import Event
from farm_ng.core.events_file_reader import EventsFileReader
from farm_ng.core.events_file_writer import EventsFileWriter
from farm_ng.core.uri import make_proto_uri
from google.protobuf.wrappers_pb2 import Int32Value, StringValue

if TYPE_CHECKING:
    from pathlib import Path

pytestmark = pytest.mark.anyio


async def test_write_async(file_base: Path) -> None:
    with EventsFileWriter(file_base=file_base) as writer:
        await asyncio.gather(
            *(
                writer.write_async("test_path", Int32Value(value=value))
                for value in range(1, 11)
            ),
        )

    with EventsFileReader(file_name=file_base.with_suffix(".0000.bin")) as reader:
        messages = [event_log.read_message() for event_log in reader.get_index()]

    assert messages == [Int32Value(value=value) for value in range(1, 11)]


async def test_write_event_payload_async(file_base: Path) -> None:
    message = StringValue(value="test_payload")
    payload = message.SerializeToString()
    event = Event(
        uri=make_proto_uri(path="/test_path", message=message),
        payload_length=len(payload),
    )

    with EventsFileWriter(file_base=file_base) as writer:
        await writer.write_event_payload_async(event, payload)

    with EventsFileReader(file_name=file_base.with_suffix(".0000.bin")) as reader:
        event_log = reader.read_next_event()
        assert event_log.read_message() == message


async def test_write_async_does_not_block_event_loop(
    file_base: Path,
    monkeypatch: pytest.MonkeyPatch,
) -> None:
    write_started = threading.Event()
    allow_write = threading.Event()

    with EventsFileWriter(file_base=file_base) as writer:
        write = writer.write

        def blocking_write(*args, **kwargs) -> None:
            write_started.set()
            if not allow_write.wait(timeout=2):
                error_message = "test did not release the file write"
                raise TimeoutError(error_message)
            write(*args, **kwargs)

        monkeypatch.setattr(writer, "write", blocking_write)
        task = asyncio.create_task(
            writer.write_async("test_path", Int32Value(value=1)),
        )

        for _ in range(100):
            if write_started.is_set():
                break
            await asyncio.sleep(0.01)

        assert write_started.is_set()
        assert not task.done()
        allow_write.set()
        await task


async def test_cancellation_waits_for_active_write(
    file_base: Path,
    monkeypatch: pytest.MonkeyPatch,
) -> None:
    write_started = threading.Event()
    allow_write = threading.Event()

    with EventsFileWriter(file_base=file_base) as writer:
        write = writer.write

        def blocking_write(*args, **kwargs) -> None:
            write_started.set()
            if not allow_write.wait(timeout=2):
                error_message = "test did not release the file write"
                raise TimeoutError(error_message)
            write(*args, **kwargs)

        monkeypatch.setattr(writer, "write", blocking_write)
        task = asyncio.create_task(
            writer.write_async("test_path", Int32Value(value=1)),
        )

        for _ in range(100):
            if write_started.is_set():
                break
            await asyncio.sleep(0.01)

        assert write_started.is_set()
        task.cancel()
        await asyncio.sleep(0)
        assert not task.done()

        allow_write.set()
        with pytest.raises(asyncio.CancelledError):
            await task


@pytest.fixture()
def file_base(tmp_path: Path) -> Path:
    return tmp_path / "test_events_file"
