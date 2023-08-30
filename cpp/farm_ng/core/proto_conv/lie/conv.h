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
#include "sophus/lie/isometry2.h"
#include "sophus/lie/isometry3.h"
#include "sophus/lie/rotation2.h"
#include "sophus/lie/rotation3.h"

namespace farm_ng::core {

sophus::QuaternionF64 fromProto(proto::QuaternionF64 const& proto);
proto::QuaternionF64 toProto(sophus::QuaternionF64 const& quat);

sophus::Rotation2<double> fromProto(proto::Rotation2F64 const& proto);
proto::Rotation2F64 toProto(sophus::Rotation2<double> const& rotation);

sophus::Isometry2<double> fromProto(proto::Isometry2F64 const& proto);
proto::Isometry2F64 toProto(sophus::Isometry2<double> const& pose);

Expected<sophus::Rotation3<double>> fromProto(proto::Rotation3F64 const& proto);
proto::Rotation3F64 toProto(sophus::Rotation3<double> const& rotation);

Expected<sophus::Isometry3<double>> fromProto(proto::Isometry3F64 const& proto);
proto::Isometry3F64 toProto(sophus::Isometry3<double> const& pose);

}  // namespace farm_ng::core
