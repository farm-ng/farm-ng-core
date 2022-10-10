from farm_ng.core import uri_pb2
from google.protobuf.message import Message
import platform

_g_host_name = platform.node()


def set_host_name(name: str) -> None:
    global _g_host_name
    _g_host_name = name


def get_host_name() -> str:
    return _g_host_name


def get_authority() -> str:
    return get_host_name()


def make_proto_uri(path, message: Message) -> uri_pb2.Uri:
    return uri_pb2.Uri(
        scheme="protobuf",
        authority=get_authority(),
        path=path,
        query="type=" + message.DESCRIPTOR.full_name,
    )


def uri_to_string(uri: uri_pb2.Uri) -> str:
    return f"{uri.scheme}://{uri.authority}/{uri.path}?{uri.query}"


def string_to_uri(string: str) -> uri_pb2.Uri:
    scheme_name, remainder = string.split("://")
    remainder, query = remainder.split("?")
    authority, path = remainder.split("/")
    return uri_pb2.Uri(scheme=scheme_name, authority=authority, path=path, query=query)
