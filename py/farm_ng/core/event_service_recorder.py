"""EventServiceRecorder and RecorderService.

Run the event_service which starts a simple test publisher:
$ python -m farm_ng.core.event_service

Run the event_recorder which subscribes to the test publisher and logs the events to a file:
$ python -m farm_ng.core.event_service_recorder record --service-config=config.json --config-name=record_all foo

NOTE: The config file has an EventServiceConfig with name "record_all" which subscribes to the test publisher

Play back the log file:
$ python -m farm_ng.core.events_file_reader playback foo.0000.bin

Try running the event_service_recorder in service mode.

Start the service:
$ python -m farm_ng.core.event_service_recorder service --service-config config.json --service-name recorder

In a separate terminal, start recording:
$ python -m farm_ng.core.event_service_recorder start --service-config config.json  --service-name recorder

Then stop recording.
$ python -m farm_ng.core.event_service_recorder stop --service-config config.json  --service-name recorder
"""
from __future__ import annotations

import argparse
import asyncio
import logging
import sys
from collections import deque
from datetime import datetime, timezone
from pathlib import Path
from typing import TYPE_CHECKING

import grpc
from farm_ng.core import event_pb2
from farm_ng.core.event_client import EventClient
from farm_ng.core.event_service import (
    EventServiceGrpc,
    add_service_parser,
    load_service_config,
)
from farm_ng.core.event_service_pb2 import (
    EventServiceConfig,
    EventServiceConfigList,
    RequestReplyRequest,
    SubscribeRequest,
)
from farm_ng.core.events_file_reader import payload_to_protobuf, proto_from_json_file
from farm_ng.core.events_file_writer import EventsFileWriter
from farm_ng.core.stamp import StampSemantics, get_monotonic_now
from farm_ng.core.uri import get_host_name, uri_query_to_dict, uri_to_string
from google.protobuf.empty_pb2 import Empty
from google.protobuf.wrappers_pb2 import StringValue

if TYPE_CHECKING:
    from google.protobuf.message import Message

__all__ = ["EventServiceRecorder", "RecorderService"]


