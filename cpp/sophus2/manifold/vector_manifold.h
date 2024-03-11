// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sophus2/common/common.h"
#include "sophus2/concepts/manifold.h"
#include "sophus2/linalg/vector_space.h"

namespace sophus2 {

/// A n-dimensional vector is a trivial example of a manifold.
///
template <class TScalar, int kDim>
struct VectorManifold {
  /// scalar type
  using Scalar = TScalar;
  /// DOF equals kDim
  static int constexpr kDof = kDim;
  /// kNumParams equals kDim
  static int constexpr kNumParams = kDim;

  /// A vector is trivially represented as itself.
  using Params = Eigen::Vector<TScalar, kDim>;
  /// The tangent vector is trivially represented as itself.
  using Tangent = Params;

  VectorManifold() { vec.setZero(); }

  /// Constructs manifold given vector input
  VectorManifold(Params const& vec) : vec(vec) {}

  /// ominus operator is just vector addition
  auto oplus(Tangent const& a) const -> VectorManifold {
    return VectorManifold(this->vec + a);
  }

  /// ominus operator is just vector subtraction
  auto ominus(VectorManifold const& other) const -> Tangent {
    return this->vec - other.params();
  }

  /// Construct using params.
  static auto fromParams(Params const& params) -> VectorManifold {
    return VectorManifold(params);
  }

  /// Set state using params.
  auto setParams(Params const& params) -> void { vec = params; }

  /// Get params, which is just the vector itself.
  auto params() const -> Params const& { return vec; }

  /// cost access to params pointer
  auto ptr() const -> Scalar const* { return vec.data(); }

  /// unsafe mutable access to params pointer
  auto unsafeMutPtr() -> Scalar* { return vec.data(); }

  /// Examples
  static auto tangentExamples() -> std::vector<Tangent> {
    return pointExamples<Scalar, kDim>();
  }

  /// Average a set of vectors.
  template <concepts::Range TSequenceContainer>
  static auto average(TSequenceContainer const& range)
      -> std::optional<VectorManifold> {
    size_t const len = std::distance(std::begin(range), std::end(range));
    SOPHUS_ASSERT_GE(len, 0);

    Params params = Params::Zero();
    for (auto const& m : range) {
      params += m.params();
    }
    return VectorManifold(params / len);
  }

  /// Vector / n-tuple
  Params vec;
};

}  // namespace sophus2
