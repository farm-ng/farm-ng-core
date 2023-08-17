from __future__ import annotations

from pathlib import Path

import pytest
from farm_ng.core import event_pb2, timestamp_pb2, uri_pb2
from farm_ng.core.events_file_reader import (
    EventLogPosition,
    EventsFileReader,
    _parse_protobuf_descriptor,
    event_has_message,
    proto_from_json_file,
)
from farm_ng.core.events_file_writer import EventsFileWriter, proto_to_json_file
from farm_ng.core.stamp import get_monotonic_now


@pytest.fixture(name="log_base")
def fixture_log_base(tmpdir) -> Path:
    return Path(tmpdir) / "event"


@pytest.fixture(name="reader_log_file")
def fixture_reader_log_file(tmpdir) -> Path:
    return Path(tmpdir) / "event.0000.bin"


@pytest.fixture(name="log_dir")
def fixture_log_dir(tmpdir) -> Path:
    return Path(tmpdir)


def test_event_has_message() -> None:
    event = event_pb2.Event(
        uri=uri_pb2.Uri(
            scheme="protobuf",
            authority="farm_ng.core.proto.Timestamp",
            path="/farm_ng/core/timestamp.proto",
            query="type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto",
        ),
        timestamps=[
            timestamp_pb2.Timestamp(
                stamp=0.0,
                clock_name="test/monotonic",
                semantics="test/monotonic",
            ),
        ],
        payload_length=11,
    )
    assert event_has_message(event, timestamp_pb2.Timestamp)
    assert not event_has_message(event, event_pb2.Event)


class TestEventsWriter:
    def test_smoke(self, log_base: Path) -> None:
        with EventsFileWriter(file_base=log_base) as writer:
            assert writer.is_open()
            assert writer.file_name == log_base.with_suffix(".0000.bin")

    def test_ext(self, log_base: Path) -> None:
        with EventsFileWriter(file_base=log_base, extension=".log") as writer:
            assert writer.is_open()
            assert writer.file_name == log_base.with_suffix(".0000.log")

    def test_open_close(self, log_base: Path) -> None:
        # open the file
        writer = EventsFileWriter(file_base=log_base)
        assert "event.0000.bin" in str(writer.file_name)
        assert writer.file_length == 0
        assert writer.close()
        assert writer.is_closed()
        assert not writer.is_open()

        # open object
        assert writer.open()
        assert not writer.is_closed()
        assert writer.is_open()
        assert writer.close()

    def test_write_stamp(self, log_base: Path) -> None:
        writer = EventsFileWriter(file_base=log_base)
        assert writer.open()
        writer.write("test/uri", message=get_monotonic_now(semantics="test/monotonic"))
        writer.write("test/uri", message=get_monotonic_now(semantics="test/monotonic"))
        assert writer.file_length > 0
        assert writer.close()

    def test_write_rollover(self, log_base: Path, log_dir: Path) -> None:
        num_events: int = 5000
        max_mb: int = 1

        # Default - without rollover
        with EventsFileWriter(file_base=log_base) as writer:
            for i in range(num_events):
                time_stamp = timestamp_pb2.Timestamp(stamp=i)
                writer.write(path="hello", message=time_stamp)
                writer.write(path="world", message=time_stamp)

            assert writer.file_idx == 0
            assert writer.max_file_length == 0
            assert not (log_dir / "event.0001.bin").exists()

        # Specified - without rollover
        with EventsFileWriter(file_base=log_base, max_file_mb=-1) as writer:
            for i in range(num_events):
                time_stamp = timestamp_pb2.Timestamp(stamp=i)
                writer.write(path="hello", message=time_stamp)
                writer.write(path="world", message=time_stamp)

            assert writer.file_idx == 0
            assert writer.max_file_length == 0
            assert not (log_dir / "event.0001.bin").exists()

        # With rollover
        with EventsFileWriter(file_base=log_base, max_file_mb=max_mb) as writer:
            for i in range(num_events):
                time_stamp = timestamp_pb2.Timestamp(stamp=i)
                writer.write(path="hello", message=time_stamp)
                writer.write(path="world", message=time_stamp)

            assert writer.file_idx > 0
            assert writer.max_file_length == max_mb * 1e6
            assert (log_dir / "event.0001.bin").exists()


