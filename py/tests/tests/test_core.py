from pathlib import Path

import pytest
from farm_ng.core import timestamp_pb2
from farm_ng.core.events_file_reader import EventsFileReader
from farm_ng.core.events_file_writer import EventsFileWriter
from farm_ng.core.stamp import get_monotonic_now


@pytest.fixture(name="log_file")
def fixture_writer(tmpdir) -> Path:
    return Path(tmpdir) / "event.log"


class TestEventsWriter:
    def test_smoke(self, log_file: Path) -> None:
        with EventsFileWriter(log_file) as writer:
            assert writer.is_open()
            assert writer.file_name == log_file

    def test_open_close(self, log_file: Path) -> None:
        # open the file
        writer = EventsFileWriter(log_file)
        assert writer.open()
        assert not writer.is_closed()
        assert writer.is_open()
        assert writer.file_name.name == "event.log"
        # close the file
        assert writer.close()
        assert writer.is_closed()
        assert not writer.is_open()
        assert writer.file_name == log_file

    def test_write_images(self, log_file: Path) -> None:
        writer = EventsFileWriter(log_file)
        assert writer.open()
        writer.write("test/uri", message=get_monotonic_now(semantics="test/monotonic"))
        assert writer.close()
        assert writer.file_length == 181, writer.file_length


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
        # em
        # empty object
        with EventsFileReader(log_file) as reader:
            assert reader.is_open()
            uris = reader.uris()
            print(uris)
            # note lexographic ordering of paths.
            assert uris[0].path == "/leading/slash"
            assert uris[1].path == "hello/world"

    def test_write_read(self, log_file: Path) -> None:
        with EventsFileWriter(log_file) as writer:
            for i in range(10):
                time_stamp = timestamp_pb2.Timestamp(stamp=i)
                writer.write(path="hello", message=time_stamp)
                writer.write(path="world", message=time_stamp)
            print(writer.file_length)
        # empty object
        with EventsFileReader(log_file) as reader:
            assert reader.is_open()
            count = 0
            for event, message in reader.read_messages():
                if event.uri.path == "hello":
                    assert message.stamp == count
                elif event.uri.path == "world":
                    assert message.stamp == count
                    count += 1

            uris = reader.uris()

            assert len(reader.uris()) == 2, uris
            assert uris[0].path == "hello"
            assert uris[1].path == "world"
            assert reader.num_frames(uris[0]) == 10
            assert reader.num_frames(uris[1]) == 10
            for frame_n in range(reader.num_frames(uris[0])):
                reader.seek(uri=uris[0], frame_id=frame_n)
                event, message = reader.read()
                assert message.stamp == frame_n

        assert reader.close()
