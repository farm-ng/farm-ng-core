from __future__ import annotations

from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from farm_ng.core import pose_pb2

# Usage example:
# pose_tree = ... # Load your PoseTree proto object here.
# result = find_poses_by_frames(pose_tree, "base_link", "wheel_link")
# print(result)

# NOTE: pep8-naming throws an error here because names should be snake_case.


def find_pose(
    pose_tree: pose_pb2.PoseTree,
    frame_a: str,
    frame_b: str,
) -> pose_pb2.Pose | None:
    """Find all poses in a PoseTree with a given frame_A and frame_B.

    If frames are inverted, returns the inverse transform.

    Args:
        pose_tree (PoseTree): The PoseTree proto object to search within.
        frame_a (str): The desired frame_a value.
        frame_b (str): The desired frame_b value.

    Returns:
        Pose: Pose objects that match the criteria.
    """
    pose: pose_pb2.Pose
    for pose in pose_tree.poses:
        if pose.frame_a == frame_a and pose.frame_b == frame_b:
            return pose
        if pose.frame_a == frame_b and pose.frame_b == frame_a:
            print("NOT IMPLEMENTED, please swap your frames")
            return None
    return None
