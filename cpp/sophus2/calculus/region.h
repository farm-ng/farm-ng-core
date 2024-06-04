// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sophus2/common/common.h"
#include "sophus2/linalg/vector_space.h"
#include "sophus2/linalg/vector_space_traits.h"

namespace sophus2 {

template <concepts::PointType TPoint>
class Region;

using RegionI = Region<int>;
using RegionF32 = Region<float>;
using RegionF64 = Region<double>;

template <concepts::ScalarType TScalar>
using Region2 = Region<Eigen::Array<TScalar, 2, 1>>;
using Region2I = Region2<int>;
using Region2F32 = Region2<float>;
using Region2F64 = Region2<double>;

template <concepts::ScalarType TScalar>
using Region3 = Region<Eigen::Array<TScalar, 3, 1>>;
using Region3I = Region3<int>;
using Region3F32 = Region3<float>;
using Region3F64 = Region3<double>;

template <concepts::ScalarType TScalar>
using Region4 = Region<Eigen::Array<TScalar, 4, 1>>;
using Region4I = Region4<int>;
using Region4F32 = Region4<float>;
using Region4F64 = Region4<double>;

/// A region is a closed interval [a, b] with a being the lower bound (=min) and
/// b being the upper bound (=max).
///
/// Here, the bounds a, b are either both scalars (e.g. floats, doubles)
/// or fixed length vectors/arrays (such as Eigen::Vector3f or Eigen::Array2d).
///
/// Special case for integer numbers:
///
///    An integer number X is considered being equivalent to a real region
///    [X-0.5, X+0.5].
template <concepts::PointType TPoint>
class Region {
 public:
  /// Vector space over which the interval/region is defined.
  using Point = TPoint;
  /// Scalar type.
  using Scalar = typename PointTraits<TPoint>::Scalar;
  /// Is Scalar an integer?
  static bool constexpr kIsInteger = PointTraits<TPoint>::kIsInteger;

  /// Number of rows.
  static int constexpr kRows = PointTraits<TPoint>::kRows;
  /// Number of columns.
  static int constexpr kCols = PointTraits<TPoint>::kCols;
  static_assert(kRows >= 1);
  static_assert(kCols >= 1);
  /// Number of dimensions.
  static int constexpr kDim = kRows * kCols;

  /// Creates an uninitialized region.
  static auto constexpr uninitialized() noexcept -> Region<TPoint> {
    return Region(UninitTag{});
  }

  /// Creates and empty region.
  static auto constexpr empty() noexcept -> Region<TPoint> {
    auto region = Region<TPoint>::uninitialized();
    region.min_max_ = {
        PointTraits<TPoint>::max(), PointTraits<TPoint>::lowest()};
    return region;
  }

  /// Creates unbounded region.
  ///
  /// If TPoint is floating point, the region is [-inf, +inf].
  static auto constexpr unbounded() noexcept -> Region<TPoint> {
    auto region = Region<TPoint>::uninitialized();
    region.min_max_ = {
        PointTraits<TPoint>::lowest(), PointTraits<TPoint>::max()};
    return region;
  }

  /// Creates a region from a given point.
  ///
  /// If TPoint is a floating point then the region is considered degenerated.
  static auto constexpr from(TPoint const& p) noexcept -> Region<TPoint> {
    auto region = Region<TPoint>::empty();
    SOPHUS_ASSERT(!isNan(p));
    region.min_max_[0] = p;
    region.min_max_[1] = p;
    return region;
  }

  /// Creates Region from two points, min and max.
  ///
  /// The points min, max need not to be ordered. After construction it will be
  /// true that this->min() <= this->max().
  static auto constexpr fromMinMax(
      TPoint const& min, TPoint const& max) noexcept -> Region<TPoint> {
    auto region = Region<TPoint>::empty();
    SOPHUS_ASSERT(!isNan(min));
    SOPHUS_ASSERT(!isNan(max));
    region.extend(min);
    region.extend(max);
    return region;
  }

  /// Convenient constructor to create a  Segment from two points p1 and p2.
  ///
  /// The points need not to be ordered. After construction it will be true
  /// that this->min() <= this->max().
  ///
  /// Note: This constructor is only available for scalar regions. For
  /// multi-dim regions, use the  fromMinMax() factory instead.,
  template <class TScalar>
  requires std::is_same_v<TPoint, TScalar>
  constexpr Region(TScalar const& p1, TScalar const& p2) noexcept
      : min_max_{p1, p1} {
    SOPHUS_ASSERT(!isNan(p1));
    SOPHUS_ASSERT(!isNan(p2));
    this->extend(p2);
  }

  /// Creates region over a 2-dimensional vector space by providing
  /// component-wise intervals:
  ///  - interval segment along X
  ///  - interval segment along Y
  template <class TScalar>
  static auto constexpr createPerAxis(
      Region<TScalar> const& segment_x,
      Region<TScalar> const& segment_y) noexcept -> Region2<TScalar> {
    auto region = Region<TPoint>::uninitialized();
    region.setElem(segment_x, 0);
    region.setElem(segment_y, 1);
    return region;
  }

