// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sophus/common/common.h"
#include "sophus/concepts/manifold.h"

#include <Eigen/Dense>

namespace sophus {

// Forward declarations
template <class TScalar, int kDim>
class UnitVector;

// Convenience typedefs
template <class TScalar>
using UnitVector3 = UnitVector<TScalar, 3>;
template <class TScalar>
using UnitVector2 = UnitVector<TScalar, 2>;

using UnitVector2F64 = UnitVector2<double>;
using UnitVector3F64 = UnitVector3<double>;

namespace linalg {

/// A n-dimensional vector of unit length.
///
/// Impl class without any storage, but only static methods.
template <class TScalar, int kDim>
class UnitVectorImpl {
 public:
  /// scalar type
  using Scalar = TScalar;

  /// The DOF is one dimension smaller than the embedding dimension.
  /// Examples:
  ///    - A unit circle lives in a 2d space, but a point on a circle can be
  ///      represented using a single scalar (e.g. an angle).
  ///    - A unit sphere lives in a 3d space, but a point on a sphere can be
  ///      described using a 2-tuple (e.g. latitude and longitude).
  static int constexpr kDof = kDim - 1;
  /// Internally the unit vector is represented by a vector, hence the number of
  /// parameters equals the number of dimension of the embedding space.
  static int constexpr kNumParams = kDim;

  /// internal parameter representation
  using Params = Eigen::Vector<Scalar, kNumParams>;
  /// tangent vector representation
  using Tangent = Eigen::Vector<Scalar, kDof>;

  /// return the unit vector along x: (1, 0, 0, ...)
  static auto unitX() -> UnitVector<Scalar, kDim> {
    return UnitVector<Scalar, kDim>::fromUnitVector(Params::UnitX());
  }

  /// return the unit vector along y: (0, 1, 0, 0, ...)
  ///
  /// Precondition: kDim >= 1
  static auto unitY() -> UnitVector<Scalar, kDim> {
    return UnitVector<Scalar, kDim>::fromUnitVector(Params::UnitY());
  }

  /// return the unit vector along z: (0, 0, 1, 0, 0, ...)
  ///
  /// Precondition: kDim >= 2
  static auto unitZ() -> UnitVector<Scalar, kDim> {
    return UnitVector<Scalar, kDim>::fromUnitVector(Params::UnitZ());
  }

  /// Returns false if the norm of the vector if not close to 1.
  static auto areParamsValid(Params const& unit_vector)
      -> sophus::Expected<Success> {
    static Scalar const kThr = kEpsilon<Scalar>;
    Scalar const squared_norm = unit_vector.squaredNorm();
    using std::abs;
    if (!(abs(squared_norm - 1.0) <= kThr)) {
      return SOPHUS_UNEXPECTED(
          "unit vector ({}) is not of unit length.\n"
          "Squared norm: {}, thr: {}",
          unit_vector.transpose(),
          squared_norm,
          kThr);
    }
    return sophus::Expected<Success>{};
  }

  /// oplus operator
  ///
  /// See: Hartley Zisserman
  static auto oplus(Params const& params, Tangent const& delta) -> Params {
    return matRx(params) * exp(delta);
  }

  /// ominus operator
  static auto ominus(Params const& lhs_params, Params const& rhs_params)
      -> Tangent {
    return log((matRx(lhs_params).transpose() * rhs_params).eval());
  }

  /// Params examples
  static auto paramsExamples() -> std::vector<Params> {
    return std::vector<Params>(
        {Params::UnitX(), Params::UnitY(), -Params::UnitX(), -Params::UnitY()});
  }

  /// Examples of invalid params.
  static auto invalidParamsExamples() -> std::vector<Params> {
    return std::vector<Params>(
        {Params::Zero(), Params::Ones(), 2.0 * Params::UnitX()});
  }

  /// Tangent vector examples
  static auto tangentExamples() -> std::vector<Tangent> {
    return std::vector<Tangent>({
        Tangent::Zero(),
        0.01 * Tangent::UnitX(),
        0.001 * Tangent::Ones(),
    });
  }

 private:
  static auto matRx(Params const& params)
      -> Eigen::Matrix<Scalar, kNumParams, kNumParams> {
    static Eigen::Vector<TScalar, kDim> const kUnitX =
        Eigen::Vector<TScalar, kDim>::UnitX();
    if ((kUnitX - params).squaredNorm() < kEpsilon<Scalar>) {
      return Eigen::Matrix<Scalar, kNumParams, kNumParams>::Identity();
    }
    Params v = params - kUnitX;
    return Eigen::Matrix<Scalar, kNumParams, kNumParams>::Identity() -
           2.0 * (v * v.transpose()) / v.squaredNorm();
  }

  static auto exp(Tangent const& delta) -> Params {
    using std::cos;
    Params params;
    Scalar theta = delta.norm();
    params[0] = cos(theta);
    params.template tail<kDof>() = sinc(theta) * delta;
    return params;
  }

  static auto log(Params const& params) -> Tangent {
    using std::atan2;

    static Tangent const kUnitX = Tangent::UnitX();
    Scalar x = params[0];
    Tangent tail = params.template tail<kDof>();
    Scalar theta = tail.norm();

    if (abs(theta) < kEpsilon<Scalar>) {
      return atan2(Scalar(0.0), x) * kUnitX;
    }

    return (1.0 / theta) * atan2(theta, x) * tail;
  }

