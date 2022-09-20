// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "farm_ng/core/logging/logger.h"

#include <Eigen/Core>

namespace farm_ng {
namespace details {

template <class Scalar, int M, int N>
class MaxMetric<Eigen::Matrix<Scalar, M, N>> {
 public:
  static Scalar impl(
      Eigen::Matrix<Scalar, M, N> const& p0,
      Eigen::Matrix<Scalar, M, N> const& p1) {
    return (p0 - p1).template lpNorm<Eigen::Infinity>();
  }
};

}  // namespace details
}  // namespace farm_ng