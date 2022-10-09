from pathlib import Path

import pytest
from farm_ng.core import uri_pb2
from farm_ng.core import timestamp_pb2
from farm_ng.core.events_file_reader import EventsFileReader
from farm_ng.core.events_file_writer import EventsFileWriter


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

    def test_write_images(self, writer: EventsFileWriter) -> None:
        assert writer.open()
        uri = uri_pb2.Uri()
        writer.write(uri)
        assert writer.close()


class TestEventsReader:
    def test_smoke(self, reader: EventsFileReader) -> None:
        # empty object
        assert reader.is_closed()
        assert not reader.is_open()
        assert reader.file_name is None

    def test_open_close(
        self, writer: EventsFileWriter, reader: EventsFileReader
    ) -> None:
        # touch file
        assert writer.open()
        assert writer.close()
        # open the file
        assert reader.open()
        assert not reader.is_closed()
        assert reader.is_open()
        assert reader.file_name.name == "event.log"
        # close the file
        assert reader.close()
        assert reader.is_closed()
        assert not reader.is_open()
        assert reader.file_name is None

    def test_write_read(
        self, writer: EventsFileWriter, reader: EventsFileReader
    ) -> None:
        # write file
        assert writer.open()
        for i in range(1, 10):
            time_stamp = timestamp_pb2.Timestamp(stamp=i)
            writer.write(time_stamp)
        assert writer.close()
        # read back the data
        assert reader.open()
        count = 1
        while True:
            res = reader.read()
            if res is None:
                break
            _, msg = res
            assert msg.stamp == count
            count += 1

        assert reader.close()
