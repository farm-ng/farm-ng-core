from dataclasses import dataclass
import platform

from farm_ng.core import uri_pb2
from google.protobuf.message import Message


@dataclass
class PlatformConfig:
    host_name: str = platform.node()


# https://vald-phoenix.github.io/pylint-errors/plerr/errors/variables/W0603.html
platform_config = PlatformConfig()


def set_host_name(name: str) -> None:
    # global HOST_NAME
    # HOST_NAME = name
    platform_config.host_name = name


def get_host_name() -> str:
    return platform_config.host_name


def get_authority() -> str:
    return get_host_name()


def make_proto_uri(path, message: Message) -> uri_pb2.Uri:
    return uri_pb2.Uri(
        scheme="protobuf",
        authority=get_authority(),
        path=path,
        query=f"type={message.DESCRIPTOR.full_name}&pb={message.DESCRIPTOR.file.name}",
    )


def uri_to_string(uri: uri_pb2.Uri) -> str:
    return f"{uri.scheme}://{uri.authority}/{uri.path}?{uri.query}"


def string_to_uri(string: str) -> uri_pb2.Uri:
    scheme_name, remainder = string.split("://")
    remainder, query = remainder.split("?")
    authority_path = remainder.split("/")
    authority = authority_path[0]
    path = "/".join(authority_path[1:])
    return uri_pb2.Uri(scheme=scheme_name, authority=authority, path=path, query=query)
