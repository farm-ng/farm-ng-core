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

#include <gtest/gtest.h>

using namespace farm_ng;
using namespace farm_ng::core;

TEST(conv_vec, unit) {
  {
    Eigen::Vector2i v(1, 2);
    proto::Vec2I64 proto = toProt(v);
    EXPECT_EQ(proto.x(), 1);
    EXPECT_EQ(proto.y(), 2);
    auto maybe_v2 = fromProt(proto);
    Eigen::Vector2i v2 = FARM_UNWRAP(maybe_v2);
    EXPECT_EQ(v2, v);
  }

  {
    Eigen::Vector2f v(1, 2);
    proto::Vec2F32 proto = toProt(v);
    EXPECT_EQ(proto.x(), 1);
    EXPECT_EQ(proto.y(), 2);
    auto maybe_v2 = fromProt(proto);
    Eigen::Vector2f v2 = FARM_UNWRAP(maybe_v2);
    EXPECT_EQ(v2, v);
  }

  {
    Eigen::Vector2d v(5, 6);
    proto::Vec2F64 proto = toProt(v);
    EXPECT_EQ(proto.x(), 5);
    EXPECT_EQ(proto.y(), 6);
    auto maybe_v2 = fromProt(proto);
    Eigen::Vector2d v2 = FARM_UNWRAP(maybe_v2);
    EXPECT_EQ(v2, v);
  }

  {
    Eigen::Vector3i v(1, 2, 3);
    proto::Vec3I64 proto = toProt(v);
    EXPECT_EQ(proto.x(), 1);
    EXPECT_EQ(proto.y(), 2);
    EXPECT_EQ(proto.z(), 3);
    auto maybe_v2 = fromProt(proto);
    Eigen::Vector3i v2 = FARM_UNWRAP(maybe_v2);
    EXPECT_EQ(v2, v);
  }

  {
    Eigen::Vector3f v(1, 2, 3);
    proto::Vec3F32 proto = toProt(v);
    EXPECT_EQ(proto.x(), 1);
    EXPECT_EQ(proto.y(), 2);
    EXPECT_EQ(proto.z(), 3);
    auto maybe_v2 = fromProt(proto);
    Eigen::Vector3f v2 = FARM_UNWRAP(maybe_v2);
    EXPECT_EQ(v2, v);
  }

  {
    Eigen::Vector3d v(1, 2, 3);
    proto::Vec3F64 proto = toProt(v);
    EXPECT_EQ(proto.x(), 1);
    EXPECT_EQ(proto.y(), 2);
    EXPECT_EQ(proto.z(), 3);
    auto maybe_v2 = fromProt(proto);
    Eigen::Vector3d v2 = FARM_UNWRAP(maybe_v2);
    EXPECT_EQ(v2, v);
  }
}

TEST(conv_mat, unit) {
  {
    Eigen::Matrix2f v;
    v << 1, 2,  //
        3, 4;
    proto::Mat2F32 proto = toProt(v);
    EXPECT_EQ(proto.col_0().x(), 1);
    EXPECT_EQ(proto.col_1().x(), 2);
    EXPECT_EQ(proto.col_0().y(), 3);
    EXPECT_EQ(proto.col_1().y(), 4);

    auto maybe_v2 = fromProt(proto);
    Eigen::Matrix2f v2 = FARM_UNWRAP(maybe_v2);
    EXPECT_EQ(v2, v);
  }

  {
    Eigen::Matrix2d v;
    v << 1, 2,  //
        3, 4;
    proto::Mat2F64 proto = toProt(v);
    EXPECT_EQ(proto.col_0().x(), 1);
    EXPECT_EQ(proto.col_1().x(), 2);
    EXPECT_EQ(proto.col_0().y(), 3);
    EXPECT_EQ(proto.col_1().y(), 4);
    auto maybe_v2 = fromProt(proto);
    Eigen::Matrix2d v2 = FARM_UNWRAP(maybe_v2);
    EXPECT_EQ(v2, v);
  }

  {
    Eigen::Matrix3f v;
    v << 1, 2, 3,  //
        4, 5, 6,   //
        7, 8, 9;
    proto::Mat3F32 proto = toProt(v);
    EXPECT_EQ(proto.col_0().x(), 1);
    EXPECT_EQ(proto.col_1().x(), 2);
    EXPECT_EQ(proto.col_2().x(), 3);
    EXPECT_EQ(proto.col_0().y(), 4);
    EXPECT_EQ(proto.col_1().y(), 5);
    EXPECT_EQ(proto.col_2().y(), 6);
    EXPECT_EQ(proto.col_0().z(), 7);
    EXPECT_EQ(proto.col_1().z(), 8);
    EXPECT_EQ(proto.col_2().z(), 9);

    auto maybe_v2 = fromProt(proto);
    Eigen::Matrix3f v2 = FARM_UNWRAP(maybe_v2);
    EXPECT_EQ(v2, v);
  }

  {
    Eigen::Matrix3d v;
    v << 1, 2, 3,  //
        4, 5, 6,   //
        7, 8, 9;
    proto::Mat3F64 proto = toProt(v);
    EXPECT_EQ(proto.col_0().x(), 1);
    EXPECT_EQ(proto.col_1().x(), 2);
    EXPECT_EQ(proto.col_2().x(), 3);
    EXPECT_EQ(proto.col_0().y(), 4);
    EXPECT_EQ(proto.col_1().y(), 5);
    EXPECT_EQ(proto.col_2().y(), 6);
    EXPECT_EQ(proto.col_0().z(), 7);
    EXPECT_EQ(proto.col_1().z(), 8);
    EXPECT_EQ(proto.col_2().z(), 9);

    auto maybe_v2 = fromProt(proto);
    Eigen::Matrix3d v2 = FARM_UNWRAP(maybe_v2);
    EXPECT_EQ(v2, v);
  }
}
