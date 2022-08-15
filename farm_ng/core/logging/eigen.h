// Copyright (c) farm-ng, inc. All rights reserved.

#pragma once

#include "farm_ng/core/logging/logger.h"

#include <Eigen/Core>

namespace farm_ng_core {
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
}  // namespace farm_ng_core
