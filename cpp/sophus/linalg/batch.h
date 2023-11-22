// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sophus/common/common.h"

namespace sophus {

// Following similar pattern than ceres::Jet
template <class TT, int kNum>
struct Batch {
  Batch() {}

  explicit Batch(Eigen::Array<TT, kNum, 1> const& b_) : b(b_) {}

  // Constructor from scalar: a + 0.
  explicit Batch(const TT& value) { b.setConstant(TT(value)); }

  // Compound operators
  Batch<TT, kNum>& operator+=(Batch<TT, kNum> const& y) {
    *this = *this + y;
    return *this;
  }

  Batch<TT, kNum> operator-() const { return Batch<TT, kNum>(-this->b); }

  bool operator==(Batch<TT, kNum> const& y) { return (this->b == y.b).all(); }

  Eigen::Array<TT, kNum, 1> b;
};

template <class TT, int kNum>
auto operator<<(std::ostream& os, Batch<TT, kNum> const& batch)
    -> std::ostream& {
  os << "[" << batch.b << "]";
  return os;
}

template <class T, int N>
inline Batch<T, N> hypot(Batch<T, N> const& f, Batch<T, N> const& g) {
  return Batch{((f.b * f.b) + (g.b * g.b)).sqrt().eval()};
}

template <class T, int N>
inline Batch<T, N> abs(Batch<T, N> const& f) {
  return Batch{f.b.abs().eval()};
}

template <class T, int N>
inline Batch<T, N> operator-(Batch<T, N> const& f, Batch<T, N> const& g) {
  return Batch{(f.b - g.b).eval()};
}

template <class T, int N>
inline Batch<T, N> operator+(Batch<T, N> const& f, Batch<T, N> const& g) {
  return Batch{(f.b + g.b).eval()};
}
template <class T, int N>
inline Batch<T, N> operator*(Batch<T, N> const& f, Batch<T, N> const& g) {
  return Batch{(f.b * g.b).eval()};
}

template <class T, int N>
inline Batch<T, N> operator/(Batch<T, N> const& f, Batch<T, N> const& g) {
  return Batch{(f.b / g.b).eval()};
}

template <class TT, int N>
inline Batch<TT, N> operator*(Batch<TT, N> const& f, TT s) {
  return Batch{(f.b * s).eval()};
}

template <class TT, int N>
inline Batch<TT, N> operator*(TT s, Batch<TT, N> const& f) {
  return Batch{(s * f.b).eval()};
}

template <class TScalar>
struct BatchTrait {
  using Scalar = TScalar;
  using ScalarBatch = TScalar;
  static int constexpr kNumBatches = 1;
};

template <class TScalar, int kNum>
struct BatchTrait<Batch<TScalar, kNum>> {
  using Scalar = TScalar;
  using ScalarBatch = Batch<TScalar, kNum>;
  static int constexpr kNumBatches = kNum;
};

}  // namespace sophus

namespace Eigen {

// This is mirrored from ceres::Jet.
template <class TT, int kNum>
struct NumTraits<sophus::Batch<TT, kNum>> {
  using Real = sophus::Batch<TT, kNum>;
  using NonInteger = sophus::Batch<TT, kNum>;
  using Nested = sophus::Batch<TT, kNum>;
  using Literal = sophus::Batch<TT, kNum>;
  enum {
    IsComplex = 0,
    IsInteger = 0,
    IsSigned,
    ReadCost = 1,
    AddCost = 1,
    // For Jet types, multiplication is more expensive than addition.
    MulCost = 3,
    HasFloatingPoint = 1,
    RequireInitialization = 1
  };

  static sophus::Batch<TT, kNum> dummy_precision() {
    return sophus::Batch<TT, kNum>(1e-12);
  }

  inline static Real epsilon() {
    return Real(std::numeric_limits<TT>::epsilon());
  }

  inline static int digits10() { return NumTraits<TT>::digits10(); }

  inline static Real highest() {
    return Real((std::numeric_limits<TT>::max)());
  }
  inline static Real lowest() {
    return Real(-(std::numeric_limits<TT>::max)());
  }
};

template <class BinaryOp, class TT, int kNum>
struct ScalarBinaryOpTraits<sophus::Batch<TT, kNum>, TT, BinaryOp> {
  using ReturnType = sophus::Batch<TT, kNum>;
};
template <class BinaryOp, class TT, int kNum>
struct ScalarBinaryOpTraits<TT, sophus::Batch<TT, kNum>, BinaryOp> {
  using ReturnType = sophus::Batch<TT, kNum>;
};

}  // namespace Eigen
