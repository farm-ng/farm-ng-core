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

namespace farm_ng::core {

Expected<sophus::UnitVector3F64> fromProto(proto::UnitVec3F64 const& proto) {
  return sophus::UnitVector3F64::tryFromUnitVector(fromProto(proto.vec3()));
}

proto::UnitVec3F64 toProto(sophus::UnitVector3F64 const& uvec) {
  proto::UnitVec3F64 proto;
  *proto.mutable_vec3() = toProto(uvec.params());
  return proto;
}

Expected<Eigen::Hyperplane<double, 3>> fromProto(
    proto::Hyperplane3F64 const& proto) {
  SOPHUS_TRY(sophus::UnitVector3F64, normal, fromProto(proto.normal()));
  return Eigen::Hyperplane<double, 3>{normal.params(), proto.offset()};
}

proto::Hyperplane3F64 toProto(Eigen::Hyperplane<double, 3> const& plane) {
  proto::Hyperplane3F64 proto;
  *proto.mutable_normal() =
      toProto(sophus::UnitVector3F64::fromVectorAndNormalize(plane.normal()));
  proto.set_offset(plane.offset());
  return proto;
}

}  // namespace farm_ng::core
