# farm-ng-core
<h1 align="center"> farm-ng-core </h1>

<p align="center">
  <i> Foundational library for robotics and machine sensing application
      under active development </i>
</p>

<div align="center">

<a href="https://github.com/farm-ng/farm-ng-core/actions/workflows/ci.yml">
  <img src="https://github.com/farm-ng/farm-ng-core/actions/workflows/ci.yml/badge.svg"
       alt="C++ Build CI Badge"/>
</a>

<a href="https://pypi.org/project/farm-ng-core">
  <img src="https://badge.fury.io/py/farm-ng-core.svg"
       alt="PyPI version"/>
</a>

<a href="https://github.com/farm-ng/farm-ng-core/actions/workflows/format.yml">
  <img src="https://github.com/farm-ng/farm-ng-core/actions/workflows/format.yml/badge.svg"
       alt="Format CI Badge"/>
</a>
<a href="https://github.com/farm-ng/farm-ng-core/actions/workflows/docs.yml">
  <img src="https://github.com/farm-ng/farm-ng-core/actions/workflows/docs.yml/badge.svg"
       alt="Docs CI Badge"/>
</a>
</div>

<br>
<p align="center"><i> for c++, python and soon rust - under active development </i>
</p>
<br>


- [farm-ng-core](#farm-ng-core)
  - [👟Getting Started](#getting-started)
  - [Core layer](#core-layer)
  - [Sophus 2: Building Blocks for 2D and 3D Geometry](#sophus-2-building-blocks-for-2d-and-3d-geometry)
    - [🌐Lie Groups](#lie-groups)
    - [🌁Image classes, Sensor Models and more](#image-classes-sensor-models-and-more)
  - [Component Pipeline](#component-pipeline)
  - [Logging/Serialization Infrastructure](#loggingserialization-infrastructure)

## 👟Getting Started

Check out the getting-started docs:

[here](docs/docs/intro.md)

or

[here](https://farm-ng.github.io/farm-ng-core/docs/intro)




## Core layer

Convenient macros such as FARM_ENUM, FARM_ASSERT_* and FARM_UNWRAP.

Standard Library-like genera purpose utilities.

farm-ng-core only has a small set of dependencies: libfmt, expected
(both being targeted for c++ standardization) and Protocol Buffers /
GRPC.

## Sophus 2: Building Blocks for 2D and 3D Geometry

Sophus started as a c++ implementation of Lie Groups / Manifolds. It evolved to a
collection of types and functions commonly used or 2d and 3d geometric problems
especially in the domain of `robotics`, `computer vision` annd `graphics`.

### 🌐Lie Groups


#### tldr: rotations, translations and scaling in 2d and 3d

`Lie groups` are generalizations of the Euclidean vector spaces R^N. A little
more formally, a Manifold which is also an [abstract group](https://en.wikipedia.org/wiki/Group_theory#Abstract_groups).

*Okay, and what is a Manifold?*

`Manifold` are generalizations of the Euclidean vector spaces R^N. In
particular, they behave locally like a Euclidean vector space, but globally
can have a very different structures. In particular there can be wrap-around.
The circle group SO(2) is the simplest example for such wrap around. Assume,
we have a dial pointing North. If you turn the dial 90 degree to the left, it
points West. If you turn it another 90 degrees it turns South. Now, if you turn
it again 90 degrees is points East. And you turn it left again for 90 degrees it
points North again. It wrapped around: `90 "+" 90 "+" 90 "+" 90 = 0`.

#### 3d rotation example using the SO(3) type

```c++
  // The following demonstrates the group multiplication of rotation matrices

  // Create rotation matrices from rotations around the x and y and z axes:
  double const kPi = sophus::kPi<double>;
  sophus::Rotation3F64 R1 = sophus::Rotation3F64::fromRx(kPi / 4);
  sophus::Rotation3F64 R2 = sophus::Rotation3F64::fromRy(kPi / 6);
  sophus::Rotation3F64 R3 = sophus::Rotation3F64::fromRz(-kPi / 3);

  std::cout << "The rotation matrices are" << std::endl;
  std::cout << "R1:\n" << R1.matrix() << std::endl;
  std::cout << "R2:\n" << R2.matrix() << std::endl;
  std::cout << "R3:\n" << R3.matrix() << std::endl;
  std::cout << "Their product R1*R2*R3:\n"
            << (R1 * R2 * R3).matrix() << std::endl;
  std::cout << std::endl;

  // Rotation matrices can act on vectors
  Eigen::Vector3d x;
  x << 0.0, 0.0, 1.0;
  std::cout << "Rotation matrices can act on 3-vectors" << std::endl;
  std::cout << "x\n" << x << std::endl;
  std::cout << "R2*x\n" << R2 * x << std::endl;
  std::cout << "R1*(R2*x)\n" << R1 * (R2 * x) << std::endl;
  std::cout << "(R1*R2)*x\n" << (R1 * R2) * x << std::endl;
  std::cout << std::endl;

  // SO(3) are internally represented as unit quaternions.
  std::cout << "R1 in matrix form:\n" << R1.matrix() << std::endl;
  std::cout << "R1 in unit quaternion form:\n"
            << R1.unitQuaternion().coeffs() << std::endl;
  // Note that the order of coefficients of Eigen's quaternion class is
  // (imag0, imag1, imag2, real)
  std::cout << std::endl;
```

#### 3d rotation + translation example using the SE(3) type

```c++
  // Example of create a rigid transformation from an SO(3) = 3D rotation and a
  // translation 3-vector:

  // Let use assume there is a camera in the world. First we describe its
  // orientation in the world reference frame.
  sophus::Rotation3F64 world_from_camera_rotation =
      sophus::Rotation3F64::fromRx(sophus::kPi<double> / 4);
  // Then the position of the camera in the world.
  Eigen::Vector3d camera_in_world(0.0, 0.0, 1.0);

  // The pose (position and orientation) of the camera in the world is
  // constructed by its orientation ``world_from_camera_rotation`` as well as
  // its position ``camera_in_world``.
  sophus::Isometry3F64 world_anchored_camera_pose(
      world_from_camera_rotation, camera_in_world);

  // SE(3) naturally representation is a 4x4 matrix which can be accessed using
  // the .matrix() method:
  std::cout << "world_anchored_camera_pose:\n"
            << world_anchored_camera_pose.matrix() << std::endl;
```

#### Table of Lie Groups

The following table gives an overview of all Lie Groups in Sophus.

| c++ type                                      | Lie group name                                       | Description                                                                                                |
| ----------------------------------------------|------------------------------------------------------| ---------------------------------------------------------------------------------------------------------- |
| [`Rotation2<T>`](cpp/sophus/lie/so2.h)              | Special Orthogonal Group in 2D, SO(2)                | rotations in 2d, also called Circle Group, or just "angle"                                                 |
| [`Rotation3<T>`](cpp/sophus/lie/so3.h)              | Special Orthogonal Group in 3D, SO(3)                | rotations in 3d, 3D orientations                                                                           |
| [`Isometry2<T>`](cpp/sophus/lie/se2.h)              | Special Euclidean Group in 2D, SE(3)                 | rotations and translations in 2D, also called 2D rigid body transformations, 2d poses, plane isometries    |
| [`Isometry3<T>`](cpp/sophus/lie/se3.h)              | Special Euclidean Group in 3D, SE(3)                 | rotations and translations in 3D, also called rigid body transformations,6 DoF poses, Euclidean isometries |
| [`RxSo2<T>`](cpp/sophus/lie/rxso2.h)          | Direct product of SO(3) and scalar matrix, R x SO(2) | scaling and rotations in 2D                                                                                |
| [`RxSo3<T>`](cpp/sophus/lie/rxso3.h)          | Direct product of SO(3) and scalar matrix  R x SO(3) | scaling and rotations in 3D                                                                                |
| [`Similarity2<T>`](cpp/sophus/lie/sim2.h)            | Similarity Group in 2D, Sim(2)                       | scaling, rotations and translation in 2D                                                                   |
| [`Similarity3<T>`](cpp/sophus/lie/sim3.h)            | Similarity Group in 3D, Sim(3)                       | scaling, rotations and translation in 3D                                                                   |
| [`Cartesian2<T>`](cpp/sophus/lie/cartesian.h) | 2D Euclidean Vector Space, R^2                       | all vector spaces are trivial Lie groups, also called 2d translation group, the translation part of SE(2)  |
| [`Cartesian3<T>`](cpp/sophus/lie/cartesian.h) | 3D Euclidean Vector Space, R^3                       | all vector spaces are trivial Lie groups, also called 3d translation group, the translation part of SE(2)  |

Supported advanced features on Lie groups:

- ✅ (linear) interpolation
- ✅ Spline interpolation
- ✅ Averaging (of more than two elements)


### 🌁Image classes, Sensor Models and more

Image Classes: Image, MutImage, DynImage, MutDynImage and view classes.

Collection of camera models (pinhole, brown-conrady aka opencv,
kannala-brandt and orthographic), IMU mode and more.

## Component Pipeline

C++ Component pipeline (aka actor framework) for easy parallelization of data
processing pipelines.

## Logging/Serialization Infrastructure

 - Text Logging

 - Protobuf Logging

 - RPCs
