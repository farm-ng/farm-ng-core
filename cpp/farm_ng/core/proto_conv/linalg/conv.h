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

#include "farm_ng/core/linalg.pb.h"
#include "farm_ng/core/logging/expected.h"
#include "farm_ng/core/proto_conv/proto_conv.h"

#include <Eigen/Core>

namespace farm_ng {

FARM_PROTO_CONV_TRAIT(Eigen::Vector2i, core::proto::Vec2I64);

FARM_PROTO_CONV_TRAIT(Eigen::Vector2f, core::proto::Vec2F32);

FARM_PROTO_CONV_TRAIT(Eigen::Vector2d, core::proto::Vec2F64);

FARM_PROTO_CONV_TRAIT(Eigen::Vector3i, core::proto::Vec3I64);

FARM_PROTO_CONV_TRAIT(Eigen::Vector3f, core::proto::Vec3F32);

FARM_PROTO_CONV_TRAIT(Eigen::Vector3d, core::proto::Vec3F64);

FARM_PROTO_CONV_TRAIT(Eigen::Matrix2f, core::proto::Mat2F32);

FARM_PROTO_CONV_TRAIT(Eigen::Matrix2d, core::proto::Mat2F64);

FARM_PROTO_CONV_TRAIT(Eigen::Matrix3f, core::proto::Mat3F32);

FARM_PROTO_CONV_TRAIT(Eigen::Matrix3d, core::proto::Mat3F64);

}  // namespace farm_ng
