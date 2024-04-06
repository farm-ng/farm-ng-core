// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sophus2/ceres/jet_helpers.h"
#include "sophus2/common/common.h"
#include "sophus2/geometry/point_transform.h"
#include "sophus2/linalg/batch.h"
#include "sophus2/sensor/camera_distortion/affine.h"

#include <Eigen/Dense>

namespace sophus2 {

// https://github.com/facebookincubator/isometric_pattern_matcher/blob/main/IsometricPatternMatcher/CameraModels.h
//
// parameters = fx, fy, cx, cy, kb0, kb1, kb2, kb3
class KannalaBrandtK3Transform {
 public:
  static int constexpr kNumDistortionParams = 4;
  static int constexpr kNumParams = kNumDistortionParams + 4;
  static std::string_view constexpr const kProjectionModel =
      "KannalaBrandtK3: fx, fy, cx, cy, kb0, kb1, kb2, kb3";

  template <class TScalar>
  using ProjInCameraZ1Plane = Eigen::Matrix<TScalar, 2, 1>;
  template <class TScalar>
  using PixelImage = Eigen::Matrix<TScalar, 2, 1>;
  template <class TScalar>
  using Params = Eigen::Matrix<TScalar, kNumParams, 1>;
  template <class TScalar>
  using DistorationParams = Eigen::Matrix<TScalar, kNumDistortionParams, 1>;

  template <class TParamsTypeT, class TPointTypeT>
  static auto distort(
      Eigen::MatrixBase<TParamsTypeT> const& params,
      Eigen::MatrixBase<TPointTypeT> const& proj_point_in_camera_z1_plane)
      -> PixelImage<typename TPointTypeT::Scalar> {
    static_assert(
        TParamsTypeT::ColsAtCompileTime == 1, "params must be a column-vector");
    static_assert(
        TParamsTypeT::RowsAtCompileTime == kNumParams,
        "params must have exactly kNumParams rows");
    static_assert(
        TPointTypeT::ColsAtCompileTime == 1,
        "point_camera must be a column-vector");
    static_assert(
        TPointTypeT::RowsAtCompileTime == 2,
        "point_camera must have exactly 2 columns");
    using Scalar = typename TPointTypeT::Scalar;

    Scalar const k0 = params[4];
    Scalar const k1 = params[5];
    Scalar const k2 = params[6];
    Scalar const k3 = params[7];

    Scalar const radius_squared =
        proj_point_in_camera_z1_plane[0] * proj_point_in_camera_z1_plane[0] +
        proj_point_in_camera_z1_plane[1] * proj_point_in_camera_z1_plane[1];
    using std::atan2;
    using std::sqrt;

    auto scaling_fn = [&]() -> Scalar {
      Scalar const radius = sqrt(radius_squared);
      Scalar const radius_inverse = Scalar(1.0) / radius;
      Scalar const theta = atan2(radius, Scalar(1.0));
      Scalar const theta2 = theta * theta;
      Scalar const theta4 = theta2 * theta2;
      Scalar const theta6 = theta4 * theta2;
      Scalar const theta8 = theta4 * theta4;
      Scalar const r_distorted =
          theta *
          (Scalar(1.0) + k0 * theta2 + k1 * theta4 + k2 * theta6 + k3 * theta8);
      return r_distorted * radius_inverse;
    };

    if constexpr (BatchTrait<Scalar>::kIsBatch) {
      Scalar const scaling =
          radius_squared.cwGreaterEqual(Scalar(sophus2::kEpsilonF64))
              .select(scaling_fn(), Scalar(1.0));

      return scaling * PixelImage<Scalar>(
                           proj_point_in_camera_z1_plane[0] * params[0],
                           proj_point_in_camera_z1_plane[1] * params[1]) +
             params.template segment<2>(2);
    } else {
      // linearize r around radius=0
      Scalar scaling = Scalar(1.0);
      if (radius_squared > sophus2::kEpsilonF64) {
        scaling = scaling_fn();
      }
      return Scalar{scaling} *
                 PixelImage<Scalar>(
                     proj_point_in_camera_z1_plane[0] * params[0],
                     proj_point_in_camera_z1_plane[1] * params[1]) +
             params.template segment<2>(2);
    }
  }

