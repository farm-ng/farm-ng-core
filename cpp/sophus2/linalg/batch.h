// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sophus2/common/common.h"
#include "sophus2/concepts/batch.h"

#include <fmt/ostream.h>

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

  // This is required and we cannot change the defintion, for Batch to act as a
  // scalar.
  bool operator==(Batch<TT, kNum> const& y) {
    return (this->lanes == y.lanes).all();
  }

  Batch<bool, kNum> cwLessEqual(Batch<TT, kNum> const& rhs) const {
    return Batch<bool, kNum>((this->lanes <= rhs.lanes).eval());
  }

  Batch<bool, kNum> cwLess(Batch<TT, kNum> const& rhs) const {
    return Batch<bool, kNum>((this->lanes < rhs.lanes).eval());
  }

  Batch<bool, kNum> cwGreaterEqual(Batch<TT, kNum> const& rhs) const {
    return Batch<bool, kNum>((this->lanes >= rhs.lanes).eval());
  }

  Batch<bool, kNum> cwGreater(Batch<TT, kNum> const& rhs) const {
    return Batch<bool, kNum>((this->lanes > rhs.lanes).eval());
  }

  template <class TOtherScalar>
  Batch<TOtherScalar, kNum> select(
      Batch<TOtherScalar, kNum> const& lhs,
      Batch<TOtherScalar, kNum> const& rhs) const {
    return Batch<TOtherScalar, kNum>(this->lanes.select(lhs.lanes, rhs.lanes));
  }

  template <class TOtherScalar, int kRows, int kCols>
  Eigen::Matrix<Batch<TOtherScalar, kNum>, kRows, kCols> select(
      Eigen::Matrix<Batch<TOtherScalar, kNum>, kRows, kCols> const& lhs,
      Eigen::Matrix<Batch<TOtherScalar, kNum>, kRows, kCols> const& rhs) const {
    static_assert(kRows >= 0);
    static_assert(kCols >= 0);

    Eigen::Matrix<Batch<TOtherScalar, kNum>, kRows, kCols> result;
    for (int r = 0; r < kRows; ++r) {
      for (int c = 0; c < kCols; ++c) {
        result(r, c) = this->select(lhs(r, c), rhs(r, c));
      }
    }
    return result;
  }

  Eigen::Array<TT, kNum, 1> lanes;
};

template <class TT, int kNum>
auto operator<<(std::ostream& os, Batch<TT, kNum> const& batch)
    -> std::ostream& {
  os << "[" << batch.lanes.transpose() << "]";
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
inline Batch<T, N> tan(Batch<T, N> const& f) {
  return Batch{f.lanes.tan().eval()};
}

template <class T, int N>
inline Batch<T, N> atan(Batch<T, N> const& f) {
  return Batch{f.lanes.atan().eval()};
}

template <class T, int N>
inline Batch<T, N> pow(Batch<T, N> const& f, T v) {
  return Batch{f.lanes.pow(v).eval()};
}

template <class T, int N>
inline Batch<T, N> atan2(Batch<T, N> const& lhs, Batch<T, N> const& rhs) {
  Eigen::Array<T, N, 1> result;
  using std::atan2;

  // TODO: See whether there is a more efficient way.
  //       First we would want to confirm that the other functions (tan, sqrt)
  //       have efficient versions - by inspecting the Eigen implementation
  //       for relevant intrinsics (AMD64 and ARM64).
  for (int i = 0; i < N; ++i) {
    result[i] = atan2(lhs.lanes[i], rhs.lanes[i]);
  }

  return Batch{result};
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
inline Batch<TT, N> operator/(Batch<TT, N> const& f, TT s) {
  return Batch{(f.lanes / s).eval()};
}

template <class TT, int N>
inline Batch<TT, N> operator*(TT s, Batch<TT, N> const& f) {
  return Batch{(s / f.lanes).eval()};
}

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

template <class TScalar, int kRows, int kBatchSize>
using BatchVector = Eigen::Vector<Batch<TScalar, kBatchSize>, kRows>;

template <class TScalar, int kRows, int kCols, int kBatchSize>
using BatchMatrix = Eigen::Matrix<Batch<TScalar, kBatchSize>, kRows, kCols>;

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

// Defined in fmt namespace due to gcc bug
// https://stackoverflow.com/a/69144223
namespace fmt {

template <class TT, int kNum>
struct formatter<sophus2::Batch<TT, kNum>> : ostream_formatter {};
}  // namespace fmt
