from __future__ import annotations

from farm_ng.core.linalg_pb2 import MatXF32, MatXF64, VecXF32, VecXF64


def test_vecx():
    VecXF32(data=[0.0, 0.0, 0.0, 1.0])
    VecXF64(data=[0.0, 0.0, 0.0, 1.0])


def test_matx():
    MatXF32(
        rows=4,
        cols=4,
        data=[
            1.0,
            0.0,
            0.0,
            0.0,  # row 1
            0.0,
            1.0,
            0.0,
            0.0,  # row 2
            0.0,
            0.0,
            1.0,
            0.0,  # row 3
            0.0,
            0.0,
            0.0,
            1.0,
        ],
    )  # row 4

    MatXF64(
        rows=4,
        cols=4,
        data=[
            1.0,
            0.0,
            0.0,
            0.0,  # row 1
            0.0,
            1.0,
            0.0,
            0.0,  # row 2
            0.0,
            0.0,
            1.0,
            0.0,  # row 3
            0.0,
            0.0,
            0.0,
            1.0,
        ],
    )  # row 4
