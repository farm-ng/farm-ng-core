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

TEST(robot_velocity, unit_tests) {
  /// Uniform circular velocity in a rigid body frame in its own frame.
  Eigen::Vector<double, 6> uniform_egocentric_velocity = {
      1.0, 0.1, 0, 0, 0, 0.1};

  // Initial condition. Robot is at an arbitrary pose in the world frame.
  Pose3F64 world_from_robot_at_t0(
      Isometry3F64::fromRz(1.5) * Isometry3F64::fromTx(0.2),
      "world",
      "robot_at_t0");
  EXPECT_EQ(world_from_robot_at_t0.frameA(), "world");
  EXPECT_EQ(world_from_robot_at_t0.frameB(), "robot_at_t0");
  world_from_robot_at_t0.tangentInB() = uniform_egocentric_velocity;

  Pose3F64 world_from_robot_at_t1 = Pose3F64(
      world_from_robot_at_t0.evolve(0.1).aFromB(), "world", "robot_at_t1");
  world_from_robot_at_t1.tangentInB() = uniform_egocentric_velocity;

  Pose3F64 world_from_robot_at_t2 = Pose3F64(
      world_from_robot_at_t0.evolve(0.2).aFromB(), "world", "robot_at_t2");
  world_from_robot_at_t2.tangentInB() = uniform_egocentric_velocity;

  FARM_ASSERT_NEAR(
      world_from_robot_at_t1.evolve(0.1).aFromB().compactMatrix(),
      world_from_robot_at_t2.aFromB().compactMatrix(),
      0.1,
      "The robot moves with constant velocity. We expect get the same result"
      "for t2 independent of whether we evolve from for 0.2s from t0 or  for "
      "0.1s form t1.");

  /// Calculate the relative pose between the robot at t1 and the robot at t2.
  auto maybe_robot_t1_from_robot_at_t2 =
      world_from_robot_at_t1.inverse() * world_from_robot_at_t2;
  Pose3F64 robot_t1_from_robot_at_t2 =
      FARM_UNWRAP(maybe_robot_t1_from_robot_at_t2);

  FARM_ASSERT_LE(
      robot_t1_from_robot_at_t2.tangentInB().norm(),
      0.00001,
      "robot at t1`, `robot at t2` is the exact same reference frame just at"
      "different points in time. We expect the velocity of the robot with "
      "respect to itself be zero by definition.");
}

}  // namespace sophus::test