class EventServiceRecorder:
    """Service that subscribes to a list of services and records the events to a file."""

    # the maximum size of the queue
    QUEUE_MAX_SIZE: int = 50

    def __init__(
        self,
        service_name: str,
        config_list: EventServiceConfigList,
        header_events: list[tuple[event_pb2.Event, bytes]] | None = None,
    ) -> None:
        """Initializes the service.

        Args:
            service_name (str): the name of the service.
            config_list (EventServiceConfigList): the configuration data structure.
            header_events (list[tuple[event_pb2.Event, bytes]], optional): the initial list header events.
        """
        self.service_name: str = service_name
        self.config_list: EventServiceConfigList = config_list
        self.recorder_config: EventServiceConfig | None = None

        # the service logger
        self._logger: logging.Logger = logging.getLogger(service_name)

        # map of service name to EventClient
        self._clients: dict[str, EventClient] = {}

        # find the recorder config and create the clients
        # Here we reuse a config for the subscriptions, from the recorder_config
        # and create the clients for the other services (with port names)
        config: EventServiceConfig
        for config in self.config_list.configs:
            if self.service_name == config.name:
                self.recorder_config = config
            elif config.port != 0:
                self._clients[config.name] = EventClient(config)

        if self.recorder_config is None:
            msg = f"service {self.service_name} not found in config list"
            raise ValueError(msg)

        # the queue to store the events
        self.record_queue: asyncio.Queue[tuple[event_pb2.Event, bytes]] = asyncio.Queue(
            maxsize=self.QUEUE_MAX_SIZE,
        )
        self.header_deque: deque[tuple[event_pb2.Event, bytes]] = deque()
        if header_events is not None:
            for event, payload in header_events:
                self.add_header_event(event, payload)

    def add_header_event(self, event: event_pb2.Event, payload: bytes) -> None:
        """Add a header event to the header_deque.

        Args:
            event: Event to add.
            payload: Payload to add.
        """
        if not isinstance(event, event_pb2.Event):
            error_msg = f"header event must be Event, not {type(event)}"
            raise TypeError(error_msg)
        if not isinstance(payload, bytes):
            error_msg = f"header payload must be bytes, not {type(payload)}"
            raise TypeError(error_msg)
        self.header_deque.append((event, payload))

    @property
    def logger(self) -> logging.Logger:
        """Returns the service logger."""
        return self._logger

    @property
    def clients(self) -> dict[str, EventClient]:
        """Returns the map of service name to EventClient."""
        return self._clients

    async def record(
        self,
        file_base: str | Path,
        extension: str = ".bin",
        max_file_mb: int = 0,
    ) -> None:
        """Records the events that arrive in the queue to a file.

        Args:
            file_base (str | Path): the base name of the file.
            extension (str, optional): the extension of the file. Defaults to ".bin".
            max_file_mb (int, optional): the maximum size of the file in MB. Defaults to 0.
        """
        with EventsFileWriter(
            file_base,
            extension,
            max_file_mb,
            header_events=list(self.header_deque),
        ) as writer:
            self.header_deque.clear()
            event: event_pb2.Event
            payload: bytes
            while True:
                # Add any header events added during recording
                while self.header_deque:
                    event, payload = self.header_deque.popleft()
                    writer.add_header_event(event, payload, write=True)
                # await a new event and payload, and write it to the file
                event, payload = await self.record_queue.get()
                event.timestamps.append(
                    get_monotonic_now(semantics=StampSemantics.FILE_WRITE),
                )
                writer.write_event_payload(event, payload)

    async def subscribe(
        self,
        client: EventClient,
        subscription: SubscribeRequest,
    ) -> None:
        """Subscribes to a service and puts the events in the queue.

        Args:
            client (EventClient): the client to subscribe to.
            subscription (SubscribeRequest): the subscription request.
        """
        event: event_pb2.Event
        payload: bytes
        async for event, payload in client.subscribe(subscription, decode=False):
            if event.uri.path.startswith("/header"):
                # Handle header events - typically these will be metadata or calibrations
                self.add_header_event(event, payload)
            try:
                self.record_queue.put_nowait((event, payload))
            except asyncio.QueueFull:
                self.logger.warning("Queue full, dropping event %s", event)
                continue

    async def subscribe_and_record(
        self,
        file_base: str | Path,
        extension: str = ".bin",
        max_file_mb: int = 0,
    ) -> None:
        """Subscribes to a list of services and records the events to a file.

        Args:
            file_base (str | Path): the base name of the file.
            extension (str, optional): the extension of the file. Defaults to ".bin".
            max_file_mb (int, optional): the maximum size of the file in MB. Defaults to 0.
        """
        if self.recorder_config is None:
            msg = "recorder_config is None"
            raise ValueError(msg)

        # create the tasks list. the first task is the record task
        async_tasks: list[asyncio.Task] = [
            asyncio.create_task(
                self.record(
                    file_base=file_base,
                    extension=extension,
                    max_file_mb=max_file_mb,
                ),
            ),
        ]

        # add the subscribe tasks based on the subscriptions in the recorder config
        subscription: SubscribeRequest
        for subscription in self.recorder_config.subscriptions:
            query: dict[str, str] = uri_query_to_dict(uri=subscription.uri)
            query_service_name: str = query["service_name"]
            if query_service_name not in self.clients:
                self.logger.warning("Invalid subscription: %s", query_service_name)
                continue
            client: EventClient = self.clients[query_service_name]
            async_tasks.append(
                asyncio.create_task(self.subscribe(client, subscription)),
            )
        try:
            await asyncio.gather(*async_tasks)
        except asyncio.CancelledError:
            self.logger.debug("cancelled recording")
        finally:
            self.logger.info("done recording")


# the format of the file name
# Example: 2021_08_31_15_54_00_000000
DATETIME_FORMAT: str = "%Y_%m_%d_%H_%M_%S_%f"

# the default timezone
DEFAULT_TIMEZONE = timezone.utc


def get_file_name_base() -> str:
    """Returns the base name of the file.

    The base name is the current date and time, and the host name.

    Returns:
        str: the base name of the file.

    Example:
        >>> get_file_name_base()
        '2021_08_31_15_54_00_000000_ubuntu'
    """
    return (
        datetime.now(DEFAULT_TIMEZONE).strftime(DATETIME_FORMAT) + "_" + get_host_name()
    )