  static auto sinc(Scalar x) -> Scalar {
    using std::abs;
    using std::sin;
    if (abs(x) < kEpsilon<Scalar>) {
      return 1.0 - (1.0 / 6.0) * (x * x);
    }
    return sin(x) / x;
  }
};

}  // namespace linalg

/// A n-dimensional vector of unit length.
template <class TScalar, int kDim>
class UnitVector : public linalg::UnitVectorImpl<TScalar, kDim> {
 public:
  /// scalar type
  using Scalar = TScalar;

  /// The stateless implementation.
  using Impl = linalg::UnitVectorImpl<Scalar, kDim>;
  static_assert(concepts::ManifoldImpl<Impl>);

  /// The DOF is one dimension smaller than the embedding dimension.
  /// Examples:
  ///    - A unit circle lives in a 2d space, but a point on a circle can be
  ///      represented using a single scalar (e.g. an angle).
  ///    - A unit sphere lives in a 3d space, but a point on a sphere can be
  ///      described using a 2-tuple (e.g. latitude and longitude).
  static int constexpr kDof = kDim - 1;
  /// Internally the unit vector is represented by a vector, hence the number of
  /// parameters equals the number of dimension of the embedding space.
  static int constexpr kNumParams = kDim;

  /// internal parameter representation
  using Params = Eigen::Vector<Scalar, kNumParams>;
  /// tangent vector representation
  using Tangent = Eigen::Vector<Scalar, kDof>;

  /// Tries to construct it given an input vector v.
  ///
  /// Returns error if the norm of v is not close to 1
  static auto tryFromUnitVector(Eigen::Matrix<TScalar, kDim, 1> const& v)
      -> Expected<UnitVector> {
    SOPHUS_TRY(auto, maybe_valid, Impl::areParamsValid(v));
    UnitVector unit_vector;
    unit_vector.vector_ = v;
    return unit_vector;
  }

  /// Constructs it given an input vector v.
  ///
  /// Precondition: norm of v must be close to 1
  static auto fromParams(Eigen::Matrix<TScalar, kDim, 1> const& v)
      -> UnitVector {
    Expected<UnitVector> e_vec = tryFromUnitVector(v);
    if (!e_vec.has_value()) {
      SOPHUS_PANIC("{}", e_vec.error());
    }
    return e_vec.value();
  }

  /// Precondition: v must be of unit length.
  static auto fromUnitVector(Eigen::Matrix<TScalar, kDim, 1> const& v)
      -> UnitVector {
    return fromParams(v);
  }

  /// Constructs it given an input vector v.
  ///
  /// If v is not of unit length  then it will be normalized accordingly.
  static auto fromVectorAndNormalize(Eigen::Matrix<TScalar, kDim, 1> const& v)
      -> UnitVector {
    return fromUnitVector(v.normalized());
  }

  /// oplus operator
  auto oplus(Tangent const& delta) const -> UnitVector {
    UnitVector v;
    v.vector_ = Impl::oplus(vector_, delta);
    return v;
  }

  /// ominus operator
  auto ominus(UnitVector const& rhs_params) const -> Tangent {
    return Impl::ominus(vector_, rhs_params.vector_);
  }

  /// Set state using params.
  void setParams(Eigen::Matrix<TScalar, kDim, 1> const& v) const {
    SOPHUS_ASSERT(Impl::areParamsValid(v));
    vector_.params = v;
  }

  /// Get params
  [[nodiscard]] auto params() const -> Eigen::Matrix<TScalar, kDim, 1> const& {
    return vector_;
  }

  /// Return as ordinary Eigen vector
  [[nodiscard]] auto vector() const -> Eigen::Matrix<TScalar, kDim, 1> const& {
    return vector_;
  }

  /// unsafe mutable access to params pointer
  ///
  /// Note: It is the users responsibility to hold up the class invariant that
  ///       the vector must have unit length.
  [[nodiscard]] auto unsafeMutPtr() { return this->vector_.data(); }

  /// cost access to params pointer
  [[nodiscard]] auto ptr() const { return this->vector_.data(); }

  /// Copy constructor.
  UnitVector(UnitVector const&) = default;
  /// Copy assignment.
  auto operator=(UnitVector const&) -> UnitVector& = default;

  /// Average a set of unit vectors.
  ///
  /// This is done by summing uo the vectors and re-normalizing the result.
  template <concepts::Range TSequenceContainer>
  static auto average(TSequenceContainer const& range) -> UnitVector {
    size_t const len = std::distance(std::begin(range), std::end(range));
    SOPHUS_ASSERT_GE(len, 0);

    Params params = Params::Zero();
    for (auto const& m : range) {
      params += m.params();
    }
    return fromVectorAndNormalize(params / len);
  }

 private:
  UnitVector() {}

  // Class invariant: v_ is of unit length.
  Eigen::Matrix<TScalar, kDim, 1> vector_;
};

static_assert(concepts::Manifold<UnitVector<double, 3>>);

}  // namespace sophus
