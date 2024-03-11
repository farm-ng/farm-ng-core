// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sophus2/lie/se3.h"
#include "sophus2/sensor/camera_model.h"

namespace sophus2 {

/// Camera attached to a rigid body. We call the frame of the rigid body "rig".
struct RigidCamera {
  /// Camera intrinsics
  CameraModel intrinsics;

  /// Camera extrinsics
  sophus2::SE3d rig_from_camera;
};

using MultiCameraRig = std::vector<RigidCamera>;

}  // namespace sophus2