class RecorderService:
    """Service that subscribes to a list of services and records the events to a file.

    - This service will record events to a file when it receives a request to start.
    - It will stop recording when it receives a request to stop.
    - It will publish the progress of recording.
    - It will only record one recording at a time.
    """

    def __init__(self, event_service: EventServiceGrpc) -> None:
        """Initializes the service.

        Args:
            event_service (EventServiceGrpc): the event service.
        """
        parser = argparse.ArgumentParser()
        parser.add_argument("--data-dir", required=True)
        args = parser.parse_args(event_service.config.args)

        self._data_dir: Path = Path(args.data_dir)
        self._data_dir.mkdir(parents=True, exist_ok=True)
        if not self._data_dir.exists():
            msg = f"{self._data_dir} does not exist"
            raise OSError(msg)

        # set the request reply handler
        self._event_service = event_service
        self._event_service.request_reply_handler = self._request_reply_handler

        # the recorder event service
        self._recorder: EventServiceRecorder | None = None

        # the recorder task
        self._recorder_task: asyncio.Task | None = None

        # For tracking header events (e.g. metadata, calibrations) to be logged in the recordings
        self.header_events: dict[str, tuple[event_pb2.Event, bytes]] = {}

    # public methods

    async def start_recording(
        self,
        file_base: Path,
        config_list: EventServiceConfigList,
        config_name: str | None = None,
    ) -> None:
        """Starts recording the events to a file.

        Args:
            file_base (Path): the base name of the file.
            config_list (EventServiceConfigList): the configuration data structure.
            config_name (str, optional): the name of the config. Defaults to None,
                                            in which case "record_default" is used.
        """
        # stop recording if it is already running
        if self._recorder is not None:
            await self.stop_recording()

        # start recording
        self._recorder = EventServiceRecorder(
            config_name or "record_default",
            config_list,
            list(self.header_events.values()),
        )
        if self._recorder.recorder_config is None:
            msg = "recorder_config is None"
            raise ValueError(msg)

        # Handle if the max_file_mb is set in the record_config args
        # Expectation: single string of "--max-file-mb=500" or "--max-file-mb 500"
        max_file_mb: int = 0
        for arg in self._recorder.recorder_config.args:
            if arg.startswith("--max-file-mb"):
                try:
                    eq_arg = arg.replace(" ", "=").strip()
                    max_file_mb = int(eq_arg.split("=")[1])
                    self._recorder.logger.info("Setting max_file_mb to %s", max_file_mb)
                except ValueError:
                    self._recorder.logger.exception(
                        "Failed to parse max_file_mb from %s",
                        eq_arg,
                    )

        self._recorder_task = asyncio.create_task(
            self._recorder.subscribe_and_record(
                file_base=file_base,
                max_file_mb=max_file_mb,
            ),
        )

        def _safe_done_callback(
            future: asyncio.Task,
            recorder_task: asyncio.Task | None,
        ) -> None:
            try:
                future.result()
            except asyncio.CancelledError:
                self._event_service.logger.debug("cancelled recording task")
            finally:
                del future
                del recorder_task
                self._recorder = None

        self._recorder_task.add_done_callback(
            lambda f: _safe_done_callback(f, self._recorder_task),
        )

    async def stop_recording(self) -> None:
        """Stops recording the events to a file."""
        # do nothing if not recording
        if self._recorder is None or self._recorder_task is None:
            return

        self._recorder.logger.info("stopping recording")

        # cancel the task and set the recorder to None
        self._recorder_task.cancel()

    # private methods

    async def _request_reply_handler(
        self,
        request: RequestReplyRequest,
    ) -> Message:
        """Handles the request reply.

        Args:
            event_service (EventServiceGrpc): the event service.
            request (RequestReplyRequest): the request.

        Returns:
            Message: the response message.
        """
        cmd: str = request.event.uri.path
        # Ensure path was sent with a leading slash
        if not cmd.startswith("/"):
            cmd = "/" + cmd

        if cmd.startswith("/start"):
            # config_name is the optional part of the path after "/start/"
            config_name: str | None = cmd[7:] or None

            config_list: EventServiceConfigList = payload_to_protobuf(
                request.event,
                request.payload,
            )
            self._event_service.logger.info("/start %s: %s", config_name, config_list)
            file_base = self._data_dir.joinpath(get_file_name_base())
            await self.start_recording(file_base, config_list, config_name)
            return StringValue(value=str(file_base))
        if cmd == "/stop":
            self._event_service.logger.info("/stop")
            await self.stop_recording()
        elif cmd.startswith("/header"):
            header_detail: str = cmd[8:] or ""
            if not header_detail:
                msg = "/header command requires a specifier, e.g. '/header/metadata'"
                raise ValueError(msg)
            self._event_service.logger.info(
                "header:\n%s",
                payload_to_protobuf(request.event, request.payload),
            )
            self._event_service.logger.info("with uri:\n%s", request.event.uri)
            self.add_header_event(request.event, request.payload)
        elif cmd == "/clear_headers":
            self._event_service.logger.info("/clear_headers")
            self.header_events.clear()
            if self._recorder is not None:
                self._recorder.header_deque.clear()
        return Empty()

    def add_header_event(self, event: event_pb2.Event, payload: bytes) -> None:
        """Adds a header event to the header_events list.

        If this is a duplicate (per URI), it will replace the existing message.

        Args:
            event (event_pb2.Event): the event.
            payload (bytes): the payload.
        """
        if not isinstance(event, event_pb2.Event):
            error_msg = f"header event must be Event, not {type(event)}"
            raise TypeError(error_msg)
        if not isinstance(payload, bytes):
            error_msg = f"header payload must be bytes, not {type(payload)}"
            raise TypeError(error_msg)
        self.header_events[uri_to_string(event.uri)] = (event, payload)
        if self._recorder is not None:
            self._recorder.add_header_event(event, payload)


