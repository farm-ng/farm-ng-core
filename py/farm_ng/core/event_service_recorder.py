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
from pathlib import Path
import sys

from farm_ng.core.event_client import EventClient
from farm_ng.core.event_service_pb2 import (
    EventServiceConfigList,
    SubscribeRequest,
)
from farm_ng.core.events_file_reader import proto_from_json_file
from farm_ng.core.events_file_writer import EventsFileWriter
from farm_ng.core.uri import uri_query_to_dict
from farm_ng.core.stamp import get_monotonic_now, StampSemantics


class EventServiceRecorder:
    def __init__(self, service_name, config_list: EventServiceConfigList):
        self.service_name = service_name
        self.config_list = config_list
        self.recorder_config = None
        self.clients = dict()
        for config in self.config_list.configs:
            if self.service_name == config.name:
                self.recorder_config = config
            if config.port != 0:
                self.clients[config.name] = EventClient(config)

        assert (
            self.recorder_config is not None
        ), f"service {self.service_name} not found in config list {self.config_list}"
        self.record_queue: asyncio.Queue = asyncio.Queue()

    async def record(
        self, file_base: str | Path, extension: str = ".bin", max_file_mb: int = 0
    ):
        with EventsFileWriter(file_base, extension, max_file_mb) as writer:
            while True:
                event, payload = await self.record_queue.get()
                event.timestamps.append(
                    get_monotonic_now(semantics=StampSemantics.FILE_WRITE)
                )
                writer.write_event_payload(event, payload)

    async def subscribe(self, client: EventClient, subscription: SubscribeRequest):
        async for event, payload in client.subscribe(subscription, decode=False):
            self.record_queue.put_nowait((event, payload))

    async def subscribe_and_record(
        self, file_base: str | Path, extension: str = ".bin", max_file_mb: int = 0
    ):
        async_subscriptions: list[asyncio.Task] = [
            asyncio.create_task(
                self.record(
                    file_base=file_base, extension=extension, max_file_mb=max_file_mb
                )
            )
        ]
        for subscription in self.recorder_config.subscriptions:
            query = uri_query_to_dict(subscription.uri)
            client = self.clients[query["service_name"]]
            async_subscriptions.append(
                asyncio.create_task(self.subscribe(client, subscription))
            )
        await asyncio.gather(*async_subscriptions)


def record_command(args):
    async def job():
        configs = proto_from_json_file(args.config, EventServiceConfigList())
        recorder = EventServiceRecorder(args.config_name, config_list=configs)
        await recorder.subscribe_and_record(
            args.output, args.extension, args.max_file_mb
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
