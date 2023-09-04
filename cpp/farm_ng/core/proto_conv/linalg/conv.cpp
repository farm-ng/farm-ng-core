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

#include "farm_ng/core/proto_conv/linalg/conv.h"

namespace farm_ng {

template <>
auto fromProt<core::proto::Vec2I64>(core::proto::Vec2I64 const& proto)
    -> Expected<Eigen::Vector2i> {
  return Eigen::Vector2i(proto.x(), proto.y());
}

template <>
auto toProt<Eigen::Vector2i>(Eigen::Vector2i const& v) -> core::proto::Vec2I64 {
  core::proto::Vec2I64 proto;
  proto.set_x(v.x());
  proto.set_y(v.y());
  return proto;
}

template <>
auto fromProt<core::proto::Vec2F32>(core::proto::Vec2F32 const& proto)
    -> Expected<Eigen::Vector2f> {
  return Eigen::Vector2f(proto.x(), proto.y());
}

template <>
auto toProt<Eigen::Vector2f>(Eigen::Vector2f const& v) -> core::proto::Vec2F32 {
  core::proto::Vec2F32 proto;
  proto.set_x(v.x());
  proto.set_y(v.y());
  return proto;
}

template <>
auto fromProt<core::proto::Vec2F64>(core::proto::Vec2F64 const& proto)
    -> Expected<Eigen::Vector2d> {
  return Eigen::Vector2d(proto.x(), proto.y());
}

template <>
auto toProt<Eigen::Vector2d>(Eigen::Vector2d const& v) -> core::proto::Vec2F64 {
  core::proto::Vec2F64 proto;
  proto.set_x(v.x());
  proto.set_y(v.y());
  return proto;
}

template <>
auto fromProt<core::proto::Vec3I64>(core::proto::Vec3I64 const& proto)
    -> Expected<Eigen::Vector3i> {
  return Eigen::Vector3i(proto.x(), proto.y(), proto.z());
}

template <>
auto toProt<Eigen::Vector3i>(Eigen::Vector3i const& v) -> core::proto::Vec3I64 {
  core::proto::Vec3I64 proto;
  proto.set_x(v.x());
  proto.set_y(v.y());
  proto.set_z(v.z());
  return proto;
}

template <>
auto fromProt<core::proto::Vec3F32>(core::proto::Vec3F32 const& proto)
    -> Expected<Eigen::Vector3f> {
  return Eigen::Vector3f(proto.x(), proto.y(), proto.z());
}

template <>
auto toProt<Eigen::Vector3f>(Eigen::Vector3f const& v) -> core::proto::Vec3F32 {
  core::proto::Vec3F32 proto;
  proto.set_x(v.x());
  proto.set_y(v.y());
  proto.set_z(v.z());
  return proto;
}

template <>
auto fromProt<core::proto::Vec3F64>(core::proto::Vec3F64 const& proto)
    -> Expected<Eigen::Vector3d> {
  return Eigen::Vector3d(proto.x(), proto.y(), proto.z());
}

template <>
auto toProt<Eigen::Vector3d>(Eigen::Vector3d const& v) -> core::proto::Vec3F64 {
  core::proto::Vec3F64 proto;
  proto.set_x(v.x());
  proto.set_y(v.y());
  proto.set_z(v.z());
  return proto;
}

template <>
auto fromProt<core::proto::Mat2F32>(core::proto::Mat2F32 const& proto)
    -> Expected<Eigen::Matrix2f> {
  FARM_TRY(auto, col0, fromProt(proto.col_0()));
  FARM_TRY(auto, col1, fromProt(proto.col_1()));

  Eigen::Matrix2f m;
  m.col(0) = col0;
  m.col(1) = col1;
  return m;
}

template <>
auto toProt<Eigen::Matrix2f>(Eigen::Matrix2f const& v) -> core::proto::Mat2F32 {
  core::proto::Mat2F32 proto;
  *proto.mutable_col_0() = toProt<Eigen::Vector2f>(v.col(0));
  *proto.mutable_col_1() = toProt<Eigen::Vector2f>(v.col(1));
  return proto;
}

template <>
auto fromProt<core::proto::Mat2F64>(core::proto::Mat2F64 const& proto)
    -> Expected<Eigen::Matrix2d> {
  FARM_TRY(auto, col0, fromProt(proto.col_0()));
  FARM_TRY(auto, col1, fromProt(proto.col_1()));

  Eigen::Matrix2d m;
  m.col(0) = col0;
  m.col(1) = col1;
  return m;
}

template <>
auto toProt<Eigen::Matrix2d>(Eigen::Matrix2d const& v) -> core::proto::Mat2F64 {
  core::proto::Mat2F64 proto;
  *proto.mutable_col_0() = toProt<Eigen::Vector2d>(v.col(0));
  *proto.mutable_col_1() = toProt<Eigen::Vector2d>(v.col(1));
  return proto;
}

template <>
auto fromProt<core::proto::Mat3F32>(core::proto::Mat3F32 const& proto)
    -> Expected<Eigen::Matrix3f> {
  FARM_TRY(auto, col0, fromProt(proto.col_0()));
  FARM_TRY(auto, col1, fromProt(proto.col_1()));
  FARM_TRY(auto, col2, fromProt(proto.col_2()));

  Eigen::Matrix3f m;
  m.col(0) = col0;
  m.col(1) = col1;
  m.col(2) = col2;
  return m;
}

template <>
auto toProt<Eigen::Matrix3f>(Eigen::Matrix3f const& v) -> core::proto::Mat3F32 {
  core::proto::Mat3F32 proto;
  *proto.mutable_col_0() = toProt<Eigen::Vector3f>(v.col(0));
  *proto.mutable_col_1() = toProt<Eigen::Vector3f>(v.col(1));
  *proto.mutable_col_2() = toProt<Eigen::Vector3f>(v.col(2));
  return proto;
}

template <>
auto fromProt<core::proto::Mat3F64>(core::proto::Mat3F64 const& proto)
    -> Expected<Eigen::Matrix3d> {
  FARM_TRY(auto, col0, fromProt(proto.col_0()));
  FARM_TRY(auto, col1, fromProt(proto.col_1()));
  FARM_TRY(auto, col2, fromProt(proto.col_2()));

  Eigen::Matrix3d m;
  m.col(0) = col0;
  m.col(1) = col1;
  m.col(2) = col2;
  return m;
}

template <>
auto toProt<Eigen::Matrix3d>(Eigen::Matrix3d const& v) -> core::proto::Mat3F64 {
  core::proto::Mat3F64 proto;
  *proto.mutable_col_0() = toProt<Eigen::Vector3d>(v.col(0));
  *proto.mutable_col_1() = toProt<Eigen::Vector3d>(v.col(1));
  *proto.mutable_col_2() = toProt<Eigen::Vector3d>(v.col(2));
  return proto;
}

}  // namespace farm_ng
