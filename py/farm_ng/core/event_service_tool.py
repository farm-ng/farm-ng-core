"""
Usage:
python -m farm_ng.core.event_service_tool uris config.json

This will print a config file will all the uris currently being published by the config

python -m farm_ng.core.event_service_tool config-gen config2.json

This print a sample config file with a single service called test_service, and an example recorder service configuration (note with no port numbers so scripts no not to connect)
"""

import argparse
import sys
import asyncio
from farm_ng.core.event_service_pb2 import (
    EventServiceConfigList,
    EventServiceConfig,
    SubscribeRequest,
)
from farm_ng.core.uri_pb2 import Uri
from farm_ng.core.events_file_writer import proto_to_json_file
from farm_ng.core.events_file_reader import proto_from_json_file
from farm_ng.core.event_client import EventClient
from google.protobuf.json_format import MessageToJson


def config_gen_command(args):
    proto_to_json_file(
        args.output,
        EventServiceConfigList(
            configs=[
                EventServiceConfig(
                    name=args.name,
                    host=args.host,
                    port=args.port,
                    subscriptions=[
                        SubscribeRequest(
                            uri=Uri(path="/test", query="service_name=bar"), every_n=1
                        )
                    ],
                ),
                EventServiceConfig(
                    name="record_default",
                    subscriptions=[
                        SubscribeRequest(
                            uri=Uri(path="*", query="service_name={args.name}"),
                            every_n=1,
                        )
                    ],
                ),
            ]
        ),
    )


async def list_uri(config: EventServiceConfig):
    client = EventClient(config)
    uris = await client.listUris()
    del config.uris[:]
    config.uris.extend(uris)
    return config


def uris_command(args):
    configs = proto_from_json_file(args.config, EventServiceConfigList())

    async def job():
        configs_out = EventServiceConfigList()
        for config in configs.configs:
            if config.port != 0:
                config = await list_uri(config)
                configs_out.configs.append(config)
        if args.config_out:
            proto_to_json_file(args.config_out, configs_out)
        else:
            print(MessageToJson(configs_out))

    asyncio.get_event_loop().run_until_complete(job())


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    sub_parsers = parser.add_subparsers()
    config_gen_parser = sub_parsers.add_parser("config-gen")
    config_gen_parser.add_argument("output")
    config_gen_parser.add_argument("--host", default="localhost")
    config_gen_parser.add_argument("--name", default="test_service")
    config_gen_parser.add_argument("--port", default=5001, type=int)
    config_gen_parser.add_argument("--subscription", default="/test", type=str)

    config_gen_parser.set_defaults(func=config_gen_command)

    uris_parser = sub_parsers.add_parser("uris")
    uris_parser.add_argument("config")
    uris_parser.add_argument("--config_out", nargs="?", default=None)
    uris_parser.set_defaults(func=uris_command)

    args = parser.parse_args()
    if hasattr(args, "func"):

        args.func(args)
    else:
        parser.print_help()
        sys.exit(1)