  template <class TScalar>
  static auto undistort(
      Params<TScalar> const& params, PixelImage<TScalar> const& pixel_image)
      -> ProjInCameraZ1Plane<TScalar> {
    using std::abs;
    using std::sqrt;
    using std::tan;

    // Undistortion
    TScalar const fu = params[0];
    TScalar const fv = params[1];
    TScalar const u0 = params[2];
    TScalar const v0 = params[3];

    TScalar const k0 = params[4];
    TScalar const k1 = params[5];
    TScalar const k2 = params[6];
    TScalar const k3 = params[7];

    TScalar const un = (pixel_image(0) - u0) / fu;
    TScalar const vn = (pixel_image(1) - v0) / fv;
    TScalar const rth2 = un * un + vn * vn;

    Eigen::Matrix<TScalar, 2, 1> orig(un, vn);
    if constexpr (BatchTrait<TScalar>::kIsBatch) {
      // for batch types check this in the very end only
    } else if (rth2 < sophus2::kEpsilon<TScalar> * sophus2::kEpsilon<TScalar>) {
      // early exit for non-batch type
      return orig;
    }

    TScalar const rth = sqrt(rth2);

    // Use Newtons method to solve for theta, 50 iterations max
    TScalar th = sqrt(rth);
    for (int i = 0; i < 50; ++i) {
      TScalar const th2 = th * th;
      TScalar const th4 = th2 * th2;
      TScalar const th6 = th4 * th2;
      TScalar const th8 = th4 * th4;

      TScalar const thd =
          th * (TScalar(1.0) + k0 * th2 + k1 * th4 + k2 * th6 + k3 * th8);

      TScalar const d_thd_wtr_th =
          TScalar(1.0) + TScalar(3.0) * k0 * th2 + TScalar(5.0) * k1 * th4 +
          TScalar(7.0) * k2 * th6 + TScalar(9.0) * k3 * th8;

      TScalar const step = (thd - rth) / d_thd_wtr_th;
      th = th - step;

      if (BatchTrait<TScalar>::allLessEqual(
              abs(jet_helpers::GetValue<TScalar>::impl(step)),
              sophus2::kEpsilon<TScalar>)) {
        // TODO: also exit early if rth2_close_to_zero is true
        break;
      }
    }

    TScalar radius_undistorted = tan(th);

    if constexpr (BatchTrait<TScalar>::kIsBatch) {
      Batch<bool, BatchTrait<TScalar>::kBatchSize> rth2_close_to_zero =
          rth2.cwLessEqual(
              sophus2::kEpsilon<TScalar> * sophus2::kEpsilon<TScalar>);

      TScalar sign = radius_undistorted.cwLessEqual(TScalar(0.0))
                         .select(TScalar(-1.0), TScalar(1.0));

      return rth2_close_to_zero.select(
          orig,
          Eigen::Matrix<TScalar, 2, 1>(
              sign * radius_undistorted * un / rth,
              sign * radius_undistorted * vn / rth));
    } else {
      TScalar sign = radius_undistorted < TScalar(0.0) ? -1.0 : 1.0;
      return Eigen::Matrix<TScalar, 2, 1>(
          sign * radius_undistorted * un / rth,
          sign * radius_undistorted * vn / rth);
    }
  }

