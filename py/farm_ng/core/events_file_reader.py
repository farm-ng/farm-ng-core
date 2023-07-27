from __future__ import annotations
import importlib
from dataclasses import dataclass
from pathlib import Path
from typing import Any
from typing import cast
from typing import IO
from typing import Generator
from typing import Type
import struct
import json

from farm_ng.core.uri import uri_query_to_dict
from farm_ng.core.event_pb2 import Event
from farm_ng.core.uri_pb2 import Uri
from google.protobuf.message import Message
from google.protobuf import json_format
import argparse

# public symbols

__all__ = [
    "event_has_message",
    "EventsFileReader",
    "EventLogPosition",
    "proto_from_json_file",
    "payload_to_protobuf",
    "event_to_protobuf_type",
]


def proto_from_json_file(
    file_path: str | Path, empty_proto_message: Message
) -> Message:
    """
    Load a proto Message from a JSON file.
    The ``empty_proto_message`` must be the type of the proto message to load.

    Args:
        file_path (str | Path): The path to the JSON file.
        empty_proto_message (Message): The empty proto message to parse into.

    Returns:
        A proto message of the type given in empty_proto_message read from the JSON file.
    """
    if isinstance(file_path, str):
        file_path = Path(file_path).absolute()

    if not file_path.is_file():
        raise FileNotFoundError(f"Invalid file: {str(file_path)}")

    with open(file_path, "r", encoding="utf-8") as file:
        json_pb = json.load(file)

    return json_format.ParseDict(json_pb, empty_proto_message)


def event_has_message(event: Event, message_type: Type[Any]) -> bool:
    """Check if event has a message of type msg_type.

    Args:
        event: event_pb2.Event
        msg_type: protobuf message type

    Returns:
        bool: True if event has a message of type msg_type
    """
    if not isinstance(event, Event):
        return False
    return event.uri.query.split("&")[0].split(".")[-1] == message_type.__name__


# parse a proto descriptor and extract the message name and package.
# See: https://developers.google.com/protocol-buffers/docs/reference/python-generated#invocation
# NOTE: the descriptor comes in the shape of:
# `type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto`


def _parse_protobuf_descriptor(uri: Uri) -> tuple[str, str]:
    """Parse a protobuf descriptor and extract the message name and package.

    Args:
        uri: uri_pb2.Uri

    Returns:
        Tuple[str, str]: message name and package
    """
    assert uri.scheme == "protobuf"
    type_split, pb_split = None, None
    for x in uri.query.split("&"):
        if x.startswith("type="):
            type_split = x.split("=")[-1]
        elif x.startswith("pb="):
            pb_split = x.split("=")[-1]
        # split by the key/value in the query

    assert type_split is not None, uri
    assert pb_split is not None, uri

    type_name = type_split.split(".")[-1]
    # parse the pb file location and convert to python module extension
    package_name = pb_split.replace(".proto", "_pb2").replace("/", ".")

    return type_name, package_name


def event_to_protobuf_type(event: Event) -> Type[Message]:
    """Return the protobuf type from an event.

    Args:
        event: event_pb2.Event

    Returns:
        Type[Message]: the protobuf type

    Example:
        >>> event = Event()
        >>> event.uri.query = "type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto"
        >>> event_to_protobuf_type(event)
        <class 'farm_ng.core.timestamp_pb2.Timestamp'>
    """
    name: str
    package: str
    name, package = _parse_protobuf_descriptor(event.uri)
    return getattr(importlib.import_module(package), name)


def payload_to_protobuf(event: Event, payload: bytes) -> Message:
    """Return the protobuf message from an event and payload.

    Args:
        event: event_pb2.Event
        payload: bytes

    Returns:
        Message: the protobuf message
    """
    message_cls: Type[Message] = event_to_protobuf_type(event)

    message: Message = message_cls()
    message.ParseFromString(payload)
    return message


@dataclass
class EventLogPosition:
    """EventLogPosition is a dataclass that stores the event, position and reader."""

    def __init__(self, event: Event, pos: int, reader: "EventsFileReader") -> None:
        """Initialize EventLogPosition.

        Args:
            event: the event described in event_pb2.Event
            pos: the position of the event in the file
            reader: the EventsFileReader instance
        """
        self.event = event
        self.pos = pos
        self.reader = reader

    def __repr__(self) -> str:
        """Return a string representation of the EventLogPosition."""
        return f"event: {self.event}\npos: {self.pos}\n"

    def read_message(self) -> Message:
        """Read the message from the event."""
        return self.reader.read_message(self)


