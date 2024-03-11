// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sophus2/interp/spline/common.h"
#include "sophus2/interp/spline/details/bspline_segment.h"

namespace sophus2 {
template <class TScalar, int kDim>
class CubicBSplineImpl {
 public:
  using Scalar = TScalar;
  using Vector = Eigen::Vector<Scalar, kDim>;

  CubicBSplineImpl(std::vector<Vector> const& control_points, double delta_t)
      : control_points_(control_points), delta_t_(delta_t) {
    SOPHUS_ASSERT(
        control_points.size() >= 2u, ", but {}", control_points.size());
  }

  [[nodiscard]] Vector interpolate(int i, double u) const {
    SOPHUS_ASSERT_GE(i, 0);
    SOPHUS_ASSERT_LE(i, this->getNumSegments());

    details::SegmentCase segment_case =
        i == 0
            ? details::SegmentCase::first
            : (i == this->getNumSegments() - 1 ? details::SegmentCase::last
                                               : details::SegmentCase::normal);

    int idx_prev = std::max(0, i - 1);
    int idx_0 = i;
    int idx_1 = i + 1;
    int idx_2 = std::min(i + 2, int(this->control_points_.size()) - 1);

    return details::CubicBSplineSegment<Scalar, kDim>(
               segment_case,
               control_points_[idx_prev],
               control_points_[idx_0],
               control_points_[idx_1],
               control_points_[idx_2])
        .interpolate(u);
  }

  [[nodiscard]] Eigen::Matrix<Scalar, kDim, kDim> dxiInterpolate(
      int i, double u, int control_point_idx) const {
    SOPHUS_ASSERT_GE(i, 0);
    SOPHUS_ASSERT_LE(i, this->getNumSegments());

    details::SegmentCase segment_case =
        i == 0
            ? details::SegmentCase::first
            : (i == this->getNumSegments() - 1 ? details::SegmentCase::last
                                               : details::SegmentCase::normal);

    int idx_prev = std::max(0, i - 1);
    int idx_0 = i;
    int idx_1 = i + 1;
    int idx_2 = std::min(i + 2, int(this->control_points_.size()) - 1);

    details::CubicBSplineSegment<Scalar, kDim> spline_segment(
        segment_case,
        control_points_[idx_prev],
        control_points_[idx_0],
        control_points_[idx_1],
        control_points_[idx_2]);

    Eigen::Matrix<Scalar, kDim, kDim> dxi;
    dxi.setZero();

    if (idx_prev == control_point_idx) {
      dxi += spline_segment.dxiInterpolate(u, 0);
    }
    if (idx_0 == control_point_idx) {
      dxi += spline_segment.dxiInterpolate(u, 1);
    }
    if (idx_1 == control_point_idx) {
      dxi += spline_segment.dxiInterpolate(u, 2);
    }
    if (idx_2 == control_point_idx) {
      dxi += spline_segment.dxiInterpolate(u, 3);
    }
    return dxi;
  }

  [[nodiscard]] std::vector<Vector> const& controlPoints() const {
    return control_points_;
  }

  std::vector<Vector>& controlPoint() { return control_points_; }

  [[nodiscard]] int getNumSegments() const {
    return int(control_points_.size()) - 1;
  }

  [[nodiscard]] double deltaT() const { return delta_t_; }

 private:
  std::vector<Vector> control_points_;
  double delta_t_;
};

template <class TScalar, int kDim>
class CubicBSpline {
 public:
  using Scalar = TScalar;
  using Vector = Eigen::Vector<Scalar, kDim>;

  CubicBSpline(std::vector<Vector> control_points, double t0, double delta_t)
      : impl_(std::move(control_points), delta_t), t0_(t0) {}

  [[nodiscard]] Vector interpolate(double t) const {
    SegmentCoordinate index_and_u = this->indexAndU(t);
    return impl_.interpolate(index_and_u.segment_idx, index_and_u.fraction);
  }

  [[nodiscard]] Eigen::Matrix<Scalar, kDim, kDim> dxiInterpolate(
      double t, int control_point_idx) const {
    SegmentCoordinate index_and_u = this->indexAndU(t);
    return impl_.dxiInterpolate(
        index_and_u.segment_idx, index_and_u.fraction, control_point_idx);
  }

  [[nodiscard]] double t0() const { return t0_; }

  [[nodiscard]] double tmax() const {
    return t0_ + impl_.deltaT() * getNumSegments();
  }

  [[nodiscard]] std::vector<Vector> const& controlPoints() const {
    return impl_.controlPoints();
  }

  std::vector<Vector>& controlPoints() { return impl_.controlPoints(); }

  [[nodiscard]] int getNumSegments() const { return impl_.getNumSegments(); }

  [[nodiscard]] double s(double t) const { return (t - t0_) / impl_.deltaT(); }

  [[nodiscard]] double deltaT() const { return impl_.deltaT(); }

  [[nodiscard]] SegmentCoordinate indexAndU(double t) const {
    SOPHUS_ASSERT_GE(t, t0_);
    SOPHUS_ASSERT_LE(t, this->tmax());

    double s = this->s(t);
    double segment_idx = NAN;
    SegmentCoordinate index_and_u;
    index_and_u.fraction = std::modf(s, &segment_idx);
    index_and_u.segment_idx = int(segment_idx);
    if (index_and_u.fraction > sophus2::kEpsilonF64) {
      return index_and_u;
    }

    // u ~=~ 0.0
    if (index_and_u.segment_idx < getNumSegments() / 2) {
      // First half of spline, keep as is (i, 0.0).
      return index_and_u;
    }
    // Second half of spline, use (i-1, 1.0) instead. This way we can
    // represent t == tmax (and not just t<tmax).
    index_and_u.fraction += 1.0;
    --index_and_u.segment_idx;

    return index_and_u;
  }

 private:
  CubicBSplineImpl<Scalar, kDim> impl_;

  double t0_;
};

}  // namespace sophus2
