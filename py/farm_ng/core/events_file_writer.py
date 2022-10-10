import struct
from pathlib import Path
from typing import Any
from typing import cast
from typing import IO
from typing import Optional
from typing import List
from typing import Union
from farm_ng.core.stamp import get_monotonic_now
from farm_ng.core.uri import make_proto_uri
from google.protobuf.message import Message

# pylint can't find Event or Uri in protobuf generated files
# https://github.com/protocolbuffers/protobuf/issues/10372
from farm_ng.core.event_pb2 import Event
from farm_ng.core.uri_pb2 import Uri
from farm_ng.core.timestamp_pb2 import Timestamp


class EventsFileWriter:
    def __init__(self, file_name: Union[str, Path]) -> None:
        if isinstance(file_name, str):
            file_name = Path(file_name)
        self._file_name: Path = file_name.absolute()
        assert Path(self._file_name.parents[0]).is_dir()

        self._file_stream: Optional[IO] = None
        self._file_length: int = 0

    def __enter__(self) -> "EventsFileWriter":
        assert self.open()
        return self

    # pylint: disable=redefined-builtin
    def __exit__(self, type: Any, value: Any, traceback: Any) -> None:
        self.close()

    def __repr__(self) -> str:
        return (
            f"file_name: {str(self.file_name)} "
            + f"file_stream: {self._file_stream} "
            + f"is_open: {self.is_open()} "
        )

    @property
    def file_length(self) -> int:
        return self._file_length

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
        self._file_length = 0
        return self.is_open()

    def close(self) -> bool:
        if self.is_closed():
            return True
        file_stream = cast(IO, self._file_stream)
        file_stream.close()
        self._file_stream = None
        return self.is_closed()

    def make_write_stamp(self) -> Timestamp:
        return get_monotonic_now(semantics="log/write")

    def write_raw(
        self, uri: Uri, message: Message, timestamps: List[Timestamp]
    ) -> None:
        assert self.is_open(), f"Event log is not open: {self.file_name}"
        file_stream = cast(IO, self._file_stream)
        payload = message.SerializeToString()
        event = Event(
            uri=uri,
            timestamps=timestamps,
            payload_length=len(payload),
        ).SerializeToString()
        # note that < (little endian), I (4 bytes unsigned integer)
        event_len: bytes = struct.pack("<I", len(event))
        self._file_length += file_stream.write(event_len)
        self._file_length += file_stream.write(event)
        self._file_length += file_stream.write(payload)

    def write(
        self, path: str, message: Message, timestamps: Optional[List[Timestamp]] = None
    ) -> None:
        if timestamps is None:
            timestamps = []
        timestamps.append(self.make_write_stamp())
        uri = make_proto_uri(path=path, message=message)
        self.write_raw(uri=uri, message=message, timestamps=timestamps)
