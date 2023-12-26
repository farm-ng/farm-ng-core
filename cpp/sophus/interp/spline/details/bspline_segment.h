// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sophus/common/common.h"
#include "sophus/interp/spline/details/cubic_basis.h"

namespace sophus {
namespace details {

template <class TScalar, int kDim>
class CubicBSplineFn {
 public:
  using Scalar = TScalar;
  using Vector = Eigen::Vector<Scalar, kDim>;

  static Vector interpolate(
      Vector const& control_point,
      std::array<Vector, 3> const& control_vectors,
      double u) {
    Eigen::Vector3<TScalar> b = CubicBSplineBasis<TScalar>::b(u);
    return control_point + b[0] * control_vectors[0] +
           b[1] * control_vectors[1] + b[2] * control_vectors[2];
  }

  static Eigen::Matrix<Scalar, kDim, kDim> dxiInterpolate(
      double u, int quadruple_idx) {
    if (quadruple_idx == 0) {
      return Eigen::Matrix<Scalar, kDim, kDim>::Identity();
    }
    Eigen::Vector3<TScalar> b = CubicBSplineBasis<Scalar>::b(u);

    return b[quadruple_idx - 1] * Eigen::Matrix<Scalar, kDim, kDim>::Identity();
  }
};

template <class TScalar, int kDim>
struct CubicBSplineSegment {
 public:
  using Scalar = TScalar;
  using Vector = Eigen::Vector<Scalar, kDim>;

  CubicBSplineSegment(
      SegmentCase segment_case,
      Vector const& control_point0,
      Vector const& control_point1,
      Vector const& control_point2,
      Vector const& control_point3)
      : segment_case_(segment_case),
        control_point0_(control_point0),
        control_point1_(control_point1),
        control_point2_(control_point2),
        control_point3_(control_point3) {}

  CubicBSplineSegment(CubicBSplineSegment const&) = delete;
  CubicBSplineSegment(CubicBSplineSegment&&) = delete;

  auto operator=(CubicBSplineSegment const&) -> CubicBSplineSegment = delete;
  auto operator=(CubicBSplineSegment&&) -> CubicBSplineSegment = delete;

  Vector interpolate(double u) {
    switch (segment_case_) {
      case SegmentCase::first: {
        return CubicBSplineFn<Scalar, kDim>::interpolate(
            control_point0_,
            std::array<Vector, 3>(
                {Vector::Zero(),
                 control_point2_ - control_point1_,
                 control_point3_ - control_point2_}),
            u);
      }
      case SegmentCase::normal: {
        return CubicBSplineFn<Scalar, kDim>::interpolate(
            control_point0_,
            std::array<Vector, 3>(
                {control_point1_ - control_point0_,
                 control_point2_ - control_point1_,
                 control_point3_ - control_point2_}),
            u);
      }
      case SegmentCase::last: {
        return CubicBSplineFn<Scalar, kDim>::interpolate(
            control_point0_,
            std::array<Vector, 3>(
                {control_point1_ - control_point0_,
                 control_point2_ - control_point1_,
                 Vector::Zero()}),
            u);
      }
    }

    SOPHUS_PANIC("logic error");
  }

  Eigen::Matrix<Scalar, kDim, kDim> dxiInterpolate(
      double u, int quadruple_idx) {
    switch (segment_case_) {
      case SegmentCase::first: {
        if (quadruple_idx == 0) {
          return Eigen::Matrix<Scalar, kDim, kDim>::Zero();
        }
        if (quadruple_idx == 1) {
          return CubicBSplineFn<Scalar, kDim>::dxiInterpolate(u, 0) -
                 CubicBSplineFn<Scalar, kDim>::dxiInterpolate(u, 2);
        }
        if (quadruple_idx == 2) {
          return CubicBSplineFn<Scalar, kDim>::dxiInterpolate(u, 2) -
                 CubicBSplineFn<Scalar, kDim>::dxiInterpolate(u, 3);
        }
        return CubicBSplineFn<Scalar, kDim>::dxiInterpolate(u, 3);
      }
      case SegmentCase::normal: {
        if (quadruple_idx == 0) {
          return CubicBSplineFn<Scalar, kDim>::dxiInterpolate(u, 0) -
                 CubicBSplineFn<Scalar, kDim>::dxiInterpolate(u, 1);
        }
        if (quadruple_idx == 1) {
          return CubicBSplineFn<Scalar, kDim>::dxiInterpolate(u, 1) -
                 CubicBSplineFn<Scalar, kDim>::dxiInterpolate(u, 2);
        }
        if (quadruple_idx == 2) {
          return CubicBSplineFn<Scalar, kDim>::dxiInterpolate(u, 2) -
                 CubicBSplineFn<Scalar, kDim>::dxiInterpolate(u, 3);
        }
        return CubicBSplineFn<Scalar, kDim>::dxiInterpolate(u, 3);
      }
      case SegmentCase::last: {
        if (quadruple_idx == 0) {
          return CubicBSplineFn<Scalar, kDim>::dxiInterpolate(u, 0) -
                 CubicBSplineFn<Scalar, kDim>::dxiInterpolate(u, 1);
        }
        if (quadruple_idx == 1) {
          return CubicBSplineFn<Scalar, kDim>::dxiInterpolate(u, 1) -
                 CubicBSplineFn<Scalar, kDim>::dxiInterpolate(u, 2);
        }
        if (quadruple_idx == 2) {
          return CubicBSplineFn<Scalar, kDim>::dxiInterpolate(u, 2);
        }
        return Eigen::Matrix<Scalar, kDim, kDim>::Zero();
      }
    }

    SOPHUS_PANIC("logic error");
  }

 private:
  SegmentCase segment_case_;
  Vector const& control_point0_;
  Vector const& control_point1_;
  Vector const& control_point2_;
  Vector const& control_point3_;
};

}  // namespace details
}  // namespace sophus
