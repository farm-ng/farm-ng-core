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

#include "farm_ng/core/proto_conv/lie/conv.h"

#include "farm_ng/core/proto_conv/linalg/conv.h"

namespace farm_ng::core {

sophus::QuaternionF64 fromProto(proto::QuaternionF64 const& proto) {
  sophus::QuaternionF64 quat;
  quat.imag() = fromProto(proto.imag());
  quat.real() = proto.real();
  return quat;
}

proto::QuaternionF64 toProto(sophus::QuaternionF64 const& quat) {
  proto::QuaternionF64 proto;
  proto.set_real(quat.real());
  *proto.mutable_imag() = toProto(quat.imag().eval());
  return proto;
}

sophus::Rotation2F64 fromProto(proto::Rotation2F64 const& proto) {
  return sophus::Rotation2F64(proto.theta());
}

proto::Rotation2F64 toProto(sophus::Rotation2F64 const& rotation) {
  proto::Rotation2F64 proto;
  proto.set_theta(rotation.log()[0]);
  return proto;
}

sophus::Isometry2F64 fromProto(proto::Isometry2F64 const& proto) {
  return sophus::Isometry2F64(
      fromProto(proto.translation()), fromProto(proto.rotation()));
}

proto::Isometry2F64 toProto(sophus::Isometry2F64 const& pose) {
  proto::Isometry2F64 proto;
  *proto.mutable_rotation() = toProto(pose.rotation());
  *proto.mutable_translation() = toProto(pose.translation().eval());
  return proto;
}

Expected<sophus::Rotation3F64> fromProto(proto::Rotation3F64 const& proto) {
  sophus::QuaternionF64 quat = fromProto(proto.unit_quaternion());
  static double constexpr kEps = 1e-6;
  if (std::abs(quat.squaredNorm() - 1.0) > kEps) {
    return SOPHUS_UNEXPECTED(
        "quaternion norm ({}) is not close to 1:\n{}",
        quat.squaredNorm(),
        quat.params().transpose());
  }

  return sophus::Rotation3F64::fromUnitQuaternion(quat);
}

proto::Rotation3F64 toProto(sophus::Rotation3F64 const& rotation) {
  proto::Rotation3F64 proto;
  *proto.mutable_unit_quaternion() = toProto(rotation.unitQuaternion());
  return proto;
}

Expected<sophus::Isometry3F64> fromProto(proto::Isometry3F64 const& proto) {
  SOPHUS_TRY(sophus::Rotation3F64, rotation, fromProto(proto.rotation()));
  return sophus::Isometry3(fromProto(proto.translation()), rotation);
}

proto::Isometry3F64 toProto(sophus::Isometry3F64 const& pose) {
  proto::Isometry3F64 proto;
  *proto.mutable_rotation() = toProto(pose.rotation());
  *proto.mutable_translation() = toProto(pose.translation().eval());
  return proto;
}

}  // namespace farm_ng::core
