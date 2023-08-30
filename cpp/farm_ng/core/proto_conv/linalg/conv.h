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

#include <Eigen/Core>

namespace farm_ng::core {

Eigen::Matrix<uint32_t, 2, 1> fromProto(proto::Vec2I64 const& proto);
proto::Vec2I64 toProto(Eigen::Matrix<uint32_t, 2, 1> const& v);

Eigen::Vector2f fromProto(proto::Vec2F32 const& proto);
proto::Vec2F32 toProto(Eigen::Vector2f const& v);

Eigen::Vector2d fromProto(proto::Vec2F64 const& proto);
proto::Vec2F64 toProto(Eigen::Vector2d const& v);

Eigen::Matrix<uint32_t, 3, 1> fromProto(proto::Vec3I64 const& proto);
proto::Vec3I64 toProto(Eigen::Matrix<uint32_t, 3, 1> const& v);

Eigen::Vector3f fromProto(proto::Vec3F32 const& proto);
proto::Vec3F32 toProto(Eigen::Vector3f const& v);

Eigen::Vector3d fromProto(proto::Vec3F64 const& proto);
proto::Vec3F64 toProto(Eigen::Vector3d const& v);

Eigen::Matrix2f fromProto(proto::Mat2F32 const& proto);
proto::Mat2F32 toProto(Eigen::Matrix2f const& v);

Eigen::Matrix2d fromProto(proto::Mat2F64 const& proto);
proto::Mat2F64 toProto(Eigen::Matrix2d const& v);

Eigen::Matrix3f fromProto(proto::Mat3F32 const& proto);
proto::Mat3F32 toProto(Eigen::Matrix3f const& v);

Eigen::Matrix3d fromProto(proto::Mat3F64 const& proto);
proto::Mat3F64 toProto(Eigen::Matrix3d const& v);

}  // namespace farm_ng::core
