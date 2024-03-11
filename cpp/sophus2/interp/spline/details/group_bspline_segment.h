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

#include "sophus2/common/common.h"
#include "sophus2/interp/spline/details/cubic_basis.h"
#include "sophus2/lie/lie_group.h"

namespace sophus2 {
namespace details {

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
    Eigen::Vector3d b = CubicBSplineBasis<double>::b(u);
    return std::make_tuple(
        LieGroup::exp(b[0] * std::get<0>(control_tangent_vectors)),
        LieGroup::exp(b[1] * std::get<1>(control_tangent_vectors)),
        LieGroup::exp(b[2] * std::get<2>(control_tangent_vectors)));
  }

  static Transformation dx1A(
      Tangent const& control_tangent_vectors, double u, int i) {
    Eigen::Vector3d b = CubicBSplineBasis<double>::b(u);
    Eigen::Vector<Scalar, kDof> t;
    t[i] = Scalar(1.0);
    return LieGroup::hat(t) * LieGroup::exp(b[0] * t).matrix();
  }

  static std::tuple<Transformation, Transformation, Transformation> dtA(
      std::tuple<LieGroup, LieGroup, LieGroup> const& aa,
      std::tuple<Tangent, Tangent, Tangent> const& control_tangent_vectors,
      double u,
      double delta_t) {
    Eigen::Vector3d dt_b = CubicBSplineBasis<double>::dtB(u, delta_t);
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
    Eigen::Vector3d dt_b = CubicBSplineBasis<double>::dtB(u, delta_t);
    Eigen::Vector3d dt2_b = CubicBSplineBasis<double>::dt2B(u, delta_t);

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
struct CubicLieGroupBSplineSegment {
 public:
  using Scalar = typename TGroup::Scalar;
  using T = Scalar;
  using Transformation =
      Eigen::Matrix<Scalar, TGroup::kAmbientDim, TGroup::kAmbientDim>;
  using Params = typename TGroup::Params;

  CubicLieGroupBSplineSegment(
      SegmentCase segment_case,
      Params const& params0,
      Params const& params1,
      Params const& params2,
      Params const& params3)
      : segment_case_(segment_case),
        params0_(params0),
        params1_(params1),
        params2_(params2),
        params3_(params3) {}

  CubicLieGroupBSplineSegment(CubicLieGroupBSplineSegment const&) = delete;
  CubicLieGroupBSplineSegment(CubicLieGroupBSplineSegment&&) = delete;

  auto operator=(CubicLieGroupBSplineSegment const&)
      -> CubicLieGroupBSplineSegment = delete;
  auto operator=(CubicLieGroupBSplineSegment&&)
      -> CubicLieGroupBSplineSegment = delete;

  [[nodiscard]] TGroup worldFromFooPrev() const {
    return TGroup::fromParams(params0_);
  }
  [[nodiscard]] TGroup worldFromFoo0() const {
    return TGroup::fromParams(params1_);
  }

  [[nodiscard]] TGroup worldFromFoo1() const {
    return TGroup::fromParams(params2_);
  }

  [[nodiscard]] TGroup worldFromFoo2() const {
    return TGroup::fromParams(params3_);
  }
  TGroup parentFromSpline(double u) {
    switch (segment_case_) {
      case SegmentCase::first:
        return CubicLieGroupBSplineFn<TGroup>::parentFromSpline(
            worldFromFoo0(),
            std::make_tuple(
                (worldFromFoo0().inverse() * worldFromFoo0()).log(),
                (worldFromFoo0().inverse() * worldFromFoo1()).log(),
                (worldFromFoo1().inverse() * worldFromFoo2()).log()),
            u);
      case SegmentCase::normal:
        return CubicLieGroupBSplineFn<TGroup>::parentFromSpline(
            worldFromFooPrev(),
            std::make_tuple(
                (worldFromFooPrev().inverse() * worldFromFoo0()).log(),
                (worldFromFoo0().inverse() * worldFromFoo1()).log(),
                (worldFromFoo1().inverse() * worldFromFoo2()).log()),
            u);
      case SegmentCase::last:
        return CubicLieGroupBSplineFn<TGroup>::parentFromSpline(
            worldFromFooPrev(),
            std::make_tuple(
                (worldFromFooPrev().inverse() * worldFromFoo0()).log(),
                (worldFromFoo0().inverse() * worldFromFoo1()).log(),
                (worldFromFoo1().inverse() * worldFromFoo1()).log()),
            u);
    }
    SOPHUS_PANIC("logic error");
  }

  Transformation dtParentFromSpline(double u, double delta_t) {
    switch (segment_case_) {
      case SegmentCase::first:
        return CubicLieGroupBSplineFn<TGroup>::dtParentFromSpline(
            worldFromFoo0(),
            std::make_tuple(
                (worldFromFoo0().inverse() * worldFromFoo0()).log(),
                (worldFromFoo0().inverse() * worldFromFoo1()).log(),
                (worldFromFoo1().inverse() * worldFromFoo2()).log()),
            u,
            delta_t);
      case SegmentCase::normal:
        return CubicLieGroupBSplineFn<TGroup>::dtParentFromSpline(
            worldFromFooPrev(),
            std::make_tuple(
                (worldFromFooPrev().inverse() * worldFromFoo0()).log(),
                (worldFromFoo0().inverse() * worldFromFoo1()).log(),
                (worldFromFoo1().inverse() * worldFromFoo2()).log()),
            u,
            delta_t);
      case SegmentCase::last:
        return CubicLieGroupBSplineFn<TGroup>::dtParentFromSpline(
            worldFromFooPrev(),
            std::make_tuple(
                (worldFromFooPrev().inverse() * worldFromFoo0()).log(),
                (worldFromFoo0().inverse() * worldFromFoo1()).log(),
                (worldFromFoo1().inverse() * worldFromFoo1()).log()),
            u,
            delta_t);
    }
    SOPHUS_PANIC("logic error");
  }

  Transformation dt2ParentFromSpline(double u, double delta_t) {
    switch (segment_case_) {
      case SegmentCase::first:
        return CubicLieGroupBSplineFn<TGroup>::dt2ParentFromSpline(
            worldFromFoo0(),
            std::make_tuple(
                (worldFromFoo0().inverse() * worldFromFoo0()).log(),
                (worldFromFoo0().inverse() * worldFromFoo1()).log(),
                (worldFromFoo1().inverse() * worldFromFoo2()).log()),
            u,
            delta_t);
      case SegmentCase::normal:
        return CubicLieGroupBSplineFn<TGroup>::dt2ParentFromSpline(
            worldFromFooPrev(),
            std::make_tuple(
                (worldFromFooPrev().inverse() * worldFromFoo0()).log(),
                (worldFromFoo0().inverse() * worldFromFoo1()).log(),
                (worldFromFoo1().inverse() * worldFromFoo2()).log()),
            u,
            delta_t);
      case SegmentCase::last:
        return CubicLieGroupBSplineFn<TGroup>::dt2ParentFromSpline(
            worldFromFooPrev(),
            std::make_tuple(
                (worldFromFooPrev().inverse() * worldFromFoo0()).log(),
                (worldFromFoo0().inverse() * worldFromFoo1()).log(),
                (worldFromFoo1().inverse() * worldFromFoo1()).log()),
            u,
            delta_t);
    }
    SOPHUS_PANIC("logic error");
  }

 private:
  SegmentCase segment_case_;
  Params params0_;
  Params params1_;
  Params params2_;
  Params params3_;
};

}  // namespace details
}  // namespace sophus2
