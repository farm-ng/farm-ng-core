from typing import Any

from farm_ng.core import uri_pb2


class Uri:
    """Wrapper class to `uri_pb2.Uri` for debuggin purposes."""

    def __init__(self, uri: uri_pb2.Uri) -> None:
        self._uri = uri

    def __eq__(self, other: "Uri") -> bool:
        assert isinstance(other, Uri), f"Expected Uri type. Got: {type(other)}"
        return self.string() == other.string()

    def __repr__(self) -> str:
        return self.string()

    @property
    def proto(self) -> uri_pb2.Uri:
        return self._uri

    @property
    def scheme(self) -> str:
        scheme_value = uri_pb2.UriSchemeType.DESCRIPTOR.values[self._uri.scheme]
        return scheme_value.name.lower()

    @classmethod
    def empty(cls) -> "Uri":
        return Uri(uri_pb2.Uri())

    def string(self) -> str:
        return (
            f"{self.scheme}://{self._uri.authority}/{self._uri.path}?{self._uri.query}"
        )

    @classmethod
    def from_message(cls, message: Any) -> "Uri":
        uri = uri_pb2.Uri(
            scheme=str(message.__module__), authority=str(message.__class__.__name__)
        )
        return cls(uri)
