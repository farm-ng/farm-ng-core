"""
Usage:
python -m farm_ng.core.event_service_tool uris config.json

This will print a config file will all the uris currently being published by the config

python -m farm_ng.core.event_service_tool config-gen config2.json

This print a sample config file with a single service called test_service, and an example recorder service configuration (note with no port numbers so scripts no not to connect)


Runs a single service from the config file

python -m farm_ng.core.event_service_tool launch1 --service-config config.json --service-name test_service

"""
import signal
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
from farm_ng.core.event_service import add_service_parser, load_service_config
from farm_ng.core.subprocess import (
    get_subprocess_manager,
    Subprocess,
    SubprocessManager,
)


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
                    python_module="farm_ng.core.event_service",
                    args=["--my-arg=my-value", "foo"],
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


def create_subprocess_from_config(service_config: EventServiceConfig):
    return Subprocess(
        service_config.name,
        [
            "python",
            "-m",
            service_config.python_module,
            "--service-config",
            args.service_config,
            "--service-name",
            service_config.name,
        ]
        + [arg for arg in service_config.args],
    )


def launch1_command(args):
    config_list, service_config = load_service_config(args)
    my_process = create_subprocess_from_config(service_config=service_config)

    async def job():
        await my_process.start()
        while True:
            await asyncio.sleep(1)
            print(my_process.name, my_process.pid, my_process.state)

    asyncio.get_event_loop().run_until_complete(job())


def launch_all_command(args):
    # TODO kill all processes.
    config_list = proto_from_json_file(args.service_config, EventServiceConfigList())

    async def job():
        process_list = []
        task_list = []
        for config in config_list.configs:
            if len(config.python_module):
                process_list.append(create_subprocess_from_config(config))
                task_list.append(asyncio.create_task(process_list[-1].start()))
        await asyncio.gather(*task_list)
        while True:
            await asyncio.sleep(1)
            for my_process in process_list:
                print(my_process.name, my_process.pid, my_process.state)

    loop = asyncio.get_event_loop()
    try:
        loop.run_until_complete(job())
    except KeyboardInterrupt:
        print("KeyboardInterrupt error")
    except asyncio.CancelledError:
        print("asyncio.Cancelled error")
    finally:
        loop.close()

    print("launch_all done")


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

    launch_parser = sub_parsers.add_parser("launch1")
    add_service_parser(launch_parser)
    launch_parser.set_defaults(func=launch1_command)

    launch_parser = sub_parsers.add_parser("launch_all")
    launch_parser.add_argument("--service-config", type=str, required=True)
    launch_parser.set_defaults(func=launch_all_command)

    def signal_handler() -> None:
        """Handle SIGTERM."""
        print("SIGTERM received. Exiting...")
        sys.exit(0)

    signal.signal(signal.SIGTERM, lambda signum, frame: signal_handler())

    args = parser.parse_args()
    if hasattr(args, "func"):

        args.func(args)
    else:
        parser.print_help()
        sys.exit(1)
