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

#include "farm_ng/core/misc/conversions.h"

#include "farm_ng/core/logging/assert_within.h"

#include <gtest/gtest.h>

#include <random>

using namespace farm_ng;

TEST(conversions, unit) {
  FARM_ASSERT_WITHIN_REL(inchesFromMeters(1.0), 39.3701, 1e-3);
  FARM_ASSERT_WITHIN_REL(metersFromInches(1.0), 0.0254, 1e-3);

  FARM_ASSERT_WITHIN_REL(metersFromFeet(1.0), 0.3048, 1e-3);
  FARM_ASSERT_WITHIN_REL(feetFromMeters(1.0), 3.28084, 1e-3);

  FARM_ASSERT_WITHIN_REL(degFromRad(1.0), 57.2958, 1e-3);
  FARM_ASSERT_WITHIN_REL(radFromDeg(1.0), 0.0174533, 1e-3);

  FARM_ASSERT_WITHIN_REL(degFromRad(M_PI), 180.0, 1e-3);
  FARM_ASSERT_WITHIN_REL(radFromDeg(180.0), M_PI, 1e-3);

  using Vec2d = Eigen::Vector2d;
  FARM_ASSERT(degFromRad(Vec2d{M_PI, M_PI}).isApprox(Vec2d{180.0, 180.0}));
  FARM_ASSERT(radFromDeg(Vec2d{180.0, 180.0}).isApprox(Vec2d{M_PI, M_PI}));

  FARM_ASSERT_WITHIN_REL(radFromArcMinute(1.0), 0.000290888, 1e-3);
  FARM_ASSERT_WITHIN_REL(arcMinuteFromRad(1.0), 3437.75, 1e-3);
}

TEST(conversions, roundtrip) {
  for (double x = -10; x < 10; x += 0.1) {
    FARM_ASSERT_WITHIN_REL(inchesFromMeters(metersFromInches(x)), x, 1e-6);
    FARM_ASSERT_WITHIN_REL(metersFromInches(inchesFromMeters(x)), x, 1e-6);
    FARM_ASSERT_WITHIN_REL(degFromRad(radFromDeg(x)), x, 1e-6);
    FARM_ASSERT_WITHIN_REL(radFromArcMinute(arcMinuteFromRad(x)), x, 1e-6);
  }
}
