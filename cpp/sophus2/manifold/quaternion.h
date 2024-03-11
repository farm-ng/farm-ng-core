// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once
#include "sophus2/common/common.h"
#include "sophus2/concepts/division_ring.h"
#include "sophus2/linalg/vector_space.h"

namespace sophus2 {

/// Generic quaternion number implementation
///
/// Impl class without any storage, but only static methods.
///
/// Note: This class is following the most common and classic Hamilton
/// representation of quaternions.
template <class TScalar>
class QuaternionImpl {
 public:
  /// The underlying scalar type.
  using Scalar = TScalar;
  /// A quaternion number is a 4-tuple, consisting of 1 real scalar and an
  /// imaginary 3-vector.
  /// Note: Storage order of the parameters is (imag0, imag1, imag2, real). This
  ///       is mainly to be compatible with Eigen::Quaternion data layout.
  static int constexpr kNumParams = 4;
  /// Quaternion multiplication is not commutative (and hence the set of of
  /// quaternions are not a field but merely a division ring).
  static bool constexpr kIsCommutative = false;

  /// Parameter storage type - to be passed to static functions.
  using Params = Eigen::Vector<Scalar, kNumParams>;

  /// Return type of binary operator
  ///
  /// In particular relevant mixing different scalars (such as double and
  /// ceres::Jet).
  template <class TCompatibleScalar>
  using ParamsReturn = Eigen::Vector<
      typename Eigen::ScalarBinaryOpTraits<Scalar, TCompatibleScalar>::
          ReturnType,
      4>;

  // factories

  /// Returns zero constant which is (0, 0, 0, 0).
  static auto zero() -> Eigen::Vector<Scalar, 4> {
    return Eigen::Vector<Scalar, 4>::Zero();
  }

  /// Returns one constant which is (0, 0, 0, 1).
  static auto one() -> Eigen::Vector<Scalar, 4> {
    return Eigen::Vector<Scalar, 4>(0.0, 0.0, 0.0, 1.0);
  }

  /// There are no particular constraints on the quaternion space and this
  /// function returns always true.
  static auto areParamsValid(Params const& /*unused*/)
      -> sophus2::Expected<Success> {
    return sophus2::Expected<Success>{};
  }

  /// Returns examples of valid parameters.
  static auto paramsExamples() -> std::vector<Params> {
    return pointExamples<Scalar, 4>();
  }

  /// Since there are no invalid parameters, this return an empty list.
  static auto invalidParamsExamples() -> std::vector<Params> {
    return std::vector<Params>({});
  }

  /// Quaternion addition given two quaternion numbers (a, v) and  (b, u).
  ///
  /// Returns (a+b, u+v).
  template <class TCompatibleScalar>
  static auto addition(
      Eigen::Vector<Scalar, 4> const& a,
      Eigen::Vector<TCompatibleScalar, 4> const& b)
      -> ParamsReturn<TCompatibleScalar> {
    return a + b;
  }

  /// Quaternion multiplication given two quaternion numbers (a, v) and  (b, u).
  ///
  /// Returns (a*b - <u,v>, a*u + b*v + u x v).
  template <class TCompatibleScalar>
  static auto multiplication(
      Eigen::Vector<Scalar, 4> const& lhs,
      Eigen::Vector<TCompatibleScalar, 4> const& rhs)
      -> ParamsReturn<TCompatibleScalar> {
    Eigen::Vector3<Scalar> lhs_ivec = lhs.template head<3>();
    Eigen::Vector3<TCompatibleScalar> rhs_ivec = rhs.template head<3>();

    ParamsReturn<TCompatibleScalar> out;
    out.w() = lhs.w() * rhs.w() - lhs_ivec.dot(rhs_ivec);
    out.template head<3>() =
        lhs.w() * rhs_ivec + rhs.w() * lhs_ivec + lhs_ivec.cross(rhs_ivec);
    return out;
  }

  /// Given input (a,v), return quaternion conjugate (a, -v).
  static auto conjugate(Eigen::Vector<Scalar, 4> const& a)
      -> Eigen::Vector<Scalar, 4> {
    return Eigen::Vector<Scalar, 4>(-a.x(), -a.y(), -a.z(), a.w());
  }

  /// Given input (a,i) return quaternion inverse (a, -v) / (a**2 + <v,v>).
  static auto inverse(Eigen::Vector<Scalar, 4> const& q)
      -> Eigen::Vector<Scalar, 4> {
    return conjugate(q) / squaredNorm(q);
  }

  /// Given input (a,i), returns quaternion norm sqrt(a**2 + <v,v>).
  static auto norm(Eigen::Vector<Scalar, 4> const& q) -> Scalar {
    return q.norm();
  }

  /// Given input (a,i), returns square of quaternion norm a**2 + <v,v>.
  static auto squaredNorm(Eigen::Vector<Scalar, 4> const& q) -> Scalar {
    return q.squaredNorm();
  }
};

/// Quaternion number class template.
///
/// A quaternion number is manifold with additional structure. In particular,
/// it is - like the Quaternion numbers - a division ring and fulfills the
/// sophus2::concepts::DivisionRingConcept.
template <class TScalar>
class Quaternion {
 public:
  /// The underlying scalar type.
  using Scalar = TScalar;
  /// Type of the imaginary part is a 3-vector.
  using Imag = Eigen::Vector<Scalar, 3>;
  /// The stateless implementation.
  using Impl = QuaternionImpl<Scalar>;
  /// A quaternion number is a 4-tuple, consisting of 1 real scalar and an
  /// imaginary 3-vector.
  /// Note: storage order of the parameters is (imag0, imag1, imag2, real). This
  ///       is mainly to be compatible with Eigen::Quaternion data layout.
  static int constexpr kNumParams = 4;

  /// Parameter storage type.
  using Params = Eigen::Vector<Scalar, kNumParams>;

  /// Return type of binary operator
  ///
  /// In particular relevant mixing different scalars (such as double and
  /// ceres::Jet).
  template <class TCompatibleScalar>
  using QuaternionReturn = Quaternion<typename Eigen::ScalarBinaryOpTraits<
      Scalar,
      TCompatibleScalar>::ReturnType>;

  // constructors and factories

  /// Default constructor creates a zero quaternion number (0, (0,0,0)).
  Quaternion() : params_(Impl::zero()) {}

  /// Copy constructor
  Quaternion(Quaternion const&) = default;
  /// Copy assignment
  auto operator=(Quaternion const&) -> Quaternion& = default;

  /// Creates quaternion from params (imag0, imag1, imag2, real)
  static auto fromParams(Params const& params) -> Quaternion {
    Quaternion q(UninitTag{});
    q.setParams(params);
    return q;
  }

  /// From real scalar and imaginary 3-vector.
  ///
  /// Note: Internal storage order is [imag0, imag1, imag2, real].
  static auto from(Scalar real, Eigen::Vector<Scalar, 3> const& imag)
      -> Quaternion {
    return fromParams(Params(imag[0], imag[1], imag[2], real));
  }

  /// Returns zero constant which is (0, 0, 0, 0).
  static auto zero() -> Quaternion {
    return Quaternion::fromParams(Impl::zero());
  }

  /// Returns one constant which is (0, 0, 0, 1).
  static auto one() -> Quaternion {
    return Quaternion::fromParams(Impl::one());
  }

  /// Returns params 4-tuple.
  [[nodiscard]] auto params() const -> Params const& { return params_; }

  /// Set state using params 4-tuple.
  void setParams(Params const& params) { params_ = params; }

  /// Returns reference to real scalar.
  [[nodiscard]] auto real() const -> Scalar const& { return params_[3]; }

  /// Returns reference to real scalar.
  [[nodiscard]] auto real() -> Scalar& { return params_[3]; }

  /// Returns reference to imaginary scalar.
  [[nodiscard]] auto imag() const { return params_.template head<3>(); }
  /// Returns reference to imaginary scalar.
  [[nodiscard]] auto imag() { return params_.template head<3>(); }

  /// Quaternion addition
  template <class TCompatibleScalar>
  [[nodiscard]] auto operator+(Quaternion<TCompatibleScalar> const& other) const
      -> QuaternionReturn<TCompatibleScalar> {
    return QuaternionReturn<TCompatibleScalar>::fromParams(
        Impl::addition(this->params_, other.params()));
  }

  /// Quaternion multiplication
  template <class TCompatibleScalar>
  [[nodiscard]] auto operator*(Quaternion<TCompatibleScalar> const& other) const
      -> QuaternionReturn<TCompatibleScalar> {
    return QuaternionReturn<TCompatibleScalar>::fromParams(
        Impl::multiplication(this->params_, other.params()));
  }

  /// Quaternion conjugate
  [[nodiscard]] auto conjugate() const -> Quaternion {
    return Quaternion::fromParams(Impl::conjugate(this->params_));
  }

  /// Quaternion inverse
  [[nodiscard]] auto inverse() const -> Quaternion {
    return Quaternion::fromParams(Impl::inverse(this->params_));
  }

  /// Quaternion norm
  [[nodiscard]] auto norm() const -> Scalar {
    return Impl::norm(this->params_);
  }

  /// Square of Quaternion norm
  [[nodiscard]] auto squaredNorm() const -> Scalar {
    return Impl::squaredNorm(this->params_);
  }

 private:
  Quaternion(UninitTag /*unused*/) {}
  Eigen::Vector4<Scalar> params_;
};

using QuaternionF64 = Quaternion<double>;

}  // namespace sophus2
