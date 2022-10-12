import importlib
from collections import defaultdict
from pathlib import Path
from typing import Any
from typing import Tuple
from typing import cast
from typing import DefaultDict
from typing import IO
from typing import List
from typing import Optional
from typing import Union
import struct
from farm_ng.core.event_pb2 import Event
from farm_ng.core.uri_pb2 import Uri
from farm_ng.core.uri import uri_to_string, string_to_uri
from google.protobuf.message import Message


# parse a proto descriptor and extract the message name and package.
# See: https://developers.google.com/protocol-buffers/docs/reference/python-generated#invocation
# NOTE: the descriptor comes in the shape of:
# `type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto`


def parse_protobuf_descriptor(uri: Uri) -> Tuple[str, str]:
    assert uri.scheme == "protobuf"
    # split by the key/value in the query
    type_split, pb_split = uri.query.split("&")
    # parse the type and keep the class name
    assert "type=" in type_split, type_split
    type_name = type_split.split("type=")[-1].split(".")[-1]
    # parse the pb file location and convert to python module extension
    assert "pb=" in pb_split, pb_split
    package_name = pb_split.split("pb=")[-1].replace(".proto", "_pb2").replace("/", ".")
    return type_name, package_name


class EventsFileReader:
    def __init__(self, file_name: Union[str, Path]) -> None:
        if isinstance(file_name, str):
            file_name = Path(file_name)
        self._file_name: Path = file_name.absolute()
        assert self._file_name.exists(), self._file_name

        self._file_stream: Optional[IO] = None
        self._file_length: int = 0

        # store the bytes offset in a dictionary where:
        # - the key is the string representation of the Uri of the message
        # - the value is a list with tuples of Event and its offsets to messages with that Uri
        self._event_index: DefaultDict[str, List[Tuple[Event, int]]] = defaultdict(list)

    def __enter__(self) -> "EventsFileReader":
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
        self._file_stream = open(self._file_name, "rb")
        self._file_length = self._file_name.stat().st_size
        return self.is_open()

    def close(self) -> bool:
        if self.is_closed():
            return True
        file_stream = cast(IO, self._file_stream)
        file_stream.close()
        self._file_stream = None
        return self.is_closed()

    def _reset_event_index(self) -> None:
        self._event_index = defaultdict(list)

    def uris(self) -> List[Uri]:
        if not self._event_index:
            self._build_event_index()
        return [string_to_uri(key) for key in sorted(self._event_index.keys())]

    def has_uri(self, uri: Uri) -> bool:
        if self._event_index is None:
            return False
        return uri_to_string(uri) in self._event_index.keys()

    def _read_next_event(self) -> Event:
        file_stream = cast(IO, self._file_stream)
        buffer = file_stream.read(4)
        if len(buffer) != 4:
            raise EOFError()

        event_len = struct.unpack("<I", buffer)[0]
        event_bytes: bytes = file_stream.read(event_len)
        event = Event()
        event.ParseFromString(event_bytes)
        return event

    def _skip_next_message(self, event: Event) -> None:
        msg_bytes: int = event.payload_length
        file_stream = cast(IO, self._file_stream)
        file_stream.seek(msg_bytes, 1)

    def _build_event_index(self) -> None:
        if not self.is_open():
            raise Exception("Reader not open. Please, use reader.open()")

        file_stream = cast(IO, self._file_stream)
        file_stream.seek(0)

        # clear, if any the previous offsets
        self._reset_event_index()
        while True:
            try:
                event = self._read_next_event()
                current_offset = file_stream.tell()
                self._event_index[uri_to_string(event.uri)].append(
                    (event, current_offset)
                )
                self._skip_next_message(event)
            except EOFError:
                break

        file_stream.seek(0)

    def events(self, uri: Uri) -> List[Tuple[Event, int]]:
        if not self._event_index:
            self._build_event_index()
        return self._event_index[uri_to_string(uri)]

    def num_events(self, uri: Uri) -> int:
        if not self.has_uri(uri):
            return 0
        return len(self.events(uri))

    def get_event(self, uri: Uri, frame_id: int) -> Tuple[Event, int]:
        assert self.has_uri(uri)
        assert frame_id < self.num_events(uri)
        return self.events(uri)[frame_id]

    def read_message(self, event: Event, offset: Optional[int] = None) -> Message:
        file_stream = cast(IO, self._file_stream)
        if offset is not None:
            file_stream.seek(offset, 0)
        name, package = parse_protobuf_descriptor(event.uri)
        message_cls = getattr(importlib.import_module(package), name)

        payload: bytes = file_stream.read(event.payload_length)

        message: Message = message_cls()
        message.ParseFromString(payload)
        return message

    def read(self) -> Tuple[Event, Message]:
        assert self.is_open()
        event = self._read_next_event()
        return event, self.read_message(event)

    def read_messages(self):
        self._file_stream.seek(0)
        try:
            while True:
                event, message = self.read()
                yield event, message
        except EOFError:
            pass
