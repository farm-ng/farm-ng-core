import importlib
import sys
from collections import defaultdict
from pathlib import Path
from typing import Any
from typing import BinaryIO
from typing import Dict
from typing import List
from typing import Optional

from farm_ng.core import event_pb2
from farm_ng.core.uri import Uri


class EventsFileReader:
    def __init__(self, file_name: Path) -> None:
        self._file_name = file_name.absolute()
        assert Path(self._file_name.parents[0]).is_dir()

        self._file_stream: Optional[BinaryIO] = None
        # assert self.open()
        # store the bytes offset in a dictionary where:
        # - the key is the string representation of the Uri of the message
        # - the value is a list of offsets to messages with that Uri
        self.offsets: Optional[Dict[str, List[int]]] = None
        # self.compute_offsets()

    def __repr__(self) -> str:
        return f"file_name: {str(self.file_name)}\nfile_stream: {self._file_stream}\nis_open: {self.is_open}"

    def reset_offsets(self) -> None:
        self.offsets = defaultdict(list)

    def uris(self) -> List[Uri]:
        return [Uri.from_string(key) for key in self.offsets.keys()]

    def has_uri(self, uri) -> bool:
        return uri.string() in self.offsets.keys()

    # TODO: discuss the api signature
    def compute_offsets(self) -> Dict[Uri, List[int]]:
        if not self.is_open:
            raise Exception("Reader not open. Please, use reader.open()")

        # clear, if any the previous offsets
        self.reset_offsets()

        current_offset: int = 0

        skip_bytes: int = 0

        # reade first the header
        header = self.header()
        assert header is not None

        current_offset += 1 + header.ByteSize()

        while True:

            event_len = int.from_bytes(self._file_stream.read(1), sys.byteorder)
            # end file condition
            if event_len == 0:
                self._file_stream.seek(0)
                break

            event: bytes = self._file_stream.read(event_len)

            event_proto = event_pb2.Event()
            event_proto.ParseFromString(event)

            uri = Uri(event_proto.uri)
            self.offsets[uri.string()].append(current_offset)

            # skip the main message decoding
            # message: bytes = self._file_stream.read(event_proto.length_next)
            skip_bytes = event_proto.length_next

            current_offset += 1 + event_len + skip_bytes

            self._file_stream.seek(current_offset)

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

    def open(self) -> bool:
        self._file_stream = open(self._file_name, "rb")
        return self.is_open()

    def close(self) -> bool:
        if self._file_stream is not None:
            self._file_stream.close()
            self._file_stream = None
        return self.is_closed()

    def num_frames(self, uri: Uri) -> int:
        if uri.string() not in self.offsets.keys():
            return -1
        return len(self.offsets[uri.string()])

    def seek(self, uri: Uri, frame_id: int) -> None:
        assert uri.string() in self.offsets.keys()
        assert frame_id < self.num_frames(uri)
        self._file_stream.seek(self.offsets[uri.string()][frame_id])

    def seek_and_read(self, uri: Uri, frame_idx: int) -> Optional[Any]:
        self.seek(uri, frame_idx)
        return self.read()

    def read(self) -> Optional[Any]:
        # Read the message's length as bytes and convert it to an integer
        event_len = int.from_bytes(self._file_stream.read(1), sys.byteorder)
        if event_len == 0:
            self.close()
            return None

        # Read that number of bytes as the message bytes
        event: bytes = self._file_stream.read(event_len)

        event_proto = event_pb2.Event()
        event_proto.ParseFromString(event)

        # parse the message
        message_cls = getattr(importlib.import_module(event_proto.module), event_proto.name)

        message: bytes = self._file_stream.read(event_proto.length_next)

        message_out = message_cls()
        message_out.ParseFromString(message)

        return message_out

    def header(self) -> event_pb2.EventHeader:
        self._file_stream.seek(0)
        header_len = int.from_bytes(self._file_stream.read(1), sys.byteorder)

        # Read that number of bytes as the message bytes
        header: bytes = self._file_stream.read(header_len)

        header_proto = event_pb2.EventHeader()
        header_proto.ParseFromString(header)

        return header_proto