class TestEventsReader:
    def test_smoke(self, log_base: Path, reader_log_file: Path) -> None:
        with EventsFileWriter(file_base=log_base) as writer:
            writer.write(
                path="hello/world",
                message=get_monotonic_now(semantics="test/monotonic"),
            )
            writer.write(
                path="/leading/slash",
                message=get_monotonic_now(semantics="test/monotonic"),
            )
        with EventsFileReader(reader_log_file) as reader:
            assert reader.is_open()
            assert len(reader.events_index) == 0
            uris = sorted([*{x.event.uri.path for x in reader.get_index()}])
            # note lexographic ordering of paths.
            assert uris[0] == "/leading/slash"
            assert uris[1] == "hello/world"

    def test_open_close(self, log_base: Path, reader_log_file: Path) -> None:
        with EventsFileWriter(file_base=log_base) as writer:
            writer.write(
                path="hello/world",
                message=get_monotonic_now(semantics="test/monotonic"),
            )
        # empty object
        reader = EventsFileReader(reader_log_file)
        assert "event.0000.bin" in str(reader.file_name)
        assert reader.file_length == 0
        assert reader.close()
        assert reader.is_closed()
        assert not reader.is_open()
        assert len(reader.events_index) == 0

        # open object
        assert reader.open()
        assert reader.file_length > 0
        assert not reader.is_closed()
        assert reader.is_open()
        assert reader.close()

    def test_parse_proto_descriptor(self):
        uri = uri_pb2.Uri(
            scheme="protobuf",
            path="tik/tok",
            query="type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto",
        )
        name, package = _parse_protobuf_descriptor(uri)
        assert name == "Timestamp"
        assert package == "farm_ng.core.timestamp_pb2"

    def test_write_read(self, log_base: Path, reader_log_file: Path) -> None:
        num_events = 10
        with EventsFileWriter(file_base=log_base) as writer:
            for i in range(num_events):
                time_stamp = timestamp_pb2.Timestamp(stamp=i)
                writer.write(path="hello", message=time_stamp)
                writer.write(path="world", message=time_stamp)

        with EventsFileReader(reader_log_file) as reader:
            assert reader.is_open()
            count = 0
            for event, message in reader.read_messages():
                assert isinstance(message, timestamp_pb2.Timestamp)
                if event.uri.path == "hello":
                    assert message.stamp == count
                elif event.uri.path == "world":
                    assert message.stamp == count
                    count += 1

            # test get/has uris
            assert len(reader.events_index) == 0
            all_events: list[EventLogPosition] = reader.get_index()
            assert len(reader.events_index) > 0

            events: dict = {}

            for event_log in all_events:
                path: str = event_log.event.uri.path
                if path not in events:
                    events[path] = []
                events[path].append(event_log)

            for path, _ in events.items():
                for i, event_log in enumerate(events[path]):
                    _message = reader.read_message(event_log)
                    assert isinstance(_message, timestamp_pb2.Timestamp)
                    assert _message == event_log.read_message()
                    assert _message.stamp == i

        assert reader.close()


class TestEventsJson:
    def test_json_write_read_path(self, tmp_path: Path) -> None:
        stamp = timestamp_pb2.Timestamp(
            stamp=1.2,
            clock_name="clock0",
            semantics="test/proto",
        )

        # Test write w/ str, read w/ Path
        assert proto_to_json_file(tmp_path / "test.json", stamp)
        assert stamp == proto_from_json_file(
            tmp_path / "test.json",
            timestamp_pb2.Timestamp(),
        )

        # Test overwrite
        assert proto_to_json_file(tmp_path / "test.json", stamp)
        assert stamp == proto_from_json_file(
            tmp_path / "test.json",
            timestamp_pb2.Timestamp(),
        )

    def test_json_write_read_str(self, tmp_path: Path) -> None:
        stamp = timestamp_pb2.Timestamp(
            stamp=9.67832,
            clock_name="clock1",
            semantics="test/proto_again",
        )

        # Test write w/ Path, read w/ str
        assert proto_to_json_file(tmp_path / "test_1.json", stamp)
        assert stamp == proto_from_json_file(
            f"{tmp_path}/test_1.json",
            timestamp_pb2.Timestamp(),
        )

        # Test overwrite
        assert proto_to_json_file(tmp_path / "test_1.json", stamp)
        assert stamp == proto_from_json_file(
            f"{tmp_path}/test_1.json",
            timestamp_pb2.Timestamp(),
        )
