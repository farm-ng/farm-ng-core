---
sidebar_position: 1
---

# Transformations, points, frames and velocities


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

This is discussing the convention for velocities of points and rigid body. In
more general terms, this are entities which need to be annotated by three
frames, such velocities, accelerations, etc.

The three frame of interest are:
  - the **anchor frame** we compare the motion to
  - the frame of the **entity** in motion
  - the **frame of origin** in which the velocity is expressed


Let ``foo`` by the anchor frame, ``entity`` the frame of the entity in motion
and ``origin`` the origin frame.
Then we write:

```
foo_anchored_entity_velocity_in_origin
```

Let first give a few examples to illustrate why really three frames are needed,
and what the difference between is between the anchor frame and the origin is:


- ``world_anchored_robot_velocity_in_robot``: Velocity of the robot with respect
  to the world frame, expressed in the robot frame. Hence this is a egocentric
  velocity.

- ``world_anchored_robot_velocity_in_world``: Velocity of the robot with respect
  to the world frame, expressed in the world frame. Hence this is a world-centric
  velocity.

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


### More about the three frames involved

It is important to note that the swapping the anchor frame and the entity frame
does lead to a velocity with opposite direction. That is:

```
foo_anchored_bar_velocity_in_origin = -bar_anchored_foo_velocity_in_origin
```

This is obvious in the example of two race cars. If car1 is moving with a speed
of 5 mps with respect to car2, then car2 is moving with a speed of -5 mps with
respect to car1. (Car1 is 5mph faster than car2, or hence car2 is 5mph slower
than car1.)

We call a velocity ego-centric if the origin frame is the entity frame.

If the entity frame equals the anchor frame, then the velocity is zero by
definition. This trivial case is not interesting. (If you were to see non-zero
velocities foo_anchored_foo_velocity_in_bar in code, the notation is not used
correctly.)

Changing the origin frame is done using the adjoint map. Let ``robot`` be an entity
in motion with respect to the ``world`` frame, and have the following velocity:
``world_anchored_robot_velocity_in_robot``.  If we want to express the velocity
of the robot in a different frame, e.g. a ``sensor`` frame, we use the adjoint map:

```
world_anchored_robot_velocity_in_sensor
= sensor_from_robot.adjoint() * world_anchored_robot_velocity_in_robot
```

where ``sensor_from_robot.adjoint()`` is the adjoint map of the rigid body transform
``sensor_from_robot``. Note that applying the adjoint map only changes the
origin frame, but the other two frames (anchor and entity) stay the same.

The adjoint map can also be used to change from world-centric to egocentric
velocities. Let ``sensor`` be a body in motion with respect to the ``world``
frame, and it has the following world-centric velocity:
``world_anchored_sensor_velocity_in_world``.  If we want to express
the of velocity in the sensor frame, we use the adjoint map:

```
world_anchored_sensor_velocity_in_sensor
= sensor_from_world.adjoint() * world_anchored_sensor_velocity_in_world
```

Note, that ``world_anchored_sensor_velocity_in_sensor`` is still the velocity of
the sensor (with respect to the world frame), but expressed in the sensor frame.
A different but related quantity is the velocity of the world (with respect to
the sensor frame) expressed in the sensor frame:

```
sensor_anchored_world_velocity_in_sensor
= -(sensor_from_world.adjoint() * world_anchored_sensor_velocity_in_world)
= -world_anchored_sensor_velocity_in_sensor

```

While the concept of world velocity expressed in the sensor frame may sound
strange, it is a useful quantity. For instance to describe a sensor model (e.g.
for a lidar on a self driving car), it would make sense to talk about the
velocity of measured world points expressed in the sensor frame.
