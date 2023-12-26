// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

/// @file
// Basis spline implementation on Lie Group following:
// S. Lovegrove, A. Patron-Perez, G. Sibley, BMVC 2013
// http://www.bmva.org/bmvc/2013/Papers/paper0093/paper0093.pdf

#pragma once

#include "sophus/interp/spline/common.h"
#include "sophus/interp/spline/details/group_bspline_segment.h"

namespace sophus {

template <concepts::LieGroup TGroup>
class CubicLieGroupBSplineFn {
 public:
  using LieGroup = TGroup;
  static int const kDof = LieGroup::kDof;
  using Scalar = typename LieGroup::Scalar;
  using Transformation =
      Eigen::Matrix<Scalar, TGroup::kAmbientDim, TGroup::kAmbientDim>;
  using Tangent = typename LieGroup::Tangent;

  static LieGroup parentFromSpline(
      LieGroup const& parent_ts_control_point,
      std::tuple<Tangent, Tangent, Tangent> const& control_tangent_vectors,
      double u) {
    auto aa = a(control_tangent_vectors, u);
    return parent_ts_control_point * std::get<0>(aa) * std::get<1>(aa) *
           std::get<2>(aa);
  }

  static Transformation dtParentFromSpline(
      LieGroup const& parent_ts_control_point,
      std::tuple<Tangent, Tangent, Tangent> const& control_tangent_vectors,
      double u,
      double delta_t) {
    auto aa = a(control_tangent_vectors, u);
    auto dt_aa = dtA(aa, control_tangent_vectors, u, delta_t);
    return parent_ts_control_point.matrix() *
           ((std::get<0>(dt_aa) * std::get<1>(aa).matrix() *
             std::get<2>(aa).matrix()) +
            (std::get<0>(aa).matrix() * std::get<1>(dt_aa) *
             std::get<2>(aa).matrix()) +
            (std::get<0>(aa).matrix() * std::get<1>(aa).matrix() *
             std::get<2>(dt_aa)));
  }

  static Transformation dt2ParentFromSpline(
      LieGroup const& parent_ts_control_point,
      std::tuple<Tangent, Tangent, Tangent> const& control_tangent_vectors,
      double u,
      double delta_t) {
    using TScalar = typename LieGroup::Scalar;
    auto aa = a(control_tangent_vectors, u);
    auto dt_aa = dtA(aa, control_tangent_vectors, u, delta_t);
    auto dt2_aa = dt2A(aa, dt_aa, control_tangent_vectors, u, delta_t);

    return parent_ts_control_point.matrix() *
           ((std::get<0>(dt2_aa) * std::get<1>(aa).matrix() *
             std::get<2>(aa).matrix()) +
            (std::get<0>(aa).matrix() * std::get<1>(dt2_aa) *
             std::get<2>(aa).matrix()) +
            (std::get<0>(aa).matrix() * std::get<1>(aa).matrix() *
             std::get<2>(dt2_aa)) +
            TScalar(2) * ((std::get<0>(dt_aa) * std::get<1>(dt_aa) *
                           std::get<2>(aa).matrix()) +
                          (std::get<0>(dt_aa) * std::get<1>(aa).matrix() *
                           std::get<2>(dt_aa)) +
                          (std::get<0>(aa).matrix() * std::get<1>(dt_aa) *
                           std::get<2>(dt_aa))));
  }

  static std::tuple<LieGroup, LieGroup, LieGroup> a(
      std::tuple<Tangent, Tangent, Tangent> const& control_tangent_vectors,
      double u) {
    Eigen::Vector3d b = details::CubicBSplineBasis<double>::b(u);
    return std::make_tuple(
        LieGroup::exp(b[0] * std::get<0>(control_tangent_vectors)),
        LieGroup::exp(b[1] * std::get<1>(control_tangent_vectors)),
        LieGroup::exp(b[2] * std::get<2>(control_tangent_vectors)));
  }

  static Transformation dx1A(
      Tangent const& control_tangent_vectors, double u, int i) {
    Eigen::Vector3d b = details::CubicBSplineBasis<double>::b(u);
    Eigen::Vector<Scalar, kDof> t;
    t[i] = Scalar(1.0);
    return LieGroup::hat(t) * LieGroup::exp(b[0] * t).matrix();
  }

  static std::tuple<Transformation, Transformation, Transformation> dtA(
      std::tuple<LieGroup, LieGroup, LieGroup> const& aa,
      std::tuple<Tangent, Tangent, Tangent> const& control_tangent_vectors,
      double u,
      double delta_t) {
    Eigen::Vector3d dt_b = details::CubicBSplineBasis<double>::dtB(u, delta_t);
    return std::make_tuple(
        dt_b[0] * std::get<0>(aa).matrix() *
            LieGroup::hat(std::get<0>(control_tangent_vectors)),
        dt_b[1] * std::get<1>(aa).matrix() *
            LieGroup::hat(std::get<1>(control_tangent_vectors)),
        dt_b[2] * std::get<2>(aa).matrix() *
            LieGroup::hat(std::get<2>(control_tangent_vectors)));
  }

