// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sophus2/common/common.h"

namespace sophus2 {

// Following similar pattern as ceres::Jet
template <class TT, int kNum>
struct Batch {
  Batch() {}

  explicit Batch(Eigen::Array<TT, kNum, 1> const& b_) : lanes(b_) {}

  // Constructor from scalar: a + 0.
  explicit Batch(const TT& value) { lanes.setConstant(TT(value)); }

  // Compound operators
  Batch<TT, kNum>& operator+=(Batch<TT, kNum> const& y) {
    *this = *this + y;
    return *this;
  }

  Batch<TT, kNum> operator-() const { return Batch<TT, kNum>(-this->lanes); }

  bool operator==(Batch<TT, kNum> const& y) {
    return (this->lanes == y.lanes).all();
  }

  Eigen::Array<TT, kNum, 1> lanes;
};

template <class TT, int kNum>
auto operator<<(std::ostream& os, Batch<TT, kNum> const& batch)
    -> std::ostream& {
  os << "[" << batch.lanes << "]";
  return os;
}

template <class T, int N>
inline Batch<T, N> hypot(Batch<T, N> const& f, Batch<T, N> const& g) {
  return Batch{((f.lanes * f.lanes) + (g.lanes * g.lanes)).sqrt().eval()};
}

template <class T, int N>
inline Batch<T, N> abs(Batch<T, N> const& f) {
  return Batch{f.lanes.abs().eval()};
}

template <class T, int N>
inline Batch<T, N> sqrt(Batch<T, N> const& f) {
  return Batch{f.lanes.sqrt().eval()};
}

template <class T, int N>
inline Batch<T, N> operator-(Batch<T, N> const& f, Batch<T, N> const& g) {
  return Batch{(f.lanes - g.lanes).eval()};
}

template <class T, int N>
inline Batch<T, N> operator+(Batch<T, N> const& f, Batch<T, N> const& g) {
  return Batch{(f.lanes + g.lanes).eval()};
}
template <class T, int N>
inline Batch<T, N> operator*(Batch<T, N> const& f, Batch<T, N> const& g) {
  return Batch{(f.lanes * g.lanes).eval()};
}

template <class T, int N>
inline Batch<T, N> operator/(Batch<T, N> const& f, Batch<T, N> const& g) {
  return Batch{(f.lanes / g.lanes).eval()};
}

template <class TT, int N>
inline Batch<TT, N> operator*(Batch<TT, N> const& f, TT s) {
  return Batch{(f.lanes * s).eval()};
}

template <class TT, int N>
inline Batch<TT, N> operator*(TT s, Batch<TT, N> const& f) {
  return Batch{(s * f.lanes).eval()};
}

template <class TScalar>
struct BatchTrait {
  using Scalar = TScalar;
  using ScalarBatch = TScalar;
  static int constexpr kBatchSize = 1;
  static bool constexpr kIsBatch = false;

  static bool anyLessEqual(TScalar const& lhs, TScalar const& rhs) {
    return lhs < rhs;
  }

  static bool allLessEqual(TScalar const& lhs, TScalar const& rhs) {
    return lhs < rhs;
  }
};

template <class TScalar, int kNum>
struct BatchTrait<Batch<TScalar, kNum>> {
  using Scalar = TScalar;
  using ScalarBatch = Batch<TScalar, kNum>;
  static int constexpr kBatchSize = kNum;
  static bool constexpr kIsBatch = true;

  static bool anyLessEqual(ScalarBatch const& lhs, ScalarBatch const& rhs) {
    return (lhs.lanes < rhs.lanes).any();
  }

  static bool anyLessEqual(ScalarBatch const& lhs, Scalar const& rhs) {
    return (lhs.lanes < rhs).any();
  }

  static bool allLessEqual(ScalarBatch const& lhs, ScalarBatch const& rhs) {
    return (lhs.lanes < rhs.lanes).all();
  }

  static bool allLessEqual(ScalarBatch const& lhs, Scalar const& rhs) {
    return (lhs.lanes < rhs).all();
  }
};

}  // namespace sophus2

namespace Eigen {

// This is mirrored from ceres::Jet.
template <class TT, int kNum>
struct NumTraits<sophus2::Batch<TT, kNum>> {
  using Real = sophus2::Batch<TT, kNum>;
  using NonInteger = sophus2::Batch<TT, kNum>;
  using Nested = sophus2::Batch<TT, kNum>;
  using Literal = sophus2::Batch<TT, kNum>;
  enum {
    IsComplex = 0,
    IsInteger = 0,
    IsSigned,
    ReadCost = 1,
    AddCost = 1,
    MulCost = 2,
    HasFloatingPoint = 1,
    RequireInitialization = 1
  };

  static sophus2::Batch<TT, kNum> dummy_precision() {
    return sophus2::Batch<TT, kNum>(1e-12);
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
struct ScalarBinaryOpTraits<sophus2::Batch<TT, kNum>, TT, BinaryOp> {
  using ReturnType = sophus2::Batch<TT, kNum>;
};
template <class BinaryOp, class TT, int kNum>
struct ScalarBinaryOpTraits<TT, sophus2::Batch<TT, kNum>, BinaryOp> {
  using ReturnType = sophus2::Batch<TT, kNum>;
};

}  // namespace Eigen