  /// Creates region over a 3-dimensional vector space by providing
  /// component-wise intervals:
  ///  - interval segment along X
  ///  - interval segment along Y
  ///  - interval segment along Z
  template <class TScalar>
  static auto constexpr createPerAxis(
      Region<TScalar> const& segment_x,
      Region<TScalar> const& segment_y,
      Region<TScalar> const& segment_z) noexcept -> Region2<TScalar> {
    auto region = Region<TPoint>::uninitialized();
    region.setElem(segment_x, 0);
    region.setElem(segment_y, 1);
    region.setElem(segment_z, 2);
    return region;
  }

  /// Creates region over a 4-dimensional vector space by providing
  /// component-wise intervals:
  ///  - interval segment along dimension 0
  ///  - interval segment along dimension 1
  ///  - interval segment along dimension 2
  ///  - interval segment along dimension 3
  template <class TScalar>
  static auto constexpr createPerAxis(
      Region<TScalar> const& segment_0,
      Region<TScalar> const& segment_1,
      Region<TScalar> const& segment_2,
      Region<TScalar> const& segment_3) noexcept -> Region2<TScalar> {
    auto region = Region<TPoint>::uninitialized();
    region.setElem(segment_0, 0);
    region.setElem(segment_1, 1);
    region.setElem(segment_2, 2);
    region.setElem(segment_3, 3);
    return region;
  }

  /// Precondition: !this->isEmptpy()
  [[nodiscard]] auto constexpr getElem(size_t row) const
      -> Region<Scalar> const& {
    return {
        SOPHUS_UNWRAP(tryGetElem(min(), row)),
        SOPHUS_UNWRAP(tryGetElem(max(), row))};
  }

  /// Precondition: !this->isEmptpy()
  void constexpr setElem(size_t row, Region<Scalar> const& s) {
    SOPHUS_UNWRAP(trySetElem(min(), s, row));
    SOPHUS_UNWRAP(trySetElem(max(), s, row));
  }

  /// Returns the lower bound of the region.
  ///
  /// Precondition: !this->isEmptpy()
  [[nodiscard]] auto constexpr min() const noexcept -> TPoint const& {
    SOPHUS_ASSERT(!this->isEmpty());
    return min_max_[0];
  }

  /// Returns the upper bound of the region.
  ///
  /// Precondition: !this->isEmptpy()
  [[nodiscard]] auto constexpr max() const noexcept -> TPoint const& {
    SOPHUS_ASSERT(!this->isEmpty());
    return min_max_[1];
  }

  /// Returns the lower bound of the region if it exit and nullopt otherwise.
  [[nodiscard]] auto constexpr tryMin() const noexcept
      -> std::optional<TPoint> {
    if (isEmpty()) {
      return std::nullopt;
    }
    return min_max_[0];
  }

  /// Returns the lower bound of the region if it exists and nullopt otherwise.
  [[nodiscard]] auto constexpr tryMax() const noexcept
      -> std::optional<TPoint> {
    if (isEmpty()) {
      return std::nullopt;
    }
    return min_max_[1];
  }

  /// Returns the clamped version of the given point.
  ///
  /// Precondition: !this->isEmptpy()
  [[nodiscard]] auto constexpr clamp(TPoint const& point) const noexcept
      -> TPoint {
    return sophus2::clamp(point, min(), max());
  }

  /// Returns true if the region contains the given point.
  [[nodiscard]] auto constexpr contains(TPoint const& point) const noexcept
      -> bool {
    if (isEmpty()) {
      return false;
    }
    return isLessEqual(min(), point) && isLessEqual(point, max());
  }

  /// Returns the range of the region.
  ///
  /// It is zero if the region is not proper.
  [[nodiscard]] auto constexpr range() const noexcept -> TPoint {
    if (isEmpty()) {
      return zero<TPoint>();
    }
    if constexpr (kIsInteger) {
      //  For integers, we consider e.g. {2} == [1.5, 2.5] hence range of
      //  Region(2, 3) == [1.5, 3.5] is 2.
      return plus(eval(max() - min()), 1);
    }
    return eval(max() - min());
  }

  /// Returns the mid point.
  ///
  /// Note: If TPoint is an integer point then the result will be rounded to
  /// the closed integer.
  ///
  /// Precondition: !this->isEmptpy()
  [[nodiscard]] auto constexpr mid() const noexcept -> TPoint {
    return eval(min() + range() / 2);
  }

  /// Extends this by other region.
  auto constexpr extend(Region const& other) noexcept -> Region<TPoint>& {
    if (other.isEmpty()) {
      return *this;
    }
    if (this->isEmpty()) {
      *this = other;
      return *this;
    }
    min_max_[0] = sophus2::min(min(), other.min());
    min_max_[1] = sophus2::max(max(), other.max());
    return *this;
  }

