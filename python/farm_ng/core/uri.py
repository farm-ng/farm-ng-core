from farm_ng.core import event_pb2


class Uri:
    def __init__(self, uri: event_pb2.Uri) -> None:
        self._uri = uri

    def __eq__(self, other: "Uri") -> bool:
        assert isinstance(other, Uri), f"Expected Uri type. Got: {type(other)}"
        return self.string() == other.string()

    def __repr__(self) -> str:
        return self.string()

    @property
    def proto(self) -> event_pb2.Uri:
        return self._uri

    @property
    def scheme(self) -> str:
        scheme_value = event_pb2.UriSchemeType.DESCRIPTOR.values[self._uri.scheme]
        return scheme_value.name.lower()

    @classmethod
    def empty(cls) -> "Uri":
        return Uri(event_pb2.Uri())

    def string(self) -> str:
        return f"{self.scheme}://{self._uri.authority}/{self._uri.path}?{self._uri.query}"

    @classmethod
    def from_string(self, string: str) -> "Uri":
        scheme_name, remainder = string.split("://")
        remainder, query = remainder.split("?")
        authority, path = remainder.split("/")
        return Uri.from_strings(scheme_name, authority, path, query)

    @classmethod
    def from_strings(self, scheme_name: str, authority: str, path: str, query: str) -> "Uri":
        scheme_value = event_pb2.UriSchemeType.DESCRIPTOR.values_by_name[scheme_name.upper()]
        return Uri(event_pb2.Uri(scheme=scheme_value.number, authority=authority, path=path, query=query))
