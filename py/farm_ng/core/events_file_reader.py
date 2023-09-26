from __future__ import annotations

import argparse
import json
import struct
import sys
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path
from typing import IO, TYPE_CHECKING, Any, Generator, cast

from farm_ng.core.event_pb2 import Event
from farm_ng.core.uri import uri_query_to_dict, uri_to_protobuf_type
from google.protobuf import json_format

if TYPE_CHECKING:
    from google.protobuf.message import Message

# public symbols

__all__ = [
    "build_events_dict",
    "event_has_message",
    "EventsFileReader",
    "EventLogPosition",
    "proto_from_json_file",
    "payload_to_protobuf",
]


def proto_from_json_file(
    file_path: str | Path,
    empty_proto_message: Message,
) -> Message:
    """Load a proto Message from a JSON file.

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
        msg = f"Invalid file: {file_path!s}"
        raise FileNotFoundError(msg)

    with Path(file_path).open(encoding="utf-8") as file:
        json_pb = json.load(file)

    return json_format.ParseDict(json_pb, empty_proto_message)


def event_has_message(event: Event, message_type: type[Any]) -> bool:
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


def payload_to_protobuf(event: Event, payload: bytes) -> Message:
    """Return the protobuf message from an event and payload.

    Args:
        event: event_pb2.Event
        payload: bytes

    Returns:
        Message: the protobuf message
    """
    message_cls: type[Message] = uri_to_protobuf_type(event.uri)

    message: Message = message_cls()
    message.ParseFromString(payload)
    return message


def build_events_dict(
    events_index: list[EventLogPosition],
) -> dict[str, list[EventLogPosition]]:
    """Build a dictionary of lists of events, where the key is the path of the event.

    Args:
        events_index (list[EventLogPosition]): List of events from a log file

    Returns:
        dict[str, list[EventLogPosition]]: Dictionary of lists of events
    """
    events_dict: dict[str, list[EventLogPosition]] = defaultdict(list)
    for event_index in events_index:
        events_dict[f"/{event_index.event.uri.path}"].append(event_index)
    return events_dict


@dataclass
class EventLogPosition:
    """EventLogPosition is a dataclass that stores the event, position and reader."""

    def __init__(self, event: Event, pos: int, reader: EventsFileReader) -> None:
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

    EVENTS_LENGTH_BYTES: int = 4

    def __init__(self, file_name: str | Path) -> None:
        """Initialize EventsFileReader.

        Args:
            file_name: path to the file
        """
        if isinstance(file_name, str):
            file_name = Path(file_name)
        self._file_name: Path = file_name.absolute()
        if not self._file_name.is_file():
            msg = f"Invalid file: {self._file_name!s}"
            raise FileNotFoundError(msg)

        self._file_stream: IO | None = None
        self._file_length: int = 0

        # store index of events in a list to seek faster
        self._events_index: list[EventLogPosition] = []

    def __enter__(self) -> EventsFileReader:
        """Open the file and return the EventsFileReader instance."""
        success: bool = self.open()
        if not success:
            msg = "Failed to open file"
            raise OSError(msg)
        return self

    def __exit__(self, type: object, value: object, traceback: object) -> None:
        """Close the file."""
        self.close()

    def __repr__(self) -> str:
        """Return a string representation of the EventsFileReader."""
        return "\n".join(
            [
                f"file_name: {self.file_name!s}",
                f"file_stream: {self._file_stream}",
                f"is_open: {self.is_open()}",
            ],
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
        self._file_stream = Path(self._file_name).open("rb")
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
        buffer = file_stream.read(self.EVENTS_LENGTH_BYTES)
        if len(buffer) != self.EVENTS_LENGTH_BYTES:
            msg = f"Failed to read {self.EVENTS_LENGTH_BYTES} bytes"
            raise EOFError(msg)

        # read the event length, then the event
        event_len = struct.unpack("<I", buffer)[0]
        event_bytes: bytes = file_stream.read(event_len)
        if len(event_bytes) != event_len:
            raise EOFError
        event = Event()
        event.ParseFromString(event_bytes)
        query = uri_query_to_dict(event.uri)
        event.uri.path = query.get("service_name", "") + event.uri.path
        return EventLogPosition(event, file_stream.tell(), self)

    def _skip_next_message(self, event: Event) -> None:
        """Skip the next message in the file."""
        msg_bytes: int = event.payload_length
        file_stream = cast(IO, self._file_stream)
        file_stream.seek(msg_bytes, 1)

    def _build_events_index(self) -> None:
        """Build the index of events in the file."""
        if not self.is_open():
            msg = "Reader not open. Please, use reader.open()"
            raise OSError(msg)

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
            raise EOFError
        return payload_to_protobuf(event_log.event, payload)

    def read(self) -> tuple[Event, Message]:
        """Read the next event and message and return them.

        Returns:
            tuple[Event, Message]: the event and message
        """
        if not self.is_open():
            msg = "Reader not open. Please, use reader.open()"
            raise OSError(msg)

        event_log: EventLogPosition = self.read_next_event()
        return event_log.event, self.read_message(event_log)

    def read_messages(self) -> Generator[tuple[Event, Message], None, None]:
        """Read all the messages in the file and return them.

        Yields:
            Generator[tuple[Event, Message], None, None]: the event and message
        """
        if self._file_stream is None:
            msg = "Reader not open. Please, use reader.open()"
            raise OSError(msg)

        self._file_stream.seek(0)
        try:
            while True:
                event, message = self.read()
                yield event, message
        except EOFError:
            pass


def playback_command(args):
    with EventsFileReader(args.events_file) as reader:
        for event, _ in reader.read_messages():
            # Note you can find the originating service from uri query string
            query = uri_query_to_dict(event.uri)
            service_name = query.get("service_name", "")
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
