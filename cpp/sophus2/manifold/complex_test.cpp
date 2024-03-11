// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "sophus2/manifold/complex.h"

#include "sophus2/concepts/division_ring_prop_tests.h"

#include <gtest/gtest.h>

using namespace sophus2;

TEST(complex, unit) { Complex<double> z; }

TEST(complex, prop_test) {
  test::DivisionRingTestSuite<Complex<double>>::runAllTests("Complex<double>");
  test::DivisionRingTestSuite<Complex<float>>::runAllTests("Complex<float>");
}
