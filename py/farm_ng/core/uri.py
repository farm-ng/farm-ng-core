from farm_ng.core import uri_pb2


class Uri:
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

    @classmethod
    def empty(cls) -> "Uri":
        return Uri(uri_pb2.Uri())

    def string(self) -> str:
        return f"{self.proto.scheme}://{self.proto.authority}/{self.proto.path}?{self.proto.query}"

    @classmethod
    def from_string(cls, string: str) -> "Uri":
        scheme_name, remainder = string.split("://")
        remainder, query = remainder.split("?")
        authority, path = remainder.split("/")
        return Uri.from_strings(scheme_name, authority, path, query)

    @classmethod
    def from_strings(
        cls, scheme_name: str, authority: str, path: str, query: str
    ) -> "Uri":
        scheme_value = uri_pb2.UriSchemeType.DESCRIPTOR.values_by_name[
            scheme_name.upper()
        ]
        return Uri(
            uri_pb2.Uri(
                scheme=scheme_value.number, authority=authority, path=path, query=query
            )
        )
