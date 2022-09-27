from pathlib import Path

from farm_ng.core import uri_pb2
from farm_ng.core.events_file_reader import EventsFileReader
from farm_ng.core.events_file_writer import EventsFileWriter
from farm_ng.core.uri import Uri


class TestEventsWriter:
    def test_smoke(
        self,
    ) -> None:
        # empty object
        writer = EventsFileWriter()
        assert writer.is_closed()
        assert not writer.is_open()
        assert writer.file_name is None

    def test_open_close(self, tmpdir) -> None:
        file_name = Path(tmpdir) / "event.log"
        writer = EventsFileWriter()
        # open the file
        assert writer.open(file_name)
        assert not writer.is_closed()
        assert writer.is_open()
        assert writer.file_name.name == "event.log"  # type: ignore
        # close the file
        assert writer.close()
        assert writer.is_closed()
        assert not writer.is_open()
        assert writer.file_name is None

    def test_write(self, tmpdir) -> None:
        file_name = Path(tmpdir) / "event.log"
        writer = EventsFileWriter()
        assert writer.open(file_name)
        # create a dummy message in the form of uri
        msg = uri_pb2.Uri(scheme="hello", authority="world")
        # encode the message type in the uri
        uri = Uri.from_message(msg)
        writer.write(msg, uri.proto)
        assert writer.close()


class TestEventsReader:
    def test_smoke(self) -> None:
        # empty object
        reader = EventsFileReader()
        assert reader.is_closed()
        assert not reader.is_open()
        assert reader.file_name is None

    def test_open_close(self, tmpdir) -> None:
        file_name = Path(tmpdir) / "event.log"
        writer = EventsFileWriter()
        reader = EventsFileReader()
        # touch file
        assert writer.open(file_name)
        assert writer.close()
        # open the file
        assert reader.open(file_name)
        assert not reader.is_closed()
        assert reader.is_open()
        assert reader.file_name.name == "event.log"  # type: ignore
        # close the file
        assert reader.close()
        assert reader.is_closed()
        assert not reader.is_open()
        assert reader.file_name is None

    def test_write_read(self, tmpdir) -> None:
        file_name = Path(tmpdir) / "event.log"
        writer = EventsFileWriter()
        reader = EventsFileReader()
        # write file
        assert writer.open(file_name)
        # create a dummy message in the form of uri
        msg = uri_pb2.Uri(scheme="hello", authority="world")
        # encode the message type in the uri
        uri = Uri.from_message(msg)
        writer.write(msg, uri.proto)
        assert writer.close()
        # read back the data
        assert reader.open(file_name)
        msg_out = reader.read()
        assert reader.close()
        assert msg == msg_out
