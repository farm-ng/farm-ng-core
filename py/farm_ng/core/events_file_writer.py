import struct
from pathlib import Path
from typing import cast
from typing import IO
from typing import Optional
from typing import List
import time
from farm_ng.core.stamp import get_monotonic_now
from google.protobuf.message import Message

# pylint can't find Event or Uri in protobuf generated files
# https://github.com/protocolbuffers/protobuf/issues/10372
from farm_ng.core.event_pb2 import Event
from farm_ng.core.uri_pb2 import Uri
from farm_ng.core.timestamp_pb2 import Timestamp


class EventsFileWriter:
    def __init__(self, file_name: Path) -> None:
        self._file_name: Path = file_name.absolute()
        assert Path(self._file_name.parents[0]).is_dir()
        self._file_stream: Optional[IO] = None

    def __enter__(self):
        self.open()
        return self

    def __exit__(self, type, value, traceback):
        self.close()

    def __repr__(self) -> str:
        return (
            f"file_name: {str(self.file_name)}\n"
            f"file_stream: {self._file_stream}\nis_open: {self.is_open}"
        )

    @property
    def file_name(self) -> Path:
        return self._file_name

    def is_open(self) -> bool:
        return not self.is_closed()

    def is_closed(self) -> bool:
        if self._file_stream is None:
            return True
        return self._file_stream.closed

    def open(self) -> bool:
        self._file_stream = open(self._file_name, "wb")
        return self.is_open()

    def close(self) -> bool:
        if self.is_closed():
            return True
        file_stream = cast(IO, self._file_stream)
        file_stream.close()
        self._file_stream = None
        return self.is_closed()

    def make_write_stamp(self) -> Timestamp:
        return get_monotonic_now(semantics="events_file/write")

    def write(
        self, uri: Uri, message: Message, timestamps: Optional[List[Timestamp]] = None
    ) -> None:
        assert self.is_open(), ("Event log is not open:", self.file_name)
        if timestamps is None:
            timestamps = []
        timestamps.append(self.make_write_stamp())
        file_stream = cast(IO, self._file_stream)
        assert uri.scheme == message.DESCRIPTOR.full_name, (
            uri.scheme,
            message.DESCRIPTOR.full_name,
        )
        payload = message.SerializeToString()
        event = Event(
            uri=uri,
            timestamps=timestamps,
            payload_length=len(payload),
        ).SerializeToString()
        # note that < (little endian), I (4 bytes unsigned integer)
        event_len: bytes = struct.pack("<I", len(event))
        file_stream.write(event_len)
        file_stream.write(event)
        file_stream.write(payload)
