---
sidebar_position: 1
---

# Pose and Frame Conventions


## Rigid body transform and points convention

Given a point in frame ``foo``, ``bar_from_foo`` is the rigid body
transform which maps the point to frame ``bar``:

```cpp
    point_in_bar = bar_from_foo * point_in_foo
```

Note that the frame names line up: ``bar`` - ``bar``, ``foo`` - ``foo``.

### Some details

 - If necessary to resolved ambiguities, e.g. between rotations and rigid
   body transforms, we use a corresponding notation, e.g.
   ``bar_form_foo_rotation``, ``bar_form_foo_se3s`` etc.

 - If we have a list (or vector, or map) of transforms we write:, e.g.:

      bar_from_foo_rotations

 - Transforms with compound frame names, such as ``robot_base`` or
   ``left_camera``, are written as follows::

      robot_base_from_left_camera

 - For functions and methods we use ``camelCase``. Examples::

      this->calcRobotBaseFromLeftCamera();

      other->setBarFromFoo(bar_from_foo);

 - We use the ``entity_in_frame`` conventions for points and other entities
   which have one frame attached to it. Examples:

   * ``point_in_camera``
   * ``circle_in_image``
   * ``camera_position_in_world`` (or short ``camera_in_world``)
   * ...

  - When storing transforms / using them in interfaces, prefer the
    ``parent_from_child`` convention.

    E.g. ``world_from_sensor_rig``, ``sensor_rig_from_camera``,
    ``robot_from_imu`` (and not ``sensor_rig_from_world`` etc.).

    It is easier to reason about the pose of camera in the world frame, then
    the pose of the world origin in the camera frame.

## Velocity, acceleration convention

This section includes convention for entities which require three frames such
as velocities, and accelerations. We will focus the discussion on velocities,
but the same conventions apply to accelerations and similar entities similarly.

For velocities, the three frame of interest are:
  - the **anchor frame** we compare the motion to
  - the frame of the **entity** in motion
  - the **frame of origin** in which the velocity is expressed


Let ``foo`` by the anchor frame, ``entity`` the frame on the body in motion
and ``origin`` the origin frame.
Then we write:

```
foo_anchored_entity_velocity_in_origin
```

Let first give a few examples to illustrate velocities of rigid bodies in
general, why we need three frames to annotate velocities, and what the
difference between is between the anchor frame and the origin is:


- ``world_anchored_robot_velocity_in_robot``: Ego-centric velocity of the robot
  with respect to the world frame, expressed in the robot frame. For 2d
  isometries, the velocity or tangent vector is a 3-vector (vx, vy, w),
  which consists of linear velocity (vx, vy) - i.e. forward and sideway and
  angular rate (w). For 3d rigid body transforms, the velocity vector is a
  6-vector (vx, vy, vz, wx, wy, wz), which consists of linear velocity (vx,
  vy, vz) and angular rate (wx, wy, wz).

- ``world_anchored_robot_velocity_in_world``: Velocity of the
  robot with respect to the world frame, expressed in the world frame. Here,
  the velocities (such as the linear vx, vy) are not ego-centric
  (forward, sideways) but world-centric (e.g. North-South, East-West).

- ``car1_anchored_car2_velocity_in_track``: Relative velocity of car2 with
  respect to car1, expressed in the race track frame.

It is cumbersome, to always annotate velocities with three frames. If it is
clear from context what

  (a) the anchor frame (e.g. the world frame) and the
  (b) entity in motion is (e.g. the robot),

we use the following conventions which only annotate the origin frame:

```
velocity_in_bar
```

which is short for ``world_anchored_robot_velocity_in_bar``. In particular,
we write:

```
velocity_in_world
```

for the world-centric velocity ``world_anchored_robot_velocity_in_world ``
(robot is moving WEST with a speed of 5 mps), and

```
velocity_in_robot
```

for the egocentric velocity ``world_anchored_robot_velocity_in_robot`` (robot is
moving forward with a speed of 5 mps).
