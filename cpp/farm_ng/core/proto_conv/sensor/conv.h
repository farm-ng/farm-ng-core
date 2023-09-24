//    Copyright 2022, farm-ng inc.
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#pragma once

#include "farm_ng/core/proto_conv/traits.h"
#include "farm_ng/core/sensor.pb.h"
#include "sophus/sensor/camera_model.h"
#include "sophus/sensor/camera_rig.h"

namespace farm_ng {

FARM_PROTO_CONV_TRAIT(sophus::CameraModel, core::proto::CameraModel);
FARM_PROTO_CONV_TRAIT(
    std::vector<sophus::CameraModel>, core::proto::CameraModels);
FARM_PROTO_CONV_TRAIT(sophus::RigidCamera, core::proto::RigidCamera);

}  // namespace farm_ng
