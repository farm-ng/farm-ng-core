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
#include "sophus2/lie/lie_group.h"

namespace sophus2 {
namespace details {

template <class TScalar>
class CubicBSplineBasis {
 public:
  static Eigen::Matrix<TScalar, 3, 4> c() {
    Eigen::Matrix<TScalar, 3, 4> c;
    TScalar const o(0);

    // clang-format off
    c << TScalar(5./6),  TScalar(3./6), -TScalar(3./6),  TScalar(1./6),
         TScalar(1./6),  TScalar(3./6),  TScalar(3./6), -TScalar(2./6),
                    o,             o,                o,  TScalar(1./6);
    // clang-format on
    return c;
  }

  static Eigen::Vector3<TScalar> b(TScalar const& u) {
    // SOPHUS_ASSERT(u >= TScalar(0), "but %", u);
    // SOPHUS_ASSERT(u < TScalar(1), "but %", u);
    TScalar u_square(u * u);
    return c() * Eigen::Vector4<TScalar>(TScalar(1), u, u_square, u * u_square);
  }

  static Eigen::Vector3<TScalar> dtB(TScalar const& u, TScalar const& delta_t) {
    // SOPHUS_ASSERT(u >= TScalar(0), "but %", u);
    // SOPHUS_ASSERT(u < TScalar(1), "but %", u);
    return (TScalar(1) / delta_t) * c() *
           Eigen::Vector4<TScalar>(
               TScalar(0), TScalar(1), TScalar(2) * u, TScalar(3) * u * u);
  }

  static Eigen::Vector3<TScalar> dt2B(
      TScalar const& u, TScalar const& delta_t) {
    // SOPHUS_ASSERT(u >= TScalar(0), "but %", u);
    // SOPHUS_ASSERT(u < TScalar(1), "but %", u);
    return (TScalar(1) / (delta_t * delta_t)) * c() *
           Eigen::Vector4<TScalar>(
               TScalar(0), TScalar(0), TScalar(2), TScalar(6) * u);
  }
};

enum class SegmentCase { first, normal, last };

}  // namespace details
}  // namespace sophus2
