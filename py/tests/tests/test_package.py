import farm_ng


def test_import() -> None:
    assert farm_ng.__version__ is not None
    assert farm_ng.core is not None
