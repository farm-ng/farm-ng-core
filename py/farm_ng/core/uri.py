from __future__ import annotations

import importlib
import platform
from dataclasses import dataclass, field
from typing import TYPE_CHECKING

from farm_ng.core import uri_pb2

if TYPE_CHECKING:
    from google.protobuf.message import Message


__all__ = [
    "PlatformConfig",
    "get_host_name",
    "get_authority",
    "make_proto_uri",
    "uri_to_string",
    "string_to_uri",
    "uri_query_to_dict",
    "uri_to_protobuf_type",
]


def _get_platform_node() -> str:
    """Return the platform node name.

    Example:
        >>> get_platform_node()
        'farm-ng-1'
    """
    return platform.node()


@dataclass
class PlatformConfig:
    """Platform configuration."""

    host_name: str = field(default_factory=_get_platform_node)


# https://vald-phoenix.github.io/pylint-errors/plerr/errors/variables/W0603.html
platform_config = PlatformConfig()


def set_host_name(name: str) -> None:
    """Set the host name."""
    # global HOST_NAME
    platform_config.host_name = name


def get_host_name() -> str:
    """Return the host name.

    Example:
        >>> get_host_name()
        'farm-ng-1'
    """
    return platform_config.host_name


def get_authority() -> str:
    """Return the authority.

    Example:
        >>> get_authority()
        'farm-ng-1'
    """
    return get_host_name()


def make_proto_uri(
    path: str,
    message: Message,
    service_name: str | None = None,
) -> uri_pb2.Uri:
    """Return a protobuf uri from protobuf message.

    Args:
        path: the semantic path.
        message: the protobuf message.

    Returns:
        uri_pb2.Uri: the protobuf uri.

    Example:
        >>> from farm_ng.core import timestamp_pb2
        >>> stamp = timestamp_pb2.Timestamp(
        ...     stamp=1.2,
        ...     clock_name="clock0",
        ...     semantics="test/proto",
        ... )
        >>> make_proto_uri("tik/tok", stamp)
        scheme: "protobuf"
        authority: "farm-ng-1"
        path: "tik/tok"
        query: "type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto"
    """
    service_query = ""
    if service_name is not None:
        service_query = f"&service_name={service_name}"
    return uri_pb2.Uri(
        scheme="protobuf",
        authority=get_authority(),
        path=path,
        query=f"type={message.DESCRIPTOR.full_name}&pb={message.DESCRIPTOR.file.name}{service_query}",
    )


def get_service_name(uri: uri_pb2.Uri) -> str | None:
    """Return the service name from a protobuf uri.

    Args:
        uri: uri_pb2.Uri

    Returns:
        str | None: the service name.

    Example:
        >>> uri = Uri()
        >>> uri.query = "type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto&service_name=service"
        >>> get_service_name(uri)
        'service'
    """
    return uri_query_to_dict(uri).get("service_name", None)


def uri_to_string(uri: uri_pb2.Uri) -> str:
    """Return a string from a protobuf uri.

    Args:
        uri: uri_pb2.Uri

    Returns:
        str: the string representation of the uri.

    Example:
        >>> uri = Uri()
        >>> uri.scheme = "protobuf"
        >>> uri.authority = "farm-ng-1"
        >>> uri.query = "type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto"
        >>> uri_to_string(uri)
        'protobuf://farm-ng-1//?type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto'
    """
    return f"{uri.scheme}://{uri.authority}/{uri.path}?{uri.query}"


def string_to_uri(string: str) -> uri_pb2.Uri:
    """Return a protobuf uri from a string.

    Args:
        string: the string representation of the uri.

    Returns:
        uri_pb2.Uri: the protobuf uri.

    Example:
        >>> string = "protobuf://farm-ng-1//?type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto"
        >>> string_to_uri(string)
        scheme: "protobuf"
        authority: "farm-ng-1"
        path: ""
        query: "type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto"
    """
    scheme_name, remainder = string.split("://")
    remainder, query = remainder.split("?")
    authority_path = remainder.split("/")
    authority = authority_path[0]
    path = "/".join(authority_path[1:])
    return uri_pb2.Uri(scheme=scheme_name, authority=authority, path=path, query=query)


def uri_query_to_dict(uri: uri_pb2.Uri) -> dict[str, str]:
    """Return a dictionary from a uri query.

    Args:
        uri: uri_pb2.Uri

    Returns:
        dict[str, str]: the dictionary representation of the uri query.

    Example:
        >>> uri = Uri()
        >>> uri.query = "type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto"
        >>> uri_query_to_dict(uri)
        {'type': 'farm_ng.core.proto.Timestamp', 'pb': 'farm_ng/core/timestamp.proto'}
    """
    return dict([x.split("=") for x in uri.query.split("&")])


def uri_to_protobuf_type(uri: uri_pb2.Uri) -> type[Message]:
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
    name, package = _parse_protobuf_descriptor(uri)
    return getattr(importlib.import_module(package), name)


# private functions

# parse a proto descriptor and extract the message name and package.
# See: https://developers.google.com/protocol-buffers/docs/reference/python-generated#invocation
# NOTE: the descriptor comes in the shape of:
# `type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto`


def _parse_protobuf_descriptor(uri: uri_pb2.Uri) -> tuple[str, str]:
    """Parse a protobuf descriptor and extract the message name and package.

    Args:
        uri: uri_pb2.Uri

    Returns:
        tuple[str, str]: message name and package

    Example:
        >>> uri = Uri()
        >>> uri.query = "type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto"
        >>> parse_protobuf_descriptor(uri)
        ('Timestamp', 'farm_ng.core.timestamp_pb2')
    """
    if uri.scheme != "protobuf":
        msg = f"Invalid uri scheme: {uri.scheme}"
        raise ValueError(msg)

    # convert the query string to a dictionary and extract the type and protobuf file name
    query: dict[str, str] = uri_query_to_dict(uri)
    type_split: str | None = query.get("type", None)
    pb_split: str | None = query.get("pb", None)

    if type_split is None:
        msg = f"Invalid uri query: {uri.query}"
        raise ValueError(msg)

    if pb_split is None:
        msg = f"Invalid uri query: {uri.query}"
        raise ValueError(msg)

    # the message name is the last part of the type
    type_name: str = type_split.split(".")[-1]

    # parse the pb file location and convert to python module extension
    package_name: str = pb_split.replace(".proto", "_pb2").replace("/", ".")

    return type_name, package_name