def service_command(_args):
    config_list, service_config = load_service_config(args)
    event_service: EventServiceGrpc = EventServiceGrpc(
        grpc.aio.server(),
        service_config,
    )
    RecorderService(event_service)

    async def job():
        # define the async tasks
        async_tasks: list[asyncio.Task] = []

        # add the service task
        async_tasks.append(event_service.serve())

        # run the tasks
        await asyncio.gather(*async_tasks)

    asyncio.get_event_loop().run_until_complete(job())


def client_start_command(_args):
    config_list, service_config = load_service_config(args)
    loop = asyncio.get_event_loop()
    config_name: str = args.config_name

    async def job():
        reply = await EventClient(service_config).request_reply(
            f"/start/{config_name}",
            config_list,
        )
        print(payload_to_protobuf(reply.event, reply.payload))

    loop.run_until_complete(job())


def client_stop_command(_args):
    config_list, service_config = load_service_config(args)
    loop = asyncio.get_event_loop()
    loop.run_until_complete(EventClient(service_config).request_reply("/stop", Empty()))


def record_command(_args):
    async def job():
        configs = proto_from_json_file(_args.service_config, EventServiceConfigList())
        recorder = EventServiceRecorder(_args.config_name, config_list=configs)
        await recorder.subscribe_and_record(
            _args.output,
            _args.extension,
            _args.max_file_mb,
        )

    asyncio.get_event_loop().run_until_complete(job())


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    sub_parsers = parser.add_subparsers()
    service_parser = sub_parsers.add_parser("service")
    add_service_parser(service_parser)
    service_parser.add_argument("--out-dir", default="/tmp/")  # noqa: S108
    service_parser.set_defaults(func=service_command)

    client_parser = sub_parsers.add_parser("start")
    add_service_parser(client_parser)
    client_parser.add_argument("--config-name", default="record_default")
    client_parser.set_defaults(func=client_start_command)

    client_parser = sub_parsers.add_parser("stop")
    add_service_parser(client_parser)
    client_parser.set_defaults(func=client_stop_command)

    record_parser = sub_parsers.add_parser("record")
    record_parser.add_argument("--service-config", required=True, type=str)
    record_parser.add_argument("--output")
    record_parser.add_argument("--extension", default=".bin")
    record_parser.add_argument("--max_file_mb", default=0, type=int)
    record_parser.add_argument("--config-name", default="record_default")

    record_parser.set_defaults(func=record_command)

    args = parser.parse_args()
    if hasattr(args, "func"):

        args.func(args)
    else:
        parser.print_help()
        sys.exit(1)
