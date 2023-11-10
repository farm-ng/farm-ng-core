// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sophus/concepts/manifold.h"

namespace sophus {
namespace test {

/// Property tests for Manifolds (e.g. sophus::GroupManifold<...>,
/// sophus::UnitVector, ...)
template <concepts::Manifold TManifold>
struct ManifoldPropTestSuite {
  /// the manifold
  using Manifold = TManifold;
  /// the underlying scalar
  using Scalar = typename Manifold::Scalar;
  /// degrees of freedom - dimensionality of tangent space
  static int constexpr kDof = Manifold::kDof;
  /// @brief  number of parameters
  static int constexpr kNumParams = Manifold::kNumParams;

  /// tangent type
  using Tangent = Eigen::Vector<Scalar, kDof>;
  /// internal parametrization
  using Params = Eigen::Vector<Scalar, kNumParams>;

  /// test oplus/ominus properties
  static void oplusOMinusRoundTripTest(std::string manifold_name) {
    auto params_examples = Manifold::paramsExamples();
    auto tangent_examples = Manifold::tangentExamples();

    for (size_t params_id = 0; params_id < params_examples.size();
         ++params_id) {
      Params params = SOPHUS_AT(params_examples, params_id);
      Manifold a = Manifold::fromParams(params);
      for (size_t tangent_id = 0; tangent_id < tangent_examples.size();
           ++tangent_id) {
        Tangent delta = SOPHUS_AT(tangent_examples, tangent_id);
        Manifold b = a.oplus(delta);
        SOPHUS_ASSERT_NEAR(
            a.ominus(b),
            delta,
            kEpsilonF64,
            "oplusOMinusRoundTripTest: {}\n"
            "a: {}\nb: {}",
            manifold_name,
            a.vector().transpose(),
            b.vector().transpose());
      }
    }
  }

  /// all tests
  static void runAllTests(std::string manifold_name) {
    oplusOMinusRoundTripTest(manifold_name);
  }
};

}  // namespace test
}  // namespace sophus
