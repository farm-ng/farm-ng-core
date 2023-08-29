from __future__ import annotations

from farm_ng.core.pose_pb2 import Pose, PoseTree
from farm_ng.core.pose_tree import find_pose


def test_pose_tree_find_pose_a_b():
    tree = PoseTree()
    tree.poses.append(Pose(frame_a="a", frame_b="b"))
    pose: Pose | None = None
    pose = find_pose(tree, frame_a="a", frame_b="b")
    assert pose is not None
    assert pose.frame_a == "a"
    assert pose.frame_b == "b"

    pose = find_pose(tree, frame_a="b", frame_b="a")
    assert pose is None, "not implemented yet"

    # should this return identity?
    pose = find_pose(tree, frame_a="a", frame_b="a")
    assert pose is None

    pose = find_pose(tree, frame_a="a", frame_b="c")
    assert pose is None

    pose = find_pose(tree, frame_a="c", frame_b="d")
    assert pose is None
