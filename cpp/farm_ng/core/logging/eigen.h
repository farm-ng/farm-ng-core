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

// TODO: Move to Sophus.
namespace farm_ng {
namespace details {

template <class Scalar, int M, int N>
struct CheckNear<Eigen::Matrix<Scalar, M, N>> {
  static void impl(
      Eigen::Matrix<Scalar, M, N> const& lhs,
      Eigen::Matrix<Scalar, M, N> const& rhs,
      Scalar const& thr,
      char const* lhs_cstr,
      char const* rhs_cstr,
      char const* thr_cstr,
      std::string const& file,
      int line,
      std::string const& func,
      std::string const& str) {
    using std::abs;
    using std::min;

    Scalar max_error = 0.0;
    int max_error_dim = -1;
    bool absolute = true;
    for (int i = 0; i < lhs.reshaped().rows(); ++i) {
      auto lhs_abs = abs(lhs.reshaped()[i]);
      auto rhs_abs = abs(rhs.reshaped()[i]);

      if (min(lhs_abs, rhs_abs) < thr) {
        /* close to zero, we are doing an absolute comparison*/
        Scalar err = abs(lhs.reshaped()[i] - rhs.reshaped()[i]);
        if (err > max_error) {
          max_error = err;
          max_error_dim = i;
          absolute = true;
        }
      } else {
        Scalar err = abs((lhs.reshaped()[i] / rhs.reshaped()[i]) - 1.0);
        if (err > max_error) {
          max_error = err;
          max_error_dim = i;
          absolute = false;
        }
      }
    }
    if (max_error < thr) {
      // all errors below threshold
      return;
    }
    farm_ng::defaultLogger().log(
        farm_ng::LogLevel::critical,
        FARM_FORMAT(
            "ASSERT_NEAR({})]"
            "Not true: {} near {}; has error of {} in {} (thr: "
            "{})\n{}\n{}\n{}\nvs.\n{}\n{}\n{}",
            absolute ? "absolute" : "relative",
            lhs_cstr,
            rhs_cstr,
            max_error,
            max_error_dim,
            thr,
            lhs.reshaped()[max_error_dim],
            lhs_cstr,
            lhs,
            rhs.reshaped()[max_error_dim],
            rhs_cstr,
            rhs),
        file,
        line,
        func,
        str);
    FARM_IMPL_ABORT();
  }
};

}  // namespace details
}  // namespace farm_ng
