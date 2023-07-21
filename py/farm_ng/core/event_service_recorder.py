"""
# run the event_service which starts a simple test publisher
python -m farm_ng.core.event_service
# run the event_recorder which subscribes to the test publisher and records the events to a file
python -m farm_ng.core.event_service_recorder record config.json --config_name=record_all foo
# note that the config file has an EventServiceConfig with name "record_all" which subscribes to the test publisher

# then try playing back log file:
python -m farm_ng.core.events_file_reader playback foo.0000.bin
"""
from __future__ import annotations

import argparse
import asyncio
import logging
from pathlib import Path
import sys

from farm_ng.core import event_pb2
from farm_ng.core.event_client import EventClient
from farm_ng.core.event_service_pb2 import (
    EventServiceConfig,
    EventServiceConfigList,
    SubscribeRequest,
)
from farm_ng.core.events_file_reader import proto_from_json_file
from farm_ng.core.events_file_writer import EventsFileWriter
from farm_ng.core.uri import uri_query_to_dict
from farm_ng.core.stamp import get_monotonic_now, StampSemantics
from google.protobuf.message import Message


class EventServiceRecorder:
    """Service that subscribes to a list of services and records the events to a file."""

    # the maximum size of the queue
    QUEUE_MAX_SIZE: int = 50

    def __init__(self, service_name: str, config_list: EventServiceConfigList) -> None:
        """Initializes the service.

        Args:
            service_name (str): the name of the service.
            config_list (EventServiceConfigList): the configuration data structure.
        """
        self.service_name: str = service_name
        self.config_list: EventServiceConfigList = config_list
        self.recorder_config: EventServiceConfig | None = None

        # the service logger
        self._logger: logging.Logger = logging.getLogger(service_name)

        # map of service name to EventClient
        self.clients: dict[str, EventClient] = {}

        # find the recorder config and create the clients
        config: EventServiceConfig
        for config in self.config_list.configs:
            if self.service_name == config.name:
                self.recorder_config = config
            if config.port != 0:
                self.clients[config.name] = EventClient(config)

        assert (
            self.recorder_config is not None
        ), f"service {self.service_name} not found in config list {self.config_list}"

        # the queue to store the events
        self.record_queue: asyncio.Queue[event_pb2.Event, Message] = asyncio.Queue(
            maxsize=self.QUEUE_MAX_SIZE
        )

    @property
    def logger(self) -> logging.Logger:
        """Returns the service logger."""
        return self._logger

    async def record(
        self, file_base: str | Path, extension: str = ".bin", max_file_mb: int = 0
    ) -> None:
        """Records the events that arrive in the queue to a file.

        Args:
            file_base (str | Path): the base name of the file.
            extension (str, optional): the extension of the file. Defaults to ".bin".
            max_file_mb (int, optional): the maximum size of the file in MB. Defaults to 0.
        """
        with EventsFileWriter(file_base, extension, max_file_mb) as writer:
            while True:
                # await a new event and payload, and write it to the file
                event: event_pb2.Event
                payload: bytes
                event, payload = await self.record_queue.get()
                event.timestamps.append(
                    get_monotonic_now(semantics=StampSemantics.FILE_WRITE)
                )
                writer.write_event_payload(event, payload)

    async def subscribe(
        self, client: EventClient, subscription: SubscribeRequest
    ) -> None:
        """Subscribes to a service and puts the events in the queue.

        Args:
            client (EventClient): the client to subscribe to.
            subscription (SubscribeRequest): the subscription request.
        """
        event: event_pb2.Event
        payload: bytes
        async for event, payload in client.subscribe(subscription, decode=False):
            try:
                self.record_queue.put_nowait((event, payload))
            except asyncio.QueueFull:
                self.logger.warning("Queue full, dropping event %s", event)
                continue

    async def subscribe_and_record(
        self, file_base: str | Path, extension: str = ".bin", max_file_mb: int = 0
    ) -> None:
        """Subscribes to a list of services and records the events to a file.

        Args:
            file_base (str | Path): the base name of the file.
            extension (str, optional): the extension of the file. Defaults to ".bin".
            max_file_mb (int, optional): the maximum size of the file in MB. Defaults to 0.
        """
        # create the tasks list. the first task is the record task
        async_tasks: list[asyncio.Task] = [
            asyncio.create_task(
                self.record(
                    file_base=file_base, extension=extension, max_file_mb=max_file_mb
                )
            )
        ]

        # add the subscribe tasks based on the subscriptions in the recorder config
        subscription: SubscribeRequest
        for subscription in self.recorder_config.subscriptions:
            query: dict[str, str] = uri_query_to_dict(uri=subscription.uri)
            client: EventClient = self.clients[query["service_name"]]
            async_tasks.append(
                asyncio.create_task(self.subscribe(client, subscription))
            )
        await asyncio.gather(*async_tasks)


def record_command(_args):
    async def job():
        configs = proto_from_json_file(_args.config, EventServiceConfigList())
        recorder = EventServiceRecorder(_args.config_name, config_list=configs)
        await recorder.subscribe_and_record(
            _args.output, _args.extension, _args.max_file_mb
        )

    asyncio.get_event_loop().run_until_complete(job())


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    sub_parsers = parser.add_subparsers()
    record_parser = sub_parsers.add_parser("record")
    record_parser.add_argument("config")
    record_parser.add_argument("output")
    record_parser.add_argument("--extension", default=".bin")
    record_parser.add_argument("--max_file_mb", default=0, type=int)
    record_parser.add_argument("--config_name", default="record_default")

    record_parser.set_defaults(func=record_command)

    args = parser.parse_args()
    if hasattr(args, "func"):

        args.func(args)
    else:
        parser.print_help()
        sys.exit(1)
