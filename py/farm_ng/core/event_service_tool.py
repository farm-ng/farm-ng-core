"""
Usage:
python -m farm_ng.core.event_service_tool uris config.json

This will print a config file will all the uris currently being published by the config

python -m farm_ng.core.event_service_tool config-gen config2.json

This print a sample config file with a single service called test_service, and an example
recorder service configuration (note with no port numbers so scripts no not to connect)

Runs a single service from the config file

python -m farm_ng.core.event_service_tool launch1 --service-config config.json --service-name test_service

"""
import argparse
import asyncio
import sys

from farm_ng.core.event_client import EventClient
from farm_ng.core.event_service import add_service_parser, load_service_config
from farm_ng.core.event_service_pb2 import (
    EventServiceConfig,
    EventServiceConfigList,
    SubscribeRequest,
)
from farm_ng.core.events_file_writer import proto_to_json_file
from farm_ng.core.uri_pb2 import Uri
from google.protobuf.json_format import MessageToJson


async def config_gen_command(args):
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
                            uri=Uri(path="/test", query="service_name=bar"),
                            every_n=1,
                        ),
                    ],
                    args=["--my-arg=my-value", "foo"],
                    log_level=EventServiceConfig.LogLevel.INFO,
                ),
                EventServiceConfig(
                    name="record_default",
                    subscriptions=[
                        SubscribeRequest(
                            uri=Uri(path="*", query="service_name={args.name}"),
                            every_n=1,
                        ),
                    ],
                ),
            ],
        ),
    )


async def list_uri(config: EventServiceConfig):
    print("connecting to", config)
    client = EventClient(config)
    uris = await client.list_uris()
    del config.uris[:]
    config.uris.extend(uris)
    return config


async def uris_command(args):
    configs, service_config = load_service_config(args)
    config = await list_uri(service_config)
    print(MessageToJson(config))


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
    add_service_parser(uris_parser)
    uris_parser.set_defaults(func=uris_command)

    args = parser.parse_args()
    if hasattr(args, "func"):
        asyncio.run(args.func(args))
    else:
        parser.print_help()
        sys.exit(1)
