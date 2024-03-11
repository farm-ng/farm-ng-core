// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "sophus2/lie/scaling_translation.h"

#include "sophus2/concepts/group_accessors_unit_tests.h"
#include "sophus2/concepts/lie_group_prop_tests.h"

#include <gtest/gtest.h>

namespace sophus2::test {

TEST(ScalingTranslation, lie_group_prop_tests) {
  LieGroupPropTestSuite<ScalingTranslation2<double>>::runAllTests(
      "ScalingTranslation2F64");
  LieGroupPropTestSuite<ScalingTranslation2<float>>::runAllTests(
      "ScalingTranslation2F32");
  LieGroupPropTestSuite<ScalingTranslation3<double>>::runAllTests(
      "ScalingTranslation3F64");
  LieGroupPropTestSuite<ScalingTranslation3<float>>::runAllTests(
      "ScalingTranslation3F32");
}
}  // namespace sophus2::test
