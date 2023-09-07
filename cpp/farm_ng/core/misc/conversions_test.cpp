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

#include <gtest/gtest.h>

#include <random>

using namespace farm_ng;

TEST(conversions, unit) {
  FARM_ASSERT_NEAR(inchesFromMeters(1.0), 39.3701, 1e-3);
  FARM_ASSERT_NEAR(metersFromInches(1.0), 0.0254, 1e-3);

  FARM_ASSERT_NEAR(metersFromFeet(1.0), 0.3048, 1e-3);
  FARM_ASSERT_NEAR(feetFromMeters(1.0), 3.28084, 1e-3);

  FARM_ASSERT_NEAR(degFromRad(1.0), 57.2958, 1e-3);
  FARM_ASSERT_NEAR(radFromDeg(1.0), 0.0174533, 1e-3);

  FARM_ASSERT_NEAR(degFromRad(M_PI), 180.0, 1e-3);
  FARM_ASSERT_NEAR(radFromDeg(180.0), M_PI, 1e-3);

  FARM_ASSERT_NEAR(radFromArcMinute(1.0), 0.000290888, 1e-3);
  FARM_ASSERT_NEAR(arcMinuteFromRad(1.0), 3437.75, 1e-3);
}

TEST(conversions, roundtrip) {
  for (double x = -10; x < 10; x += 0.1) {
    FARM_ASSERT_NEAR(inchesFromMeters(metersFromInches(x)), x, 1e-6);
    FARM_ASSERT_NEAR(metersFromInches(inchesFromMeters(x)), x, 1e-6);
    FARM_ASSERT_NEAR(degFromRad(radFromDeg(x)), x, 1e-6);
    FARM_ASSERT_NEAR(radFromArcMinute(arcMinuteFromRad(x)), x, 1e-6);
  }
}
