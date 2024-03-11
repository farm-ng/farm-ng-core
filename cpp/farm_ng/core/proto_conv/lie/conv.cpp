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
#include "farm_ng/core/proto_conv/std/optional_message_impl_macro.ipp"
#include "farm_ng/core/proto_conv/std/repeated_message_impl_macro.ipp"

namespace farm_ng {

template <>
auto fromProt<core::proto::QuaternionF64>(
    core::proto::QuaternionF64 const& proto)
    -> Expected<sophus2::QuaternionF64> {
  sophus2::QuaternionF64 quat;
  FARM_TRY(auto, vec3, fromProt(proto.imag()));
  quat.imag() = vec3;
  quat.real() = proto.real();
  return quat;
}

template <>
auto toProt<sophus2::QuaternionF64>(sophus2::QuaternionF64 const& quat)
    -> core::proto::QuaternionF64 {
  core::proto::QuaternionF64 proto;
  proto.set_real(quat.real());
  *proto.mutable_imag() = toProt(quat.imag().eval());
  return proto;
}

template <>
auto fromProt<core::proto::Rotation2F64>(core::proto::Rotation2F64 const& proto)
    -> Expected<sophus2::Rotation2F64> {
  return sophus2::Rotation2F64(proto.theta());
}

template <>
auto toProt<sophus2::Rotation2F64>(sophus2::Rotation2F64 const& rotation)
    -> core::proto::Rotation2F64 {
  core::proto::Rotation2F64 proto;
  proto.set_theta(rotation.log()[0]);
  return proto;
}

template <>
auto fromProt<core::proto::Isometry2F64>(core::proto::Isometry2F64 const& proto)
    -> Expected<sophus2::Isometry2F64> {
  FARM_TRY(auto, translation, fromProt(proto.translation()));
  FARM_TRY(auto, rotation, fromProt(proto.rotation()));
  return sophus2::Isometry2F64(translation, rotation);
}

template <>
auto toProt<sophus2::Isometry2F64>(sophus2::Isometry2F64 const& pose)
    -> core::proto::Isometry2F64 {
  core::proto::Isometry2F64 proto;
  *proto.mutable_rotation() = toProt(pose.rotation());
  *proto.mutable_translation() = toProt(pose.translation().eval());
  return proto;
}

template <>
auto fromProt<core::proto::Rotation3F64>(core::proto::Rotation3F64 const& proto)
    -> Expected<sophus2::Rotation3F64> {
  FARM_TRY(auto, quat, fromProt(proto.unit_quaternion()));
  auto valid =
      sophus2::lie::Rotation3Impl<double>::areParamsValid(quat.params());
  if (!valid) {
    return FARM_UNEXPECTED("{}", valid.error());
  }
  return sophus2::Rotation3F64::fromUnitQuaternion(quat);
}

template <>
auto toProt<sophus2::Rotation3F64>(sophus2::Rotation3F64 const& rotation)
    -> core::proto::Rotation3F64 {
  core::proto::Rotation3F64 proto;
  *proto.mutable_unit_quaternion() = toProt(rotation.unitQuaternion());
  return proto;
}

template <>
auto fromProt<core::proto::Isometry3F64>(core::proto::Isometry3F64 const& proto)
    -> Expected<sophus2::Isometry3F64> {
  FARM_TRY(auto, rotation, fromProt(proto.rotation()));
  FARM_TRY(auto, translation, fromProt(proto.translation()));
  return sophus2::Isometry3F64(translation, rotation);
}

template <>
auto toProt<sophus2::Isometry3F64>(sophus2::Isometry3F64 const& pose)
    -> core::proto::Isometry3F64 {
  core::proto::Isometry3F64 proto;
  *proto.mutable_rotation() = toProt(pose.rotation());
  *proto.mutable_translation() = toProt(pose.translation().eval());
  return proto;
}

FARM_CONV_IMPL_MESSAGE_OPTIONAL(
    core::proto::OptionalG0Isometry3F64, sophus2::Isometry3F64);

FARM_CONV_IMPL_REPEATED_MESSAGE(
    core::proto::RepeatedG0Isometry3F64, std::vector<sophus2::Isometry3F64>);

}  // namespace farm_ng