  template <class TParamsTypeT, class TPointTypeT>
  static auto dxDistort(
      Eigen::MatrixBase<TParamsTypeT> const& params,
      Eigen::MatrixBase<TPointTypeT> const& proj_point_in_camera_z1_plane)
      -> Eigen::Matrix<typename TPointTypeT::Scalar, 2, 2> {
    static_assert(
        TParamsTypeT::ColsAtCompileTime == 1, "params must be a column-vector");
    static_assert(
        TParamsTypeT::RowsAtCompileTime == kNumParams,
        "params must have exactly kNumParams rows");
    static_assert(
        TPointTypeT::ColsAtCompileTime == 1,
        "point_camera must be a column-vector");
    static_assert(
        TPointTypeT::RowsAtCompileTime == 2,
        "point_camera must have exactly 2 columns");
    using Scalar = typename TPointTypeT::Scalar;

    Scalar a = proj_point_in_camera_z1_plane[0];
    Scalar b = proj_point_in_camera_z1_plane[1];
    Scalar const fx = params[0];
    Scalar const fy = params[1];
    Eigen::Matrix<Scalar, kNumDistortionParams, 1> k =
        params.template tail<kNumDistortionParams>();

    auto const radius_squared = a * a + b * b;
    using std::atan2;
    using std::sqrt;

    Eigen::Matrix<typename TPointTypeT::Scalar, 2, 2> dx0;
    dx0(0, 0) = fx;
    dx0(0, 1) = Scalar(0);
    dx0(1, 0) = Scalar(0);
    dx0(1, 1) = fy;

    if constexpr (BatchTrait<Scalar>::kIsBatch) {
      // for batch types check this in the very end only
    } else if (radius_squared < sophus2::kEpsilonSqrtF64) {
      // early exit for non-batch type
      return dx0;
    }

    using std::atan;
    using std::atan2;
    using std::pow;
    using std::sqrt;

    Scalar const c0 = a * a;
    Scalar const c1 = b * b;
    Scalar const c2 = c0 + c1;
    Scalar const c3 = pow(c2, 5.0 / 2.0);
    Scalar const c4 = c2 + Scalar(1);
    Scalar const c5 = atan(sqrt(c2));
    Scalar const c6 = c5 * c5;  // c5 ^ 2
    Scalar const c7 = c6 * k[0];
    Scalar const c8 = c6 * c6;  // c5 ^ 4
    Scalar const c9 = c8 * k[1];
    Scalar const c10 = c6 * c8;  // c5 ^ 6
    Scalar const c11 = c10 * k[2];
    Scalar const c12 = c8 * c8 * k[3];  // c5 ^ 6 * k[3]
    Scalar const c13 = 1.0 * c4 * c5 * (c11 + c12 + c7 + c9 + Scalar(1.0));
    Scalar const c14 = c13 * c3;
    Scalar const c15 = pow(c2, 3.0 / 2.0);
    Scalar const c16 = c13 * c15;
    Scalar const c17 = Scalar(1.0) * c11 + Scalar(1.0) * c12 +
                       Scalar(2.0) * c6 *
                           (Scalar(4.0) * c10 * k[3] + Scalar(2.0) * c6 * k[1] +
                            Scalar(3.0) * c8 * k[2] + k[0]) +
                       Scalar(1.0) * c7 + Scalar(1.0) * c9 + Scalar(1.0);
    Scalar const c18 = c17 * c2 * c2;
    Scalar const c19 = Scalar(1.0) / c4;
    Scalar const c20 = c19 / (c2 * c2 * c2);
    Scalar const c21 = a * b * c19 * (-c13 * c2 + c15 * c17) / c3;
    Eigen::Matrix<typename TPointTypeT::Scalar, 2, 2> dx;
    dx(0, 0) = c20 * fx * (-c0 * c16 + c0 * c18 + c14);
    dx(0, 1) = c21 * fx;

    dx(1, 0) = c21 * fy;
    dx(1, 1) = c20 * fy * (-c1 * c16 + c1 * c18 + c14);

    if constexpr (BatchTrait<Scalar>::kIsBatch) {
      return (radius_squared.cwLessEqual(sophus2::kEpsilon<Scalar>))
          .select(dx0, dx);
    }

    return dx;
  }
};

}  // namespace sophus2
