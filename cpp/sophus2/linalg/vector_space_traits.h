// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sophus2/concepts/point.h"

#include <Eigen/Core>

#include <limits>

namespace sophus2 {

/// Point trait class template.
///
/// Mathematically, we consider a point an element of a "vector space".
template <class TPoint>
struct PointTraits;

/// Trait for a multi-dimensional point.
///
/// Mathematically, we consider a point an element of a "vector space".
template <concepts::EigenDenseType TPoint>
struct PointTraits<TPoint> {
  /// Field over which the vector space is defined
  using Scalar = typename TPoint::Scalar;

  /// number of rows - for the (kRows x kCols) dimensional vector space
  static int constexpr kRows = TPoint::RowsAtCompileTime;
  /// number of columns - for the (kRows x kCols) dimensional vector space
  static int constexpr kCols = TPoint::ColsAtCompileTime;

  /// Is Scalar a floating point number?
  static bool constexpr kIsFloatingPoint = std::is_floating_point_v<Scalar>;
  /// Is Scalar an integer number?
  static bool constexpr kIsInteger = std::is_integral_v<Scalar>;

  /// Scalar set contains infinity?
  static bool constexpr kHasInfinity =
      std::numeric_limits<Scalar>::has_infinity;
  /// Scalar set quite NAN?
  static bool constexpr kHasQuietNan =
      std::numeric_limits<Scalar>::has_quiet_NaN;
  /// Scalar set contains signaling NAN?
  static bool constexpr kHasSignalingNan =
      std::numeric_limits<Scalar>::has_signaling_NaN;

  /// Lowest value (e.g. typically large negative number)
  static TPoint lowest() {
    return TPoint::Constant(std::numeric_limits<Scalar>::lowest());
  };
  /// Smallest positive value
  static TPoint min() {
    return TPoint::Constant(std::numeric_limits<Scalar>::min());
  };
  /// Smallest positive value
  static TPoint max() {
    return TPoint::Constant(std::numeric_limits<Scalar>::max());
  };
  /// Greatest value
  static TPoint epsilon() {
    return TPoint::Constant(std::numeric_limits<Scalar>::epsilon());
  };
  /// ... plus a bunch more if we need them
};

// BEGIN(exclude from doxygen) \cond HIDDEN_SYMBOLS

// Single scalar specialization (1-dim case) of PointTraits
template <concepts::ScalarType TPoint>
struct PointTraits<TPoint> {
  using Scalar = TPoint;

  static bool constexpr kIsFloatingPoint = std::is_floating_point_v<Scalar>;
  static bool constexpr kIsInteger = std::is_integral_v<Scalar>;

  static int constexpr kRows = 1;
  static int constexpr kCols = 1;

  static bool constexpr kHasInfinity =
      std::numeric_limits<Scalar>::has_infinity;
  static bool constexpr kHasQuietNan =
      std::numeric_limits<Scalar>::has_quiet_NaN;
  static bool constexpr kHasSignalingNan =
      std::numeric_limits<Scalar>::has_signaling_NaN;

  static constexpr TPoint lowest() {
    return std::numeric_limits<Scalar>::lowest();
  };
  static constexpr TPoint min() { return std::numeric_limits<Scalar>::min(); };
  static constexpr TPoint max() { return std::numeric_limits<Scalar>::max(); };
};
// END(exclude from doxygen) \endcond

}  // namespace sophus2
