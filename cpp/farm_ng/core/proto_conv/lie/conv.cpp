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

namespace farm_ng {

template <>
auto fromProt<core::proto::QuaternionF64>(
    core::proto::QuaternionF64 const& proto)
    -> Expected<sophus::QuaternionF64> {
  sophus::QuaternionF64 quat;
  FARM_TRY(auto, vec3, fromProt(proto.imag()));
  quat.imag() = vec3;
  quat.real() = proto.real();
  return quat;
}

template <>
auto toProt<sophus::QuaternionF64>(sophus::QuaternionF64 const& quat)
    -> core::proto::QuaternionF64 {
  core::proto::QuaternionF64 proto;
  proto.set_real(quat.real());
  *proto.mutable_imag() = toProt(quat.imag().eval());
  return proto;
}

template <>
auto fromProt<core::proto::Rotation2F64>(core::proto::Rotation2F64 const& proto)
    -> Expected<sophus::Rotation2F64> {
  return sophus::Rotation2F64(proto.theta());
}

template <>
auto toProt<sophus::Rotation2F64>(sophus::Rotation2F64 const& rotation)
    -> core::proto::Rotation2F64 {
  core::proto::Rotation2F64 proto;
  proto.set_theta(rotation.log()[0]);
  return proto;
}

template <>
auto fromProt<core::proto::Isometry2F64>(core::proto::Isometry2F64 const& proto)
    -> Expected<sophus::Isometry2F64> {
  FARM_TRY(auto, translation, fromProt(proto.translation()));
  FARM_TRY(auto, rotation, fromProt(proto.rotation()));
  return sophus::Isometry2F64(translation, rotation);
}

template <>
auto toProt<sophus::Isometry2F64>(sophus::Isometry2F64 const& pose)
    -> core::proto::Isometry2F64 {
  core::proto::Isometry2F64 proto;
  *proto.mutable_rotation() = toProt(pose.rotation());
  *proto.mutable_translation() = toProt(pose.translation().eval());
  return proto;
}

template <>
auto fromProt<core::proto::Rotation3F64>(core::proto::Rotation3F64 const& proto)
    -> Expected<sophus::Rotation3F64> {
  FARM_TRY(auto, quat, fromProt(proto.unit_quaternion()));
  auto valid =
      sophus::lie::Rotation3Impl<double>::areParamsValid(quat.params());
  if (!valid) {
    return FARM_UNEXPECTED("{}", valid.error());
  }
  return sophus::Rotation3F64::fromUnitQuaternion(quat);
}

template <>
auto toProt<sophus::Rotation3F64>(sophus::Rotation3F64 const& rotation)
    -> core::proto::Rotation3F64 {
  core::proto::Rotation3F64 proto;
  *proto.mutable_unit_quaternion() = toProt(rotation.unitQuaternion());
  return proto;
}

template <>
auto fromProt<core::proto::Isometry3F64>(core::proto::Isometry3F64 const& proto)
    -> Expected<sophus::Isometry3F64> {
  FARM_TRY(auto, rotation, fromProt(proto.rotation()));
  FARM_TRY(auto, translation, fromProt(proto.translation()));
  return sophus::Isometry3F64(translation, rotation);
}

template <>
auto toProt<sophus::Isometry3F64>(sophus::Isometry3F64 const& pose)
    -> core::proto::Isometry3F64 {
  core::proto::Isometry3F64 proto;
  *proto.mutable_rotation() = toProt(pose.rotation());
  *proto.mutable_translation() = toProt(pose.translation().eval());
  return proto;
}

}  // namespace farm_ng
