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

namespace farm_ng::core {

Eigen::Matrix<uint32_t, 2, 1> fromProto(proto::Vec2I64 const& proto) {
  return Eigen::Matrix<uint32_t, 2, 1>(proto.x(), proto.y());
}

proto::Vec2I64 toProto(Eigen::Matrix<uint32_t, 2, 1> const& v) {
  proto::Vec2I64 proto;
  proto.set_x(v.x());
  proto.set_y(v.y());
  return proto;
}

Eigen::Vector2f fromProto(proto::Vec2F32 const& proto) {
  return Eigen::Vector2f(proto.x(), proto.y());
}

proto::Vec2F32 toProto(Eigen::Vector2f const& v) {
  proto::Vec2F32 proto;
  proto.set_x(v.x());
  proto.set_y(v.y());
  return proto;
}

Eigen::Vector2d fromProto(proto::Vec2F64 const& proto) {
  return Eigen::Vector2d(proto.x(), proto.y());
}

proto::Vec2F64 toProto(Eigen::Vector2d const& v) {
  proto::Vec2F64 proto;
  proto.set_x(v.x());
  proto.set_y(v.y());
  return proto;
}

Eigen::Matrix<uint32_t, 3, 1> fromProto(proto::Vec3I64 const& proto) {
  return Eigen::Matrix<uint32_t, 3, 1>(proto.x(), proto.y(), proto.z());
}

proto::Vec3I64 toProto(Eigen::Matrix<uint32_t, 3, 1> const& v) {
  proto::Vec3I64 proto;
  proto.set_x(v.x());
  proto.set_y(v.y());
  proto.set_z(v.z());
  return proto;
}

Eigen::Vector3f fromProto(proto::Vec3F32 const& proto) {
  return Eigen::Vector3f(proto.x(), proto.y(), proto.z());
}

proto::Vec3F32 toProto(Eigen::Vector3f const& v) {
  proto::Vec3F32 proto;
  proto.set_x(v.x());
  proto.set_y(v.y());
  proto.set_z(v.z());
  return proto;
}

Eigen::Vector3d fromProto(proto::Vec3F64 const& proto) {
  return Eigen::Vector3d(proto.x(), proto.y(), proto.z());
}

proto::Vec3F64 toProto(Eigen::Vector3d const& v) {
  proto::Vec3F64 proto;
  proto.set_x(v.x());
  proto.set_y(v.y());
  proto.set_z(v.z());
  return proto;
}

Eigen::Matrix2f fromProto(proto::Mat2F32 const& proto) {
  Eigen::Matrix2f m;
  m.col(0) = fromProto(proto.col_0());
  m.col(1) = fromProto(proto.col_1());
  return m;
}
proto::Mat2F32 toProto(Eigen::Matrix2f const& v) {
  proto::Mat2F32 proto;
  *proto.mutable_col_0() = toProto(v.col(0).eval());
  *proto.mutable_col_1() = toProto(v.col(1).eval());
  return proto;
}

Eigen::Matrix2d fromProto(proto::Mat2F64 const& proto) {
  Eigen::Matrix2d m;
  m.col(0) = fromProto(proto.col_0());
  m.col(1) = fromProto(proto.col_1());
  return m;
}
proto::Mat2F64 toProto(Eigen::Matrix2d const& v) {
  proto::Mat2F64 proto;
  *proto.mutable_col_0() = toProto(v.col(0).eval());
  *proto.mutable_col_1() = toProto(v.col(1).eval());
  return proto;
}

Eigen::Matrix3f fromProto(proto::Mat3F32 const& proto) {
  Eigen::Matrix3f m;
  m.col(0) = fromProto(proto.col_0());
  m.col(1) = fromProto(proto.col_1());
  m.col(2) = fromProto(proto.col_2());
  return m;
}
proto::Mat3F32 toProto(Eigen::Matrix3f const& v) {
  proto::Mat3F32 proto;
  *proto.mutable_col_0() = toProto(v.col(0).eval());
  *proto.mutable_col_1() = toProto(v.col(1).eval());
  *proto.mutable_col_2() = toProto(v.col(2).eval());
  return proto;
}

Eigen::Matrix3d fromProto(proto::Mat3F64 const& proto) {
  Eigen::Matrix3d m;
  m.col(0) = fromProto(proto.col_0());
  m.col(1) = fromProto(proto.col_1());
  m.col(2) = fromProto(proto.col_2());
  return m;
}
proto::Mat3F64 toProto(Eigen::Matrix3d const& v) {
  proto::Mat3F64 proto;
  *proto.mutable_col_0() = toProto(v.col(0).eval());
  *proto.mutable_col_1() = toProto(v.col(1).eval());
  *proto.mutable_col_2() = toProto(v.col(2).eval());
  return proto;
}

}  // namespace farm_ng::core
