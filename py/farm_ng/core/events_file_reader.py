import importlib
from importlib.resources import Package
import sys
from collections import defaultdict
from pathlib import Path
from typing import Any, Tuple
from typing import cast
from typing import DefaultDict
from typing import IO
from typing import List
from typing import Optional
import struct
import os
from farm_ng.core.event_pb2 import Event
from farm_ng.core.uri_pb2 import Uri
from farm_ng.core.uri import uri_to_string, string_to_uri


def parse_protobuf_descriptor(uri: Uri) -> Tuple[str, str]:
    assert uri.scheme == "protobuf"
    # parse a proto descriptor and extract the message name and package.
    # See: https://developers.google.com/protocol-buffers/docs/reference/python-generated#invocation
    # NOTE: the descriptor comes in the shape of `farm_ng.core.proto.Timestamp`
    type_split = uri.query.split("type=")
    assert len(type_split) == 2
    desc = type_split[1].split("&")[0]  # query string can have multiple key/value pairs
    desc_list = desc.split(".")
    name = desc_list[-1]
    package = desc_list[:-2] + [name.lower() + "_pb2"]
    return name, ".".join(package)


class EventsFileReader:
    def __init__(self, file_name: Path) -> None:
        self._file_name = file_name.absolute()
        assert Path(self._file_name.parents[0]).is_dir(), self._file_name
        assert self._file_name.exists(), self._file_name
        self._file_stream: Optional[IO] = None
        self._file_length: int = 0

        # assert self.open()
        # store the bytes offset in a dictionary where:
        # - the key is the string representation of the Uri of the message
        # - the value is a list of offsets to messages with that Uri
        self.offsets: DefaultDict[Uri, List[int]] = defaultdict(list)

    def __enter__(self):
        self.open()
        return self

    def __exit__(self, type, value, traceback):
        self.close()

    def __repr__(self) -> str:
        return (
            f"file_name: {str(self.file_name)} "
            + f"file_stream: {self._file_stream} "
            + f"is_open: {self.is_open()} "
            + f"file_length: {self.file_length}"
        )

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
        self._file_length = os.path.getsize(self._file_name)
        self._compute_offsets()
        return self.is_open()

    def close(self) -> bool:
        if self.is_closed():
            return True
        file_stream = cast(IO, self._file_stream)
        file_stream.close()
        self._file_stream = None
        return self.is_closed()

    def reset_offsets(self) -> None:
        self.offsets = defaultdict(list)

    def uris(self) -> List[Uri]:
        if self.offsets is None:
            return []
        return [string_to_uri(key) for key in self.offsets.keys()]

    def has_uri(self, uri: Uri) -> bool:
        if self.offsets is None:
            return False
        return uri_to_string(uri) in self.offsets.keys()

    def _read_next_event(self) -> Event:
        buffer = self._file_stream.read(4)
        if len(buffer) != 4:
            raise EOFError()

        event_len = struct.unpack("<I", buffer)[0]
        event_bytes: bytes = self._file_stream.read(event_len)
        event = Event()
        event.ParseFromString(event_bytes)
        return event

    def _skip_next_message(self, event: Event) -> None:
        msg_bytes: int = event.payload_length
        self._file_stream.seek(msg_bytes, 1)

    def _compute_offsets(self) -> None:
        if not self.is_open():
            raise Exception("Reader not open. Please, use reader.open()")

        file_stream = cast(IO, self._file_stream)
        file_stream.seek(0)

        # clear, if any the previous offsets
        self.reset_offsets()
        while True:
            current_offset = file_stream.tell()
            try:
                event = self._read_next_event()
                self.offsets[uri_to_string(event.uri)].append(current_offset)
                self._skip_next_message(event)
            except EOFError:
                break

        file_stream.seek(0)

    def num_frames(self, uri: Uri) -> int:
        if not self.offsets:
            self._compute_offsets()
        if not self.has_uri(uri):
            return 0
        return len(self.offsets[uri])

    def seek(self, uri: Uri, frame_id: int) -> None:
        assert uri in self.offsets.keys()
        assert frame_id < self.num_frames(uri)
        assert self.is_open()

        file_stream = cast(IO, self._file_stream)
        file_stream.seek(self.offsets[uri_to_string(uri)][frame_id])

    def read(self) -> Tuple[Event, Any]:
        assert self.is_open()
        event = self._read_next_event()

        name, package = parse_protobuf_descriptor(event.uri)
        message_cls = getattr(importlib.import_module(package), name)

        file_stream = cast(IO, self._file_stream)
        payload: bytes = file_stream.read(event.payload_length)

        message = message_cls()
        message.ParseFromString(payload)

        return event, message

    def read_messages(self):
        try:
            while True:
                event, message = self.read()
                yield event, message
        except EOFError:
            pass
