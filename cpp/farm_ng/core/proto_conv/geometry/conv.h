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

#include "farm_ng/core/geometry.pb.h"
#include "farm_ng/core/proto_conv/proto_conv.h"
#include "sophus/geometry/ray.h"

namespace farm_ng {

using Hyperplane3d = Eigen::Hyperplane<double, 3>;

FARM_PROTO_CONV_TRAIT(sophus::UnitVector3F64, core::proto::UnitVec3F64);
FARM_PROTO_CONV_TRAIT(Hyperplane3d, core::proto::Hyperplane3F64);

}  // namespace farm_ng
