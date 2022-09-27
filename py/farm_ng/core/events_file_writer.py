import sys
from pathlib import Path
from typing import Any
from typing import BinaryIO
from typing import Optional

from farm_ng.core import event_pb2
from farm_ng.core import uri_pb2


class EventsFileWriter:
    def __init__(self) -> None:
        self._file_stream: Optional[BinaryIO] = None

    def __repr__(self) -> str:
        return f"file_name: {str(self.file_name)}\nfile_stream: {self._file_stream}\nis_open: {self.is_open}"

    @property
    def file_name(self) -> Optional[Path]:
        if self._file_stream is None:
            return None
        return Path(self._file_stream.name)

    def is_open(self) -> bool:
        return not self.is_closed()

    def is_closed(self) -> bool:
        if self._file_stream is None:
            return True
        return self._file_stream.closed

    def open(self, file_name: Path) -> bool:
        self._file_stream = open(file_name, "wb")
        return self.is_open()

    def close(self) -> bool:
        self._file_stream.close()
        self._file_stream = None
        return self.is_closed()

    def write(self, message: Any, uri: uri_pb2.Uri) -> None:

        # TODO: add timestamps
        event = event_pb2.Event(
            uri=uri,
            payload_length=message.ByteSize(),
        )

        event_len: bytes = event.ByteSize().to_bytes(1, sys.byteorder)

        self._file_stream.write(event_len)
        self._file_stream.write(event.SerializeToString())
        self._file_stream.write(message.SerializeToString())