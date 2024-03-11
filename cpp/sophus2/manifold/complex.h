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

/// Generic complex number implementation
///
/// Impl class without any storage, but only static methods.
template <class TScalar>
class ComplexImpl {
 public:
  /// The underlying scalar type.
  using Scalar = TScalar;
  /// A complex number is a tuple.
  static int constexpr kNumParams = 2;
  /// Complex multiplication is commutative (and it is a field itself).
  static bool constexpr kIsCommutative = true;

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
      2>;

  // factories

  /// Returns zero constant which is (0, 0).
  static auto zero() -> Eigen::Vector<Scalar, 2> {
    return Eigen::Vector<Scalar, 2>::Zero();
  }

  /// Returns one constant which is (1, 0).
  static auto one() -> Eigen::Vector<Scalar, 2> {
    return Eigen::Vector<Scalar, 2>(1.0, 0.0);
  }

  /// There are no particular constraints on the complex space and this function
  /// returns always true.
  static auto areParamsValid(Params const& /*unused*/)
      -> sophus2::Expected<Success> {
    return sophus2::Expected<Success>{};
  }

  /// Returns examples of valid parameters.
  static auto paramsExamples() -> std::vector<Params> {
    return pointExamples<Scalar, 2>();
  }

  /// Since there are no invalid parameters, this return an empty list.
  static auto invalidParamsExamples() -> std::vector<Params> {
    return std::vector<Params>({});
  }

  // operations

  /// Complex addition given two complex numbers (a,i) and  (b,j).
  ///
  /// Returns (a+b, i+j).
  template <class TCompatibleScalar>
  static auto addition(
      Eigen::Vector<Scalar, 2> const& lhs_real_imag,
      Eigen::Vector<TCompatibleScalar, 2> const& rhs_real_imag)
      -> ParamsReturn<TCompatibleScalar> {
    return lhs_real_imag + rhs_real_imag;
  }

  /// Complex multiplication given two complex numbers (a,i) and (b,j).
  ///
  /// Returns (a+b-i*j, a*j+b*i).
  template <class TCompatibleScalar>
  static auto multiplication(
      Eigen::Vector<Scalar, 2> const& lhs_real_imag,
      Eigen::Vector<TCompatibleScalar, 2> const& rhs_real_imag)
      -> ParamsReturn<TCompatibleScalar> {
    // complex multiplication
    return ParamsReturn<TCompatibleScalar>(
        lhs_real_imag.x() * rhs_real_imag.x() -
            lhs_real_imag.y() * rhs_real_imag.y(),
        lhs_real_imag.x() * rhs_real_imag.y() +
            lhs_real_imag.y() * rhs_real_imag.x());
  }

  /// Given input (a,i), return complex conjugate (a, -i).
  static auto conjugate(Eigen::Vector<Scalar, 2> const& a)
      -> Eigen::Vector<Scalar, 2> {
    return Eigen::Vector<Scalar, 2>(a.x(), -a.y());
  }

  /// Given input (a,i) return complex inverse (a, -i) / (a**2 + i**2).
  static auto inverse(Eigen::Vector<Scalar, 2> const& real_imag)
      -> Eigen::Vector<Scalar, 2> {
    return conjugate(real_imag) / squaredNorm(real_imag);
  }

  /// Given input (a,i), returns complex norm sqrt(a**2 + i**2).
  static auto norm(Eigen::Vector<Scalar, 2> const& real_imag) -> Scalar {
    using std::hypot;
    return hypot(real_imag.x(), real_imag.y());
  }

  /// Given input (a,i), returns square of complex norm a**2 + i**2.
  static auto squaredNorm(Eigen::Vector<Scalar, 2> const& real_imag) -> Scalar {
    return real_imag.squaredNorm();
  }
};

/// Complex number class template.
///
/// A complex number is manifold with additional structure. In particular,
/// it is - like the quaternion numbers - a division ring and fulfills the
/// sophus2::concepts::DivisionRingConcept. Furthermore, it is has commutative
/// multiplication (as opposed to the quaternion numbers) and hence is a field.
/// The complex numbers are the only other field of real numbers in addition to
/// the real numbers itself.
template <class TScalar>
class Complex {
 public:
  /// The underlying scalar type.
  using Scalar = TScalar;
  /// Type of the imaginary part is also a Scalar and hence has the some type
  /// than the real part.
  using Imag = Scalar;
  /// The stateless implementation.
  using Impl = ComplexImpl<Scalar>;

  /// A complex number is a tuple.
  static int constexpr kNumParams = 2;

  /// Parameter storage type.
  using Params = Eigen::Vector<Scalar, kNumParams>;

  /// Return type of binary operator
  ///
  /// In particular relevant mixing different scalars (such as double and
  /// ceres::Jet).
  template <class TCompatibleScalar>
  using ComplexReturn =
      Complex<typename Eigen::ScalarBinaryOpTraits<Scalar, TCompatibleScalar>::
                  ReturnType>;

  // constructors and factories

  /// Default constructor creates a zero complex number (0, 0).
  Complex() : params_(Impl::zero()) {}

  /// Copy constructor
  Complex(Complex const&) = default;
  /// Copy assignment
  auto operator=(Complex const&) -> Complex& = default;

  /// Returns zero complex number (0, 0).
  static auto zero() -> Complex { return Complex::fromParams(Impl::zero()); }

  /// Returns identity complex number (1, 0).
  static auto one() -> Complex { return Complex::fromParams(Impl::one()); }

  /// Constructs complex number from params tuple.
  static auto fromParams(Params const& params) -> Complex {
    Complex z(UninitTag{});
    z.setParams(params);
    return z;
  }

  /// Returns params tuple.
  [[nodiscard]] auto params() const -> Params const& { return params_; }

  /// Set state using params tuple.
  void setParams(Params const& params) { params_ = params; }

  /// Returns reference to real scalar.
  auto real() -> Scalar& { return params_[0]; }
  /// Returns reference to real scalar.
  [[nodiscard]] auto real() const -> Scalar const& { return params_[0]; }

  /// Returns reference to imaginary scalar.
  auto imag() -> Scalar& { return params_[1]; }
  /// Returns reference to imaginary scalar.
  [[nodiscard]] auto imag() const -> Scalar const& { return params_[1]; }

  /// Complex addition
  template <class TCompatibleScalar>
  auto operator+(Complex<TCompatibleScalar> const& other) const
      -> ComplexReturn<TCompatibleScalar> {
    return Complex::fromParams(Impl::addition(this->params_, other.params()));
  }

  /// Complex multiplication
  template <class TCompatibleScalar>
  auto operator*(Complex<TCompatibleScalar> const& other) const
      -> ComplexReturn<TCompatibleScalar> {
    return ComplexReturn<TCompatibleScalar>::fromParams(
        Impl::multiplication(this->params_, other.params()));
  }

  /// Complex conjugate
  [[nodiscard]] auto conjugate() const -> Complex {
    return Complex::fromParams(Impl::conjugate(this->params_));
  }

  /// Complex inverse
  [[nodiscard]] auto inverse() const -> Complex {
    return Complex::fromParams(Impl::inverse(this->params_));
  }

  /// Complex norm
  [[nodiscard]] auto norm() const -> Scalar {
    return Impl::norm(this->params_);
  }

  /// Square of complex norm
  [[nodiscard]] auto squaredNorm() const -> Scalar {
    return Impl::squaredNorm(this->params_);
  }

 private:
  Complex(UninitTag /*unused*/) {}
  Eigen::Vector2<Scalar> params_;
};

}  // namespace sophus2
