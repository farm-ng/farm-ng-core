from __future__ import annotations

from math import cos, radians

import farm_ng_core_pybind as ng
import numpy as np
import pytest
from farm_ng.core import lie_pb2


def test_rotation():
    r = ng.Rotation3F64.exp([0, 0, 0])
    assert r.params.shape[0] == 4
    assert np.array_equal(r.params, [0.0, 0.0, 0.0, 1.0])
    ng.Rotation3F64(r.params)
    r_m = r.rotation_matrix
    assert r_m.shape[0] == 3
    assert r_m.shape[1] == 3
    assert np.array_equal(r_m, [[1.0, 0.0, 0.0], [0.0, 1.0, 0.0], [0.0, 0.0, 1.0]])
    r_i = r.inverse()
    assert np.array_equal(r.params, r_i.params)

    rvec = r.log()
    assert rvec.shape[0] == 3
    r3 = ng.Rotation3F64.exp(rvec)
    assert np.array_equal(r3.log(), rvec)

    r4 = r3 * r
    assert np.array_equal(r4.log(), rvec)


def test_isometry():
    r2d_90 = ng.Isometry2F64([0, 0], radians(90))
    print(r2d_90.rotation_matrix)
    assert np.allclose(r2d_90.rotation_matrix, [[0.0, -1.0], [1.0, 0.0]])

    r3d_90 = ng.Isometry3F64([0, 0, 0], ng.Rotation3F64.exp([0, 0, radians(90)]))
    print(r3d_90.rotation_matrix)
    assert np.allclose(
        r3d_90.rotation_matrix,
        [[0.0, -1.0, 0.0], [1.0, 0.0, 0.0], [0.0, 0.0, 1.0]],
    )

    eye3d = r3d_90.inverse() * r3d_90
    print(eye3d.rotation_matrix, "\n", eye3d.translation, eye3d.log())
    assert np.allclose(
        eye3d.rotation_matrix,
        [[1.0, 0.0, 0.0], [0.0, 1.0, 0.0], [0.0, 0.0, 1.0]],
    )
    assert np.allclose(eye3d.log(), ng.Isometry3F64().log())

    yaw = ng.Isometry3F64.Rz(radians(45))
    pitch = ng.Isometry3F64.Ry(radians(-3))
    roll = ng.Isometry3F64.Rx(radians(5))
    nautical = yaw * pitch * roll
    print(nautical)


def test_protobuf():
    r2d = ng.Rotation2F64()
    pb = r2d.to_proto()
    assert type(pb) == lie_pb2.Rotation2F64
    pb.theta = radians(90)
    print(pb)
    r2d = ng.Rotation2F64.from_proto(pb)
    print(r2d.to_proto())
    assert np.allclose(r2d.rotation_matrix, [[0.0, -1.0], [1.0, 0.0]])
    assert r2d.to_proto().theta == radians(90)

    r3d = ng.Rotation3F64.exp([0, 0, radians(90)])
    pb = r3d.to_proto()
    assert type(pb) == lie_pb2.Rotation3F64
    print(pb)
    assert np.allclose([pb.unit_quaternion.real], [cos(radians(90) / 2)])
    assert np.allclose(
        [
            pb.unit_quaternion.imag.x,
            pb.unit_quaternion.imag.y,
            pb.unit_quaternion.imag.z,
        ],
        [0, 0, cos(radians(90) / 2)],
    )

    i3d = ng.Isometry3F64(
        [1, 2, 3],
        ng.Rotation3F64.exp([radians(45 / 2), radians(45), radians(90)]),
    )
    pb = i3d.to_proto()
    assert np.allclose(
        [
            pb.rotation.unit_quaternion.imag.x,
            pb.rotation.unit_quaternion.imag.y,
            pb.rotation.unit_quaternion.imag.z,
            pb.rotation.unit_quaternion.real,
        ],
        i3d.params[:4],
    )

    i3d_2 = ng.Isometry3F64.from_proto(pb)
    assert np.allclose(i3d_2.params, i3d.params)


def test_pose():
    a_from_b = ng.Pose3F64(
        a_from_b=ng.Isometry3F64(
            translation=[1, 2, 3],
            rotation=ng.Rotation3F64.exp([radians(45 / 2), radians(45), radians(90)]),
        ),
        frame_a="a",
        frame_b="b",
    )

    b_from_c = ng.Pose3F64(
        a_from_b=a_from_b.a_from_b.inverse(),
        frame_a="b",
        frame_b="c",
    )

    a_from_c = a_from_b * b_from_c
    print("woot", ng.Rotation3F64.from_proto(a_from_c.rotation.to_proto()).to_proto())
    ng.Pose3F64.from_proto(a_from_c.to_proto())
    print("Here!", a_from_c.log())
    assert a_from_c.frame_a == "a"
    assert a_from_c.frame_b == "c"

    assert np.allclose(a_from_c.log(), ng.Isometry3F64().log())

    with pytest.raises(ValueError, match="Pose frame error: lhs a=b b=a rhs a=b b=c"):
        a_from_b.inverse() * b_from_c

    dt = 1.0 / 50.0
    world_from_robot = ng.Pose3F64(ng.Isometry3F64(), frame_a="world", frame_b="robot")

    tangent_of_now_in_prev = [1.0, 0, 0, 0, 0, radians(90)]
    robot_from_robot_t1 = ng.Pose3F64.exp(
        tangent_of_now_in_prev,
        dt,
        "robot",
        "robot_t1",
    )
    for _i in range(50):
        world_from_robot_t1 = world_from_robot * robot_from_robot_t1
        world_from_robot_t1 = ng.Pose3F64.from_proto(world_from_robot_t1.to_proto())
        # note we keep the word_from_robot transform free of velocity
        world_from_robot_now = ng.Pose3F64(
            world_from_robot_t1.a_from_b,
            frame_a="world",
            frame_b="robot",
        )
        # computes the error between frame_b of two respective poses
        # e.g. robot and robot now
        err = np.sqrt(ng.Pose3F64.error(world_from_robot, world_from_robot_now)) * (
            1 / dt
        )
        # print(err)
        # print(tangent_of_now_in_prev)
        assert np.allclose(err, tangent_of_now_in_prev)
        world_from_robot = world_from_robot_now
