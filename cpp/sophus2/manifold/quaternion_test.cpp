// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "sophus2/manifold/quaternion.h"

#include "sophus2/concepts/division_ring_prop_tests.h"

#include <gtest/gtest.h>

namespace sophus2::test {

TEST(quaternion, unit) { Quaternion<double> q; }

TEST(quaternion, prop_test) {
  DivisionRingTestSuite<Quaternion<double>>::runAllTests("Quaternion<double>");
  DivisionRingTestSuite<Quaternion<float>>::runAllTests("Quaternion<float>");
}
}  // namespace sophus2::test
