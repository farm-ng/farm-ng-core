// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sophus/lie/se3.h"
#include "sophus/sensor/camera_model.h"

namespace sophus {

/// Camera attached to a rigid body. We call the frame of the rigid body "rig".
struct RigidCamera {
  /// Camera intrinsics
  CameraModel intrinsics;

  /// Camera extrinsics
  sophus::SE3d rig_from_camera;
};

}  // namespace sophus
