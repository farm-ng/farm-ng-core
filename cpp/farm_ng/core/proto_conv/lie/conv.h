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

#include "farm_ng/core/lie.pb.h"
#include "farm_ng/core/proto_conv/std/conv.h"
#include "farm_ng/core/proto_conv/traits.h"
#include "sophus/lie/isometry2.h"
#include "sophus/lie/isometry3.h"
#include "sophus/lie/rotation2.h"
#include "sophus/lie/rotation3.h"

#include <vector>

namespace farm_ng {

FARM_PROTO_CONV_TRAIT(sophus::QuaternionF64, core::proto::QuaternionF64);
FARM_PROTO_CONV_TRAIT(sophus::Rotation2F64, core::proto::Rotation2F64);
FARM_PROTO_CONV_TRAIT(sophus::Isometry2F64, core::proto::Isometry2F64);
FARM_PROTO_CONV_TRAIT(sophus::Rotation3F64, core::proto::Rotation3F64);
FARM_PROTO_CONV_TRAIT(sophus::Isometry3F64, core::proto::Isometry3F64);

FARM_PROTO_CONV_TRAIT(
    std::optional<sophus::Isometry3F64>, core::proto::OptionalG0Isometry3F64);
FARM_PROTO_CONV_TRAIT(
    std::vector<sophus::Isometry3F64>, core::proto::RepeatedG0Isometry3F64);

}  // namespace farm_ng
