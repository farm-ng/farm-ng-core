// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "sophus/lie/pose3.h"

#include <gtest/gtest.h>

namespace sophus::test {

TEST(pose3F64, unit_tests) {
  Pose3F64 a_from_b(Isometry3F64(), "a", "b");
  EXPECT_EQ(a_from_b.frameA(), "a");
  EXPECT_EQ(a_from_b.frameB(), "b");
  FARM_ASSERT_EQ(a_from_b.tangentOfBInA(), Isometry3F64::Tangent::Zero());
  Pose3F64 b_from_c(Isometry3F64(), "b", "c");

  Pose3F64 a_from_c = *(a_from_b * b_from_c);
  FARM_ASSERT_EQ(a_from_c.frameA(), "a");
  FARM_ASSERT_EQ(a_from_c.frameB(), "c");

  Expected<Pose3F64> bad_a_from_c = a_from_c.inverse() * b_from_c;
  EXPECT_FALSE(bad_a_from_c.has_value());
  FARM_INFO("{}", bad_a_from_c.error());
  EXPECT_EQ(
      bad_a_from_c.error().details[0].msg,
      "Pose frame error: lhs a=c b=a rhs a=b b=c");

  Expected<Pose3F64> c_from_b = a_from_c.inverse() * a_from_b * b_from_c;
  EXPECT_TRUE(c_from_b.has_value());

  Expected<Pose3F64> bad_c_from_c = c_from_b * b_from_c * a_from_c;
  EXPECT_FALSE(bad_c_from_c.has_value());
  FARM_INFO("{}", bad_c_from_c.error());
  EXPECT_EQ(
      bad_c_from_c.error().details[0].msg,
      "Pose frame error: lhs a=c b=c rhs a=b b=c");
}

}  // namespace sophus::test