  static std::tuple<Transformation, Transformation, Transformation> dt2A(
      std::tuple<LieGroup, LieGroup, LieGroup> const& aa,
      std::tuple<Transformation, Transformation, Transformation> const& dt_aa,
      std::tuple<Tangent, Tangent, Tangent> const& control_tangent_vectors,
      double u,
      double delta_t) {
    Eigen::Vector3d dt_b = details::CubicBSplineBasis<double>::dtB(u, delta_t);
    Eigen::Vector3d dt2_b =
        details::CubicBSplineBasis<double>::dt2B(u, delta_t);

    return std::make_tuple(
        (dt_b[0] * std::get<0>(dt_aa).matrix() +
         dt2_b[0] * std::get<0>(aa).matrix()) *
            LieGroup::hat(std::get<0>(control_tangent_vectors)),
        (dt_b[1] * std::get<1>(dt_aa).matrix() +
         dt2_b[1] * std::get<1>(aa).matrix()) *
            LieGroup::hat(std::get<1>(control_tangent_vectors)),
        (dt_b[2] * std::get<2>(dt_aa).matrix() +
         dt2_b[2] * std::get<2>(aa).matrix()) *
            LieGroup::hat(std::get<2>(control_tangent_vectors)));
  }
};

template <concepts::LieGroup TGroup>
class CubicLieGroupBSplineImpl {
 public:
  using LieGroup = TGroup;
  using Scalar = typename LieGroup::Scalar;
  using Transformation =
      Eigen::Matrix<Scalar, TGroup::kAmbientDim, TGroup::kAmbientDim>;
  using Tangent = typename LieGroup::Tangent;

  CubicLieGroupBSplineImpl(
      std::vector<LieGroup> const& parent_ts_control_point, double delta_t)
      : parent_from_control_point_transforms_(parent_ts_control_point),
        delta_transform_(delta_t) {
    SOPHUS_ASSERT(
        parent_from_control_point_transforms_.size() >= 2u,
        ", but {}",
        parent_from_control_point_transforms_.size());
  }

  [[nodiscard]] LieGroup parentFromSpline(int i, double u) const {
    SOPHUS_ASSERT(i >= 0, "i = {}", i);
    SOPHUS_ASSERT(
        i < this->getNumSegments(),
        "i = {};  this->getNumSegments() = {};  "
        "parent_from_control_point_transforms_.size() = {}",
        i,
        this->getNumSegments(),
        parent_from_control_point_transforms_.size());

    details::SegmentCase segment_case =
        i == 0
            ? details::SegmentCase::first
            : (i == this->getNumSegments() - 1 ? details::SegmentCase::last
                                               : details::SegmentCase::normal);

    int idx_prev = std::max(0, i - 1);
    int idx_0 = i;
    int idx_1 = i + 1;
    int idx_2 = std::min(
        i + 2, int(this->parent_from_control_point_transforms_.size()) - 1);

    return details::CubicLieGroupBSplineSegment<LieGroup>(
               segment_case,
               parent_from_control_point_transforms_[idx_prev].params(),
               parent_from_control_point_transforms_[idx_0].params(),
               parent_from_control_point_transforms_[idx_1].params(),
               parent_from_control_point_transforms_[idx_2].params())
        .parentFromSpline(u);
  }

  [[nodiscard]] Transformation dtParentFromSpline(int i, double u) const {
    SOPHUS_ASSERT(i >= 0, "i = {}", i);
    SOPHUS_ASSERT(
        i < this->getNumSegments(),
        "i = {};  this->getNumSegments() = {};  "
        "parent_from_control_point_transforms_.size() = {}",
        i,
        this->getNumSegments(),
        parent_from_control_point_transforms_.size());

    details::SegmentCase segment_case =
        i == 0
            ? details::SegmentCase::first
            : (i == this->getNumSegments() - 1 ? details::SegmentCase::last
                                               : details::SegmentCase::normal);

    int idx_prev = std::max(0, i - 1);
    int idx_0 = i;
    int idx_1 = i + 1;
    int idx_2 = std::min(
        i + 2, int(this->parent_from_control_point_transforms_.size()) - 1);

    return details::CubicLieGroupBSplineSegment<LieGroup>(
               segment_case,
               parent_from_control_point_transforms_[idx_prev].params(),
               parent_from_control_point_transforms_[idx_0].params(),
               parent_from_control_point_transforms_[idx_1].params(),
               parent_from_control_point_transforms_[idx_2].params())
        .dtParentFromSpline(u, delta_transform_);
  }

