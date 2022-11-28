from pathlib import Path

import pytest
from farm_ng.core.events_file_reader import (
    EventsFileReader,
    EventLogPosition,
    parse_protobuf_descriptor,
)
from farm_ng.core.events_file_writer import EventsFileWriter
from farm_ng.core.stamp import get_monotonic_now


@pytest.fixture(name="log_file")
def fixture_log_file(tmpdir) -> Path:
    return Path(tmpdir) / "event.log"


class TestEventsWriter:
    def test_smoke(self, log_file: Path) -> None:
        with EventsFileWriter(log_file) as writer:
            assert writer.is_open()
            assert writer.file_name == log_file

    def test_open_close(self, log_file: Path) -> None:
        # open the file
        writer = EventsFileWriter(log_file)
        assert "event.log" in str(writer.file_name)
        assert writer.file_length == 0
        assert writer.close()
        assert writer.is_closed()
        assert not writer.is_open()

        # open object
        assert writer.open()
        assert not writer.is_closed()
        assert writer.is_open()
        assert writer.close()

    def test_write_stamp(self, log_file: Path) -> None:
        writer = EventsFileWriter(log_file)
        assert writer.open()
        writer.write("test/uri", message=get_monotonic_now(semantics="test/monotonic"))
        writer.write("test/uri", message=get_monotonic_now(semantics="test/monotonic"))
        assert writer.file_length > 0
        assert writer.close()


class TestEventsReader:
    def test_smoke(self, log_file: Path) -> None:
        with EventsFileWriter(log_file) as writer:
            writer.write(
                path="hello/world",
                message=get_monotonic_now(semantics="test/monotonic"),
            )
            writer.write(
                path="/leading/slash",
                message=get_monotonic_now(semantics="test/monotonic"),
            )
        with EventsFileReader(log_file) as reader:
            assert reader.is_open()
            assert len(reader.event_index) == 0
            uris = reader.get_uris()
            # note lexographic ordering of paths.
            assert uris[0].path == "/leading/slash"
            assert uris[1].path == "hello/world"

    def test_open_close(self, log_file: Path) -> None:
        with EventsFileWriter(log_file) as writer:
            writer.write(
                path="hello/world",
                message=get_monotonic_now(semantics="test/monotonic"),
            )
        # empty object
        reader = EventsFileReader(log_file)
        assert "event.log" in str(reader.file_name)
        assert reader.file_length == 0
        assert reader.close()
        assert reader.is_closed()
        assert not reader.is_open()
        assert not reader.has_uri(uri_pb2.Uri())
        assert reader.num_events(uri_pb2.Uri()) == 0

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
        name, package = parse_protobuf_descriptor(uri)
        assert name == "Timestamp"
        assert package == "farm_ng.core.timestamp_pb2"

    def test_write_read(self, log_file: Path) -> None:
        num_events = 10
        with EventsFileWriter(log_file) as writer:
            for i in range(num_events):
                time_stamp = timestamp_pb2.Timestamp(stamp=i)
                writer.write(path="hello", message=time_stamp)
                writer.write(path="world", message=time_stamp)

        with EventsFileReader(log_file) as reader:
            assert reader.is_open()
            count = 0
            for event, message in reader.read_messages():
                if event.uri.path == "hello":
                    assert message.stamp == count
                elif event.uri.path == "world":
                    assert message.stamp == count
                    count += 1

            # test get/has uris
            assert len(reader.event_index) == 0
            uris: List[uri_pb2.Uri] = reader.get_uris()
            assert len(reader.event_index) > 0

            assert len(uris) == 2, uris
            assert uris[0].path == "hello"
            assert uris[1].path == "world"

            for uri in uris:
                assert reader.has_uri(uri)

                # test get/has events
                events: List[EventLogPosition] = reader.get_events(uri)
                assert len(events) == reader.num_events(uri) == num_events

                for i, event_log in enumerate(events):
                    message = reader.read_message(event_log)
                    assert message.stamp == i

        assert reader.close()