class EventsFileReader:
    """EventsFileReader reads events from a file."""

    def __init__(self, file_name: str | Path) -> None:
        """Initialize EventsFileReader.

        Args:
            file_name: path to the file
        """
        if isinstance(file_name, str):
            file_name = Path(file_name)
        self._file_name: Path = file_name.absolute()
        assert self._file_name.exists(), self._file_name

        self._file_stream: IO | None = None
        self._file_length: int = 0

        # store index of events in a list to seek faster
        self._events_index: list[EventLogPosition] = []

    def __enter__(self) -> "EventsFileReader":
        """Open the file and return the EventsFileReader instance."""
        assert self.open()
        return self

    # pylint: disable=redefined-builtin
    def __exit__(self, type: Any, value: Any, traceback: Any) -> None:
        """Close the file."""
        self.close()

    def __repr__(self) -> str:
        """Return a string representation of the EventsFileReader."""
        return "\n".join(
            [
                f"file_name: {str(self.file_name)}",
                f"file_stream: {self._file_stream}",
                f"is_open: {self.is_open()}",
            ]
        )

    @property
    def file_length(self) -> int:
        """Return the length of the file."""
        return self._file_length

    @property
    def file_name(self) -> Path:
        """Return the path to the file."""
        return self._file_name

    @property
    def events_index(self) -> list[EventLogPosition]:
        """Return the index of events in the file."""
        return self._events_index

    def is_open(self) -> bool:
        """Return True if the file is open."""
        return not self.is_closed()

    def is_closed(self) -> bool:
        """Return True if the file is closed."""
        if self._file_stream is None:
            return True
        return self._file_stream.closed

    def open(self) -> bool:
        """Open the file and return True if successful.

        Returns:
            bool: True if successful
        """
        self._file_stream = open(self._file_name, "rb")
        self._file_length = self._file_name.stat().st_size
        return self.is_open()

    def close(self) -> bool:
        """Close the file and return True if successful.

        Returns:
            bool: True if successful
        """
        if self.is_closed():
            return True
        file_stream = cast(IO, self._file_stream)
        file_stream.close()
        self._file_stream = None
        return self.is_closed()

    def _reset_events_index(self) -> None:
        """Reset the events index."""
        self._events_index = []

    def read_next_event(self) -> EventLogPosition:
        """Read the next event and return it as an EventLogPosition.

        Returns:
            EventLogPosition: the event, position and reader
        """
        file_stream = cast(IO, self._file_stream)
        buffer = file_stream.read(4)
        if len(buffer) != 4:
            raise EOFError()

        # read the event length, then the event
        event_len = struct.unpack("<I", buffer)[0]
        event_bytes: bytes = file_stream.read(event_len)
        if len(event_bytes) != event_len:
            raise EOFError()
        event = Event()
        event.ParseFromString(event_bytes)

        return EventLogPosition(event, file_stream.tell(), self)

    def _skip_next_message(self, event: Event) -> None:
        """Skip the next message in the file."""
        msg_bytes: int = event.payload_length
        file_stream = cast(IO, self._file_stream)
        file_stream.seek(msg_bytes, 1)

    def _build_events_index(self) -> None:
        """Build the index of events in the file."""
        if not self.is_open():
            raise IOError("Reader not open. Please, use reader.open()")

        file_stream = cast(IO, self._file_stream)
        file_stream.seek(0)

        # clear, if any the previous offsets
        self._reset_events_index()
        while True:
            try:
                event_log: EventLogPosition = self.read_next_event()
                self._events_index.append(event_log)
                self._skip_next_message(event_log.event)
            except EOFError:
                break

        file_stream.seek(0)

    def get_index(self) -> list[EventLogPosition]:
        """Return the index of events in the file.

        Returns:
            list[EventLogPosition]: the index of events
        """
        if len(self._events_index) == 0:
            self._build_events_index()
        return self._events_index

    def read_message(self, event_log: EventLogPosition) -> Message:
        """Read the message from the event.

        Args:
            event_log (EventLogPosition): the event log
        """
        file_stream = cast(IO, self._file_stream)
        file_stream.seek(event_log.pos, 0)
        payload: bytes = file_stream.read(event_log.event.payload_length)
        if len(payload) != event_log.event.payload_length:
            raise EOFError()
        return payload_to_protobuf(event_log.event, payload)

    def read(self) -> tuple[Event, Message]:
        """Read the next event and message and return them.

        Returns:
            tuple[Event, Message]: the event and message
        """
        assert self.is_open()
        event_log: EventLogPosition = self.read_next_event()
        return event_log.event, self.read_message(event_log)

    def read_messages(self) -> Generator[tuple[Event, Message], None, None]:
        """Read all the messages in the file and return them.

        Yields:
            Generator[tuple[Event, Message], None, None]: the event and message
        """
        self._file_stream.seek(0)
        try:
            while True:
                event, message = self.read()
                yield event, message
        except EOFError:
            pass


def playback_command(args):
    with EventsFileReader(args.events_file) as reader:
        for event, message in reader.read_messages():
            # Note you can find the originating service from uri query string
            query = uri_query_to_dict(event.uri)
            service_name= query.get('service_name', '')
            print(service_name + event.uri.path, event.uri.query, event.payload_length)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    sub_parsers = parser.add_subparsers()
    playback_parser = sub_parsers.add_parser("playback")
    playback_parser.add_argument("events_file")

    playback_parser.set_defaults(func=playback_command)

    args = parser.parse_args()
    if hasattr(args, "func"):

        args.func(args)
    else:
        parser.print_help()
        sys.exit(1)