  [[nodiscard]] Transformation dt2ParentFromSpline(int i, double u) const {
    SOPHUS_ASSERT(i >= 0, "i = {}", i);
    SOPHUS_ASSERT(
        i < this->getNumSegments(),
        "i = {};  this->getNumSegments() = {};  "
        "parent_from_control_point_transforms_.size() = {}",
        i,
        this->getNumSegments(),
        parent_from_control_point_transforms_.size());

    details::SegmentCase segment_case =
        i == 0
            ? details::SegmentCase::first
            : (i == this->getNumSegments() - 1 ? details::SegmentCase::last
                                               : details::SegmentCase::normal);

    int idx_prev = std::max(0, i - 1);
    int idx_0 = i;
    int idx_1 = i + 1;
    int idx_2 = std::min(
        i + 2, int(this->parent_from_control_point_transforms_.size()) - 1);

    return details::CubicLieGroupBSplineSegment<LieGroup>(
               segment_case,
               parent_from_control_point_transforms_[idx_prev].params(),
               parent_from_control_point_transforms_[idx_0].params(),
               parent_from_control_point_transforms_[idx_1].params(),
               parent_from_control_point_transforms_[idx_2].params())
        .dt2ParentFromSpline(u, delta_transform_);
  }

  [[nodiscard]] std::vector<LieGroup> const& parentFromsControlPoint() const {
    return parent_from_control_point_transforms_;
  }

  std::vector<LieGroup>& parentFromsControlPoint() {
    return parent_from_control_point_transforms_;
  }

  [[nodiscard]] int getNumSegments() const {
    return int(parent_from_control_point_transforms_.size()) - 1;
  }

  [[nodiscard]] double deltaT() const { return delta_transform_; }

 private:
  std::vector<LieGroup> parent_from_control_point_transforms_;
  double delta_transform_;
};

template <concepts::LieGroup TGroup>
class CubicLieGroupBSpline {
 public:
  using LieGroup = TGroup;
  using Scalar = typename LieGroup::Scalar;
  using Transformation =
      Eigen::Matrix<Scalar, TGroup::kAmbientDim, TGroup::kAmbientDim>;
  using Tangent = typename LieGroup::Tangent;

  CubicLieGroupBSpline(
      std::vector<LieGroup> parent_ts_control_point, double t0, double delta_t)
      : impl_(std::move(parent_ts_control_point), delta_t), t0_(t0) {}

  [[nodiscard]] LieGroup parentFromSpline(double t) const {
    SegmentCoordinate index_and_u = this->indexAndU(t);

    return impl_.parentFromSpline(
        index_and_u.segment_idx, index_and_u.fraction);
  }

  [[nodiscard]] Transformation dtParentFromSpline(double t) const {
    SegmentCoordinate index_and_u = this->indexAndU(t);
    return impl_.dtParentFromSpline(
        index_and_u.segment_idx, index_and_u.fraction);
  }

  [[nodiscard]] Transformation dt2ParentFromSpline(double t) const {
    SegmentCoordinate index_and_u = this->indexAndU(t);
    return impl_.dt2ParentFromSpline(
        index_and_u.segment_idx, index_and_u.fraction);
  }

  [[nodiscard]] double t0() const { return t0_; }

  [[nodiscard]] double tmax() const {
    return t0_ + impl_.deltaT() * getNumSegments();
  }

  [[nodiscard]] std::vector<LieGroup> const& parentFromsControlPoint() const {
    return impl_.parentFromsControlPoint();
  }

  std::vector<LieGroup>& parentFromsControlPoint() {
    return impl_.parentFromsControlPoint();
  }

  [[nodiscard]] int getNumSegments() const { return impl_.getNumSegments(); }

  [[nodiscard]] double s(double t) const { return (t - t0_) / impl_.deltaT(); }

  [[nodiscard]] double deltaT() const { return impl_.deltaT(); }

  [[nodiscard]] SegmentCoordinate indexAndU(double t) const {
    SOPHUS_ASSERT(t >= t0_, "{} vs. {}", t, t0_);
    SOPHUS_ASSERT(t <= this->tmax(), "{} vs. {}", t, this->tmax());

    double s = this->s(t);
    double i = NAN;
    SegmentCoordinate index_and_u;
    index_and_u.fraction = std::modf(s, &i);
    index_and_u.segment_idx = int(i);
    if (index_and_u.fraction > sophus::kEpsilonF64) {
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
  CubicLieGroupBSplineImpl<LieGroup> impl_;

  double t0_;
};

template <concepts::LieGroup TGroup>
using BasisSpline = CubicLieGroupBSpline<TGroup>;

}  // namespace sophus
