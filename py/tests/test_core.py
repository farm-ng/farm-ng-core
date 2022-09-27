from pathlib import Path

import pytest
from farm_ng.core import event_pb2
from farm_ng.core import uri_pb2
from farm_ng.core.events_file_reader import EventsFileReader
from farm_ng.core.events_file_writer import EventsFileWriter
from farm_ng.core.uri import Uri


@pytest.fixture
def writer(tmpdir) -> EventsFileWriter:
    file_name = Path(tmpdir) / "event.log"
    return EventsFileWriter(file_name)


@pytest.fixture
def reader(tmpdir) -> EventsFileReader:
    file_name = Path(tmpdir) / "event.log"
    return EventsFileReader(file_name)


class TestEventsWriter:
    def test_smoke(self, writer: EventsFileWriter) -> None:
        # empty object
        assert writer.is_closed()
        assert not writer.is_open()
        assert writer.file_name is None

    def test_open_close(self, writer: EventsFileWriter) -> None:
        # open the file
        assert writer.open()
        assert not writer.is_closed()
        assert writer.is_open()
        assert writer.file_name.name == "event.log"
        # close the file
        assert writer.close()
        assert writer.is_closed()
        assert not writer.is_open()
        assert writer.file_name is None

    def test_write_images(self, writer: EventsFileWriter) -> None:
        assert writer.open()
        # create a dummy message in the form of uri
        msg = uri_pb2.Uri(scheme="hello", authority="world")
        # encode the message type in the uri
        uri = Uri.from_message(msg)
        writer.write(msg, uri.proto)
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
        # create a dummy message in the form of uri
        msg = uri_pb2.Uri(scheme="hello", authority="world")
        # encode the message type in the uri
        uri = Uri.from_message(msg)
        writer.write(msg, uri.proto)
        assert writer.close()
        # read back the data
        assert reader.open()
        msg_out = reader.read()
        assert reader.close()
        assert msg == msg_out
