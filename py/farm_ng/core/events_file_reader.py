import importlib
import sys
from collections import defaultdict
from pathlib import Path
from typing import Any, Tuple
from typing import cast
from typing import DefaultDict
from typing import IO
from typing import List
from typing import Optional

from farm_ng.core import event_pb2
from farm_ng.core.uri import Uri
from farm_ng.core import uri_pb2


class EventsFileReader:
    def __init__(self, file_name: Path) -> None:
        self._file_name = file_name.absolute()
        assert Path(self._file_name.parents[0]).is_dir()

        self._file_stream: Optional[IO] = None
        # assert self.open()
        # store the bytes offset in a dictionary where:
        # - the key is the string representation of the Uri of the message
        # - the value is a list of offsets to messages with that Uri
        self.offsets: DefaultDict[str, List[int]] = defaultdict(list)
        # self.compute_offsets()

    def __repr__(self) -> str:
        return (
            f"file_name: {str(self.file_name)}\n"
            f"file_stream: {self._file_stream}\nis_open: {self.is_open}"
        )

    def reset_offsets(self) -> None:
        self.offsets = defaultdict(list)

    def uris(self) -> List[Uri]:
        if self.offsets is None:
            return []
        return [Uri.from_string(key) for key in self.offsets.keys()]

    def has_uri(self, uri) -> bool:
        if self.offsets is None:
            return False
        return uri.string() in self.offsets.keys()

    def compute_offsets(self) -> None:
        if not self.is_open:
            raise Exception("Reader not open. Please, use reader.open()")

        maybe_file_stream = self._file_stream
        if maybe_file_stream is None:
            return None
        file_stream = cast(IO, maybe_file_stream)

        # clear, if any the previous offsets
        self.reset_offsets()

        current_offset: int = 0

        skip_bytes: int = 0

        # reade first the header
        header = self.header()
        assert header is not None

        current_offset += 1 + header.ByteSize()
        header_offset = current_offset

        while True:

            event_len = int.from_bytes(file_stream.read(1), sys.byteorder)
            # end file condition
            if event_len == 0:
                file_stream.seek(header_offset)
                break

            event: bytes = file_stream.read(event_len)

            event_proto = event_pb2.Event()
            event_proto.ParseFromString(event)

            uri = Uri(event_proto.uri)
            self.offsets[uri.string()].append(current_offset)

            # skip the main message decoding
            # message: bytes = self._file_stream.read(event_proto.length_next)
            skip_bytes = event_proto.length_next

            current_offset += 1 + event_len + skip_bytes

            file_stream.seek(current_offset)

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
        # can't always compute offsets?
        # self.compute_offsets()
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
        maybe_file_stream = self._file_stream
        if maybe_file_stream is None:
            return None
        file_stream = cast(IO, maybe_file_stream)
        file_stream.seek(self.offsets[uri.string()][frame_id])

    def seek_and_read(self, uri: Uri, frame_idx: int) -> Optional[Any]:
        self.seek(uri, frame_idx)
        return self.read()

    def read(self) -> Optional[Tuple[Any, uri_pb2.Uri]]:
        # Read the message's length as bytes and convert it to an integer
        maybe_file_stream = self._file_stream
        if maybe_file_stream is None:
            return None
        file_stream = cast(IO, maybe_file_stream)

        event_len = int.from_bytes(file_stream.read(1), sys.byteorder)
        if event_len == 0:
            self.close()
            return None

        # Read that number of bytes as the message bytes
        event: bytes = file_stream.read(event_len)

        event_proto = event_pb2.Event()
        event_proto.ParseFromString(event)

        # parse the message
        message_cls = getattr(
            importlib.import_module(event_proto.uri.scheme), event_proto.uri.authority
        )

        message: bytes = file_stream.read(event_proto.payload_length)

        message_out = message_cls()
        message_out.ParseFromString(message)

        return event_proto, message_out

    def header(self) -> event_pb2.EventHeader:
        maybe_file_stream = self._file_stream
        if maybe_file_stream is None:
            return event_pb2.EventHeader()
        file_stream = cast(IO, maybe_file_stream)
        file_stream.seek(0)
        header_len = int.from_bytes(file_stream.read(1), sys.byteorder)

        # Read that number of bytes as the message bytes
        header: bytes = file_stream.read(header_len)

        header_proto = event_pb2.EventHeader()
        header_proto.ParseFromString(header)

        return header_proto
