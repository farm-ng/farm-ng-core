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

#include "farm_ng/core/proto_conv/geometry/conv.h"

#include "farm_ng/core/proto_conv/linalg/conv.h"

namespace farm_ng {

template <>
auto fromProt<core::proto::UnitVec3F64>(core::proto::UnitVec3F64 const& proto)
    -> Expected<sophus::UnitVector3F64> {
  FARM_TRY(auto, vec3, fromProt(proto.vec3()));
  return sophus::UnitVector3F64::tryFromUnitVector(vec3);
}

template <>
auto toProt<sophus::UnitVector3F64>(sophus::UnitVector3F64 const& uvec)
    -> core::proto::UnitVec3F64 {
  core::proto::UnitVec3F64 proto;
  *proto.mutable_vec3() = toProt(uvec.params());
  return proto;
}

template <>
auto fromProt<core::proto::Hyperplane3F64>(
    core::proto::Hyperplane3F64 const& proto)
    -> Expected<Eigen::Hyperplane<double, 3>> {
  SOPHUS_TRY(sophus::UnitVector3F64, normal, fromProt(proto.normal()));
  return Eigen::Hyperplane<double, 3>{normal.params(), proto.offset()};
}

template <>
auto toProt<Eigen::Hyperplane<double, 3>>(
    Eigen::Hyperplane<double, 3> const& plane) -> core::proto::Hyperplane3F64 {
  core::proto::Hyperplane3F64 proto;
  *proto.mutable_normal() =
      toProt(sophus::UnitVector3F64::fromVectorAndNormalize(plane.normal()));
  proto.set_offset(plane.offset());
  return proto;
}

}  // namespace farm_ng
