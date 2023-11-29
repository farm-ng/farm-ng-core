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

#include "farm_ng/core/logging/assert_within.h"

#include <gtest/gtest.h>

using namespace farm_ng;

TEST(relativeCloseness, unit) {  // NOLINT

  double constexpr kEps = 0.00001;
  EXPECT_NEAR(relativeCloseness(1.1, 1.0), 0.090909, kEps);
  EXPECT_NEAR(relativeCloseness(1.0, 0.9), 0.0999999, kEps);
  EXPECT_NEAR(relativeCloseness(5000.0, 0.9), 0.999820, kEps);
  EXPECT_NEAR(relativeCloseness(5000.0, 5005.1), 0.001018, kEps);

  EXPECT_NEAR(relativeCloseness(1.1, -1.0), 1.909090, kEps);
  EXPECT_NEAR(relativeCloseness(5000.0, -0.9), 1.000179, kEps);
  EXPECT_NEAR(relativeCloseness(-5000.0, 0.9), 1.000179, kEps);
  EXPECT_NEAR(relativeCloseness(5000.0, 5005.1), 0.001018, kEps);
  EXPECT_NEAR(relativeCloseness(5000000.0, 5005005.1), 0.001000, kEps);
  EXPECT_NEAR(relativeCloseness(5000.0, 5005.1), 0.001018, kEps);
}

TEST(assert_near, check_near_for_floating_Test) {  // NOLINT
  char const* const msg_header = "FARM_ASSERT_WITHIN_REL failed";
  char const* const abs_msg_header = "FARM_ASSERT_WITHIN_ABS failed";

  FARM_ASSERT_WITHIN_REL(0.0, 0.0, 0.0);
  FARM_ASSERT_WITHIN_ABS(0.0, 0.0, 0.0);

  FARM_ASSERT_WITHIN_REL(1.0, 1.01, 0.03);
  FARM_ASSERT_WITHIN_ABS(1.0, 1.01, 0.03);

  FARM_ASSERT_WITHIN_REL(999999.0, 999999.9f, 0.001);
  ASSERT_DEATH(
      { FARM_ASSERT_WITHIN_REL(999999.0, 999999.9, 0.00000001); }, msg_header);
  FARM_ASSERT_WITHIN_ABS(999999.0, 999999.9f, 1.0);
  ASSERT_DEATH(
      { FARM_ASSERT_WITHIN_ABS(999999.0, 999999.9f, 0.1); }, abs_msg_header);

  FARM_ASSERT_WITHIN_REL(0.0, 0.001, 1.0);
  ASSERT_DEATH({ FARM_ASSERT_WITHIN_REL(1.0f, 1.1, 0.001); }, msg_header);
  FARM_ASSERT_WITHIN_ABS(0.0, 0.001, 1.0);
  FARM_ASSERT_WITHIN_ABS(0.0, 0.001, 0.001);

  FARM_ASSERT_WITHIN_REL(
      std::numeric_limits<double>::epsilon(),
      2 * std::numeric_limits<double>::epsilon(),
      0.5);
  FARM_ASSERT_WITHIN_ABS(
      std::numeric_limits<double>::epsilon(),
      2 * std::numeric_limits<double>::epsilon(),
      0.5);
  FARM_ASSERT_WITHIN_ABS(
      std::numeric_limits<double>::epsilon(),
      2 * std::numeric_limits<double>::epsilon(),
      std::numeric_limits<double>::epsilon());

  ASSERT_DEATH({ FARM_ASSERT_WITHIN_REL(1.0, -1.01, 0.03); }, msg_header);
  ASSERT_DEATH({ FARM_ASSERT_WITHIN_ABS(1.0, -1.01, 0.03); }, abs_msg_header);

  FARM_ASSERT_WITHIN_REL(0.0, 0.001, 0.001);
  FARM_ASSERT_WITHIN_ABS(0.0, 0.001, 0.001);

  FARM_ASSERT_WITHIN_REL(-0.0001, +0.0001, 0.0003);
  FARM_ASSERT_WITHIN_ABS(-0.0001, +0.0001, 0.0003);
}
