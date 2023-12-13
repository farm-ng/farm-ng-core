from __future__ import annotations

from typing import TYPE_CHECKING

import pytest
from farm_ng.core.event_pb2 import Event
from farm_ng.core.events_file_reader import EventsFileReader, payload_to_protobuf
from farm_ng.core.events_file_writer import EventsFileWriter
from farm_ng.core.uri import make_proto_uri
from google.protobuf.wrappers_pb2 import Int32Value, StringValue

if TYPE_CHECKING:
    from pathlib import Path

    from farm_ng.core.uri_pb2 import Uri


@pytest.fixture()
def file_base(tmp_path: Path) -> Path:
    return tmp_path / "test_events_file"


@pytest.fixture()
def writer(file_base: Path):
    return EventsFileWriter(file_base=file_base)


@pytest.fixture()
def reader(file_base: Path, writer: EventsFileWriter):
    with writer as opened_writer:
        opened_writer.write("test_path", Int32Value(value=123))
    return EventsFileReader(file_name=file_base.with_suffix(".0000.bin"))


class TestEventsFileWriter:
    def test_open_and_close_file_writer(self, writer):
        with writer as opened_writer:
            assert opened_writer.is_open()
        assert writer.is_closed()

    def test_write_and_read_messages(self, writer, reader):
        event_count: int = 10
        with writer as opened_writer:
            # Start at 1 to avoid payload length of 0
            for i in range(1, 1 + event_count):
                opened_writer.write("test_path", Int32Value(value=i))

        counted_events: int = 0
        with reader as opened_reader:
            for event_log in opened_reader.get_index():
                event_message = event_log.read_message()
                assert event_message == Int32Value(value=1 + counted_events)
                assert event_log.event is not None
                assert event_log.event.uri.path == "test_path"
                assert event_log.event.payload_length == 2
                counted_events += 1
            assert counted_events == event_count

    def test_write_event_payload(self, writer, reader):
        with writer as opened_writer:
            message: StringValue = StringValue(value="test_payload")
            event_payload: bytes = message.SerializeToString()
            uri: Uri = make_proto_uri(
                path="/test_path",
                message=message,
                service_name="test_service",
            )
            event = Event(
                uri=uri,
                timestamps=[],
                payload_length=len(event_payload),
                sequence=0,
            )
            opened_writer.write_event_payload(event, event_payload)

        with reader as opened_reader:
            event_log = opened_reader.read_next_event()
            message = opened_reader.read_message(event_log)
            assert message == payload_to_protobuf(event, event_payload)

    def test_rollover_behavior(self, file_base: Path):
        max_file_bytes: int = 1000
        with EventsFileWriter(
            file_base=file_base,
            max_file_mb=max_file_bytes * 1e-6,
        ) as opened_writer:
            approx_expected_length: int = 0
            for i in range(100):
                message = StringValue(value=f"test_payload_{i}")
                payload: bytes = message.SerializeToString()
                uri: Uri = make_proto_uri(
                    path="/test_path",
                    message=message,
                    service_name="test_service",
                )
                event = Event(
                    uri=uri,
                    timestamps=[],
                    payload_length=len(payload),
                    sequence=i,
                )
                opened_writer.write_event_payload(event, payload)
                approx_expected_length += len(payload) + len(event.SerializeToString())

            # Check that the file was rolled over the expected number of times
            approx_expected_files: int = approx_expected_length // max_file_bytes
            actual_files: int = 1 + opened_writer.file_idx
            assert approx_expected_files > 5  # sufficient rollovers
            # The expected logging size is not a perfect estimate, so allow some wiggle room
            assert approx_expected_files == pytest.approx(actual_files, rel=0.5)

        for i in range(actual_files):
            file_name_bin = file_base.with_suffix(f".{i:04d}.bin")
            assert file_name_bin.exists()
            with EventsFileReader(file_name_bin) as opened_reader:
                if i != actual_files - 1:
                    # Check that the length is roughly the max file size
                    assert opened_reader.file_length == pytest.approx(
                        max_file_bytes,
                        rel=0.5,
                    )
                for event_log in opened_reader.get_index():
                    message = opened_reader.read_message(event_log)
                    assert isinstance(message, StringValue)
                    assert event_log.event.payload_length == len(
                        message.SerializeToString(),
                    )

    def test_headers(self, file_base: Path):
        # Build a list of test parameters
        header_uri_base: str = "baz_header"
        header_uri_service: str = "test_service"
        header_count: int = 10
        headers: list[tuple[Event, bytes]] = []
        approx_header_size: int = 0
        max_file_bytes: int = 10000

        # Create the header messages
        for i in range(header_count):
            message = Int32Value(value=i)
            event_payload: bytes = message.SerializeToString()
            uri: Uri = make_proto_uri(
                path=f"/{header_uri_base}_{i}",
                message=message,
                service_name=header_uri_service,
            )
            event = Event(
                uri=uri,
                timestamps=[],
                payload_length=len(event_payload),
                sequence=i,
            )
            headers.append((event, event_payload))
            approx_header_size += len(event.SerializeToString()) + len(event_payload)

            # Add some extra headers with duplicate URI's to ensure they are filtered out
            dup_message = Int32Value(value=999 + i)
            dup_payload: bytes = dup_message.SerializeToString()
            dup_event = Event(
                uri=uri,
                timestamps=[],
                payload_length=len(dup_payload),
                sequence=i + header_count,
            )
            headers.append((dup_event, dup_payload))

        # Test that headers are written to the file
        assert max_file_bytes > approx_header_size * 1.5  # sufficient space for headers
        with EventsFileWriter(
            file_base=file_base,
            max_file_mb=max_file_bytes * 1e-6,
            header_msgs=headers,
        ) as opened_writer:
            assert opened_writer.file_idx == 0
            assert opened_writer.file_length == pytest.approx(
                approx_header_size,
                rel=0.5,
            )

        # Test that headers cannot exceed the max file size
        with pytest.raises(RuntimeError):
            with EventsFileWriter(
                file_base=file_base,
                max_file_mb=(approx_header_size / 2) * 1e-6,
                header_msgs=headers,
            ) as opened_writer:
                pass

        # Test that headers are added every rollover
        file_count: int = 0
        with EventsFileWriter(
            file_base=file_base,
            max_file_mb=max_file_bytes * 1e-6,
            header_msgs=headers,
        ) as opened_writer:
            assert opened_writer.file_idx == 0
            assert opened_writer.file_length == pytest.approx(
                approx_header_size,
                rel=0.5,
            )
            for i in range(1000):
                opened_writer.write("test_path", StringValue(value=f"test_payload_{i}"))
            file_count = 1 + opened_writer.file_idx
            assert (
                file_count > 5
            )  # If this fails, increase for loop size to trigger enough rollovers

        for i in range(file_count):
            header_count_in_file: int = 0
            message_count_in_file: int = 0
            file_name_bin = file_base.with_suffix(f".{i:04d}.bin")
            assert file_name_bin.exists()
            with EventsFileReader(file_name_bin) as opened_reader:
                for event_log in opened_reader.get_index():
                    message = opened_reader.read_message(event_log)
                    if isinstance(message, StringValue):
                        message_count_in_file += 1
                    elif isinstance(message, Int32Value):
                        # Test that the duplicate header was filtered out
                        # And that the headers are in order
                        assert message.value == header_count_in_file
                        header_count_in_file += 1
                    else:
                        msg = f"Unexpected message type: {type(message)}"
                        raise TypeError(msg)

                assert header_count_in_file == header_count
                if i == file_count - 1:
                    assert message_count_in_file > 0
                else:
                    assert message_count_in_file > 10
                    # Check that the length is roughly the max file size
                    assert opened_reader.file_length == pytest.approx(
                        max_file_bytes,
                        rel=0.5,
                    )
