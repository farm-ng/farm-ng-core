import pytest
from farm_ng.core import timestamp_pb2, uri_pb2
from farm_ng.core.uri import (
    _parse_protobuf_descriptor,
    get_service_name,
    make_proto_uri,
    string_to_uri,
    uri_query_to_dict,
    uri_to_protobuf_type,
    uri_to_string,
)


@pytest.fixture()
def message():
    return timestamp_pb2.Timestamp(
        stamp=1.2,
        clock_name="clock0",
        semantics="test/proto",
    )


class TestUri:
    def test_make_proto_uri(self, message):
        uri: uri_pb2.Uri = make_proto_uri("tik/tok", message)
        assert uri.scheme == "protobuf"
        assert uri.path == "tik/tok"
        assert (
            uri.query
            == "type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto"
        )

    def test_uri_to_string(self, message):
        uri: uri_pb2.Uri = make_proto_uri("tik/tok", message)
        uri_str: str = uri_to_string(uri)
        assert "protobuf://" in uri_str
        assert (
            "/tik/tok?type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto"
            in uri_str
        )

    def test_string_to_uri(self, message):
        uri: uri_pb2.Uri = make_proto_uri("tik/tok", message)
        uri_str: str = uri_to_string(uri)
        assert uri == string_to_uri(uri_str)

    def test_uri_query_to_dict(self, message):
        uri: uri_pb2.Uri = make_proto_uri("tik/tok", message)
        assert uri_query_to_dict(uri) == {
            "type": "farm_ng.core.proto.Timestamp",
            "pb": "farm_ng/core/timestamp.proto",
        }

    def test_uri_to_protobuf_type(self, message):
        uri: uri_pb2.Uri = make_proto_uri("tik/tok", message)
        assert uri_to_protobuf_type(uri) == timestamp_pb2.Timestamp

    def test_parse_protobuf_descriptor(self, message):
        uri: uri_pb2.Uri = make_proto_uri("tik/tok", message)
        assert _parse_protobuf_descriptor(uri) == (
            "Timestamp",
            "farm_ng.core.timestamp_pb2",
        )

    def test_get_service_name(self, message):
        uri: uri_pb2.Uri = make_proto_uri("tik/tok", message)
        assert get_service_name(uri) is None

        uri = make_proto_uri("tik/tok", message, service_name="tik_service")
        assert get_service_name(uri) == "tik_service"
