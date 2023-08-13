from farm_ng.core import pose_pb2

# Usage example:
# pose_tree = ... # Load your PoseTree proto object here.
# result = find_poses_by_frames(pose_tree, "base_link", "wheel_link")
# print(result)


def find_transform(pose_tree: pose_pb2.PoseTree, frame_A: str, frame_B: str):
    """Find all poses in a PoseTree with a given frame_A and frame_B.
       If frames are inverted, returns the inverse transform.

    Args:
    - pose_tree (PoseTree): The PoseTree proto object to search within.
    - frame_A (str): The desired frame_A value.
    - frame_B (str): The desired frame_B value.

    Returns:
    - Pose: Pose objects that match the criteria.
    """

    for pose in pose_tree.poses:
        if pose.frame_A == frame_A and pose.frame_B == frame_B:
            return pose.B_from_A
        elif pose.frame_A == frame_B and pose.frame_B == frame_A:
            print("NOT IMPLEMENTED, please swap your frames")
            return None