  /// Extends this by given point.
  auto constexpr extend(TPoint const& point) noexcept -> Region<TPoint>& {
    if (this->isEmpty()) {
      *this = from(point);
    } else {
      min_max_[0] = sophus2::min(min(), point);
      min_max_[1] = sophus2::max(max(), point);
    }
    return *this;
  }

  /// Returns translated region.
  [[nodiscard]] auto constexpr translated(TPoint const& p) const noexcept
      -> Region<TPoint> {
    return Region<TPoint>::fromMinMax(min_max_[0] + p, min_max_[1] + p);
  }

  /// Cast from current TPoint to other TOtherPoint:
  ///
  /// Supported cases:
  ///
  /// - floating point to floating point (e.g. double to float)
  /// - integer to integer (e.g. int16 to int64)
  /// - integer to float
  ///    * example: [2, 5] -> [1.5, 5.5]
  ///
  /// Note: floating point to integer is not supported and shall not compile.
  ///       Use either encloseCast or roundCast.
  template <class TOtherPoint>
  auto constexpr cast() const noexcept -> Region<TOtherPoint> {
    if (isEmpty()) {
      return Region<TOtherPoint>::empty();
    }
    if constexpr (kIsInteger == Region<TOtherPoint>::kIsInteger) {
      // case 1: floating => floating  and integer => integer is trivial
      return Region<TOtherPoint>(
          sophus2::cast<TOtherPoint>(min()), sophus2::cast<TOtherPoint>(max()));
    }
    if constexpr (kIsInteger && !Region<TOtherPoint>::kIsInteger) {
      // case 2: integer to floating.
      //
      // example: [2, 5] -> [1.5, 5.5]
      return Region<TOtherPoint>(
          plus(sophus2::cast<TOtherPoint>(min()), -0.5),
          plus(sophus2::cast<TOtherPoint>(max()), 0.5));
    }
    // case 3: floating to integer.
    static_assert(
        kIsInteger || !Region<TOtherPoint>::kIsInteger,
        "For floating to integer: call encloseCast() or roundCast() "
        "instead.");
  }

  /// Returns the smallest integer region which contains this.
  ///
  /// example: [1.2, 1.3] -> [1, 2]
  template <class TOtherPoint>
  auto constexpr encloseCast() const noexcept -> Region<TOtherPoint> {
    static_assert(!kIsInteger && Region<TOtherPoint>::kIsInteger);
    if (isEmpty()) {
      return Region<TOtherPoint>::empty();
    }
    return Region<TOtherPoint>(
        sophus2::cast<TOtherPoint>(sophus2::floor(min())),
        sophus2::cast<TOtherPoint>(sophus2::ceil(max())));
  }

  /// Rounds given region bounds and returns resulting integer region.
  ///
  /// example: [1.2, 2.3] -> [1, 2]
  /// example: [1.1, 2.7] -> [1, 3]
  template <class TOtherPoint>
  auto constexpr roundCast() const noexcept -> Region<TOtherPoint> {
    if (isEmpty()) {
      return Region<TOtherPoint>::empty();
    }
    static_assert(!kIsInteger && Region<TOtherPoint>::kIsInteger);
    return Region<TOtherPoint>(
        sophus2::cast<TOtherPoint>(sophus2::round(min())),
        sophus2::cast<TOtherPoint>(sophus2::round(max())));
  }

  /// Returns true if region is empty.
  [[nodiscard]] auto constexpr isEmpty() const noexcept -> bool {
    return allTrue(min_max_[0] == PointTraits<TPoint>::max()) &&
           allTrue(min_max_[1] == PointTraits<TPoint>::lowest());
  }

  /// Returns true if region contains a single floating point number.
  [[nodiscard]] auto constexpr isDegenerated() const noexcept -> bool {
    return !kIsInteger && allTrue(min_max_[0] == min_max_[1]);
  }

  /// Returns true if region is neither empty nor degenerated.
  /// Hence it contains a range of values.
  [[nodiscard]] auto constexpr isProper() const noexcept -> bool {
    return !this->isEmpty() && !this->isDegenerated();
  }

  /// Returns true if region has no bounds.
  [[nodiscard]] auto constexpr isUnbounded() const noexcept -> bool {
    return allTrue(min_max_[0] == PointTraits<TPoint>::lowest()) &&
           allTrue(min_max_[1] == PointTraits<TPoint>::max());
  }

 private:
  explicit constexpr Region(UninitTag /*unused*/) {}

  // invariant: this->isEmpty() or min_max[0] <= min_max[1]
  std::array<TPoint, 2> min_max_{};
};

template <class TT>
auto constexpr operator==(Region<TT> const& lhs, Region<TT> const& rhs)
    -> bool {
  return lhs.min() == rhs.min() && lhs.max() == rhs.max();
}

}  // namespace sophus2
