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

#include <gtest/gtest.h>

using namespace sophus2;

using namespace farm_ng;
using namespace farm_ng::core;

TEST(conv_quat, unit) {
  {
    auto q = QuaternionF64::fromParams({2.0, 3.0, 4.0, 1.0});

    proto::QuaternionF64 proto = toProt(q);
    EXPECT_EQ(proto.real(), 1);
    EXPECT_EQ(proto.imag().x(), 2);
    EXPECT_EQ(proto.imag().y(), 3);
    EXPECT_EQ(proto.imag().z(), 4);
    auto maybe_q2 = fromProt(proto);
    sophus2::QuaternionF64 q2 = FARM_UNWRAP(maybe_q2);
    EXPECT_EQ(q2.real(), q.real());
    EXPECT_EQ(q2.imag(), q.imag());
  }

  {
    auto q = QuaternionF64::zero();
    proto::QuaternionF64 proto = toProt(q);
    EXPECT_EQ(proto.real(), 0);
    EXPECT_EQ(proto.imag().x(), 0);
    EXPECT_EQ(proto.imag().y(), 0);
    EXPECT_EQ(proto.imag().z(), 0);
    auto maybe_q2 = fromProt(proto);
    sophus2::QuaternionF64 q2 = FARM_UNWRAP(maybe_q2);
    EXPECT_EQ(q2.real(), q.real());
    EXPECT_EQ(q2.imag(), q.imag());
  }
}

TEST(conv_rot, unit) {
  {
    auto q = Rotation2F64::fromAngle(1.0);
    proto::Rotation2F64 proto = toProt(q);
    EXPECT_EQ(proto.theta(), 1);
    auto maybe_q2 = fromProt(proto);
    sophus2::Rotation2F64 q2 = FARM_UNWRAP(maybe_q2);
    EXPECT_EQ(q2.angle(), q.angle());
  }

  {
    auto q = sophus2::Rotation3F64::elementExamples().at(1);
    proto::Rotation3F64 proto = toProt(q);
    EXPECT_EQ(proto.unit_quaternion().real(), q.unitQuaternion().real());
    EXPECT_EQ(
        proto.unit_quaternion().imag().x(), q.unitQuaternion().imag().x());
    EXPECT_EQ(
        proto.unit_quaternion().imag().y(), q.unitQuaternion().imag().y());
    EXPECT_EQ(
        proto.unit_quaternion().imag().z(), q.unitQuaternion().imag().z());

    auto maybe_q2 = fromProt(proto);
    sophus2::Rotation3F64 q2 = FARM_UNWRAP(maybe_q2);
    EXPECT_EQ(q2.unitQuaternion().real(), q.unitQuaternion().real());
    EXPECT_EQ(q2.unitQuaternion().imag(), q.unitQuaternion().imag());
    EXPECT_EQ(q2.params(), q.params());
    EXPECT_EQ(q2.matrix(), q.matrix());

    proto::Rotation3F64 invalid_proto2;

    auto expect_invalid = fromProt(invalid_proto2);
    EXPECT_FALSE(expect_invalid);
  }
}

TEST(conv_isometry, unit) {
  {
    auto iso = sophus2::Isometry2F64::elementExamples().at(1);
    proto::Isometry2F64 proto = toProt(iso);
    EXPECT_EQ(proto.translation().x(), iso.translation().x());
    EXPECT_EQ(proto.translation().y(), iso.translation().y());
    EXPECT_EQ(proto.rotation().theta(), iso.rotation().angle());

    auto maybe_iso2 = fromProt(proto);
    sophus2::Isometry2F64 iso2 = FARM_UNWRAP(maybe_iso2);
    EXPECT_EQ(iso2.translation(), iso.translation());
    EXPECT_EQ(iso2.rotation().angle(), iso.rotation().angle());
  }

  {
    auto iso = sophus2::Isometry3F64::elementExamples().at(1);
    proto::Isometry3F64 proto = toProt(iso);
    EXPECT_EQ(proto.translation().x(), iso.translation().x());
    EXPECT_EQ(proto.translation().y(), iso.translation().y());
    EXPECT_EQ(proto.translation().z(), iso.translation().z());
    EXPECT_EQ(
        proto.rotation().unit_quaternion().real(),
        iso.rotation().unitQuaternion().real());
    EXPECT_EQ(
        proto.rotation().unit_quaternion().imag().x(),
        iso.rotation().unitQuaternion().imag().x());
    EXPECT_EQ(
        proto.rotation().unit_quaternion().imag().y(),
        iso.rotation().unitQuaternion().imag().y());
    EXPECT_EQ(
        proto.rotation().unit_quaternion().imag().z(),
        iso.rotation().unitQuaternion().imag().z());

    auto maybe_iso2 = fromProt(proto);
    sophus2::Isometry3F64 iso2 = FARM_UNWRAP(maybe_iso2);
    EXPECT_EQ(iso2.compactMatrix(), iso.compactMatrix());
  }
}
