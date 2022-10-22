//    Copyright 2022, farm-ng inc.
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

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
