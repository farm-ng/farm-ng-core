from farm_ng.core import uri_pb2
from farm_ng.core import timestamp_pb2
from farm_ng.core.uri import make_proto_uri, uri_to_string, string_to_uri


def test_make_proto():
    stamp = timestamp_pb2.Timestamp(
        stamp=1.2, clock_name="clock0", semantics="test/proto"
    )
    uri: uri_pb2.Uri = make_proto_uri("tik/tok", stamp)
    assert uri.scheme == "protobuf"
    assert uri.path == "tik/tok"
    assert uri.query == "type=Timestamp&farm_ng/core/timestamp.proto"

    uri_str: str = uri_to_string(uri)
    assert "protobuf://" in uri_str
    assert "/tik/tok?type=Timestamp&farm_ng/core/timestamp.proto" in uri_str

    uri_out: uri_pb2.Uri = string_to_uri(uri_str)
    assert uri == uri_out
