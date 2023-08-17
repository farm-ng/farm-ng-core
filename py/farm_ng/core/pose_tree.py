from __future__ import annotations

from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from farm_ng.core import lie_pb2, pose_pb2

# Usage example:
# pose_tree = ... # Load your PoseTree proto object here.
# result = find_poses_by_frames(pose_tree, "base_link", "wheel_link")
# print(result)

# NOTE: pep8-naming throws an error here because names should be snake_case.


def find_transform(
    pose_tree: pose_pb2.PoseTree,
    frame_A: str,
    frame_B: str,
) -> lie_pb2.Isometry3F64 | None:
    """Find all poses in a PoseTree with a given frame_A and frame_B.

    If frames are inverted, returns the inverse transform.

    Args:
        pose_tree (PoseTree): The PoseTree proto object to search within.
        frame_A (str): The desired frame_A value.
        frame_B (str): The desired frame_B value.

    Returns:
        Pose: Pose objects that match the criteria.
    """
    pose: pose_pb2.Pose
    for pose in pose_tree.poses:
        if pose.frame_A == frame_A and pose.frame_B == frame_B:
            return pose.B_from_A
        if pose.frame_A == frame_B and pose.frame_B == frame_A:
            print("NOT IMPLEMENTED, please swap your frames")
            return None
    return None
