import farm_ng


def test_import() -> None:
    assert farm_ng.core is not None
    assert farm_ng.core.__version__ is not None
