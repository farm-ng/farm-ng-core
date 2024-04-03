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

#include "farm_ng/core/logging/expected.h"
#include "sophus2/concepts/batch.h"

#include <Eigen/Core>



namespace farm_ng {

/// Checks whether two floating point numbers x, y are close to each other.
///
/// It outputs:  min(|x-y|, |x-y| / max(|x|, |y|))
///
/// The output is between 0.0 and 2.0:
///
///  * x and y are close             => ~0.0
///  * x and y are NOT close and
///    - |x| and |y| are NOT close   => ~1.0
///    - |x| and |y| are close       => ~2.0
inline double relativeCloseness(double x, double y) {
  // https://c-faq.com/fp/fpequal.html
  double abs_diff = std::abs(x - y);
  double rel_diff = abs_diff / std::max(std::abs(x), std::abs(y));

  // We are using "rel_diff" from the c-faq reference. However, instead of just
  // checking for x==0 and y==0, we perform the minimum over `rel_diff` and
  // `abs_diff`. This is to avoid surprises when there are two small numbers
  // with opposite sign.
  //  E.g. for x=0.001 and y=-0.001 we have abs_diff = 0.002, but rel_diff
  //  = 2.0, hence relativeCloseness(0.001, -0.001) = 0.002.
  return std::min(abs_diff, rel_diff);
}

namespace details {
template <class TPoint, class TPoint2, class TEnabler = void>
struct CheckNear;

template <class TPoint, class TPoint2>
struct CheckNear<
    TPoint,
    TPoint2,
    std::enable_if_t<
        std::is_arithmetic_v<TPoint> && std::is_arithmetic_v<TPoint2>,
        void>> {
  static Expected<Success> check(
      bool check_relative,
      double const& lhs,
      char const* lhs_cstr,
      double const& rhs,
      char const* rhs_cstr,
      double const& thr) {
    double err =
        check_relative ? relativeCloseness(rhs, lhs) : std::abs(rhs - lhs);
    if (!(err <= thr)) {
      // inverted comparison,  so we also end up here if err or max_error
      return FARM_UNEXPECTED(
          "\nNot true: {} near {}; has error of {} (thr: {})\n"
          "LHS: `{}` is:\n"
          "{}\n\n"
          "RHS `{}` is:\n"
          "{}\n\n",
          lhs_cstr,
          rhs_cstr,
          err,
          thr,
          lhs_cstr,
          lhs,
          rhs_cstr,
          rhs);
    }
    return Success{};
  }
};

template <class TScalar, int kRows, int kCols>
struct CheckNear<
    Eigen::Matrix<TScalar, kRows, kCols>,
    Eigen::Matrix<TScalar, kRows, kCols>,
    std::enable_if_t<std::is_arithmetic_v<TScalar>, void>> {
  static Expected<Success> check(
      bool check_relative,
      Eigen::Matrix<TScalar, kRows, kCols> const& lhs,
      char const* lhs_cstr,
      Eigen::Matrix<TScalar, kRows, kCols> const& rhs,
      char const* rhs_cstr,
      double const& thr) {
    double max_error = 0.0;
    int max_row_idx = -1;
    int max_col_idx = -1;

    auto nearness_fn =
        check_relative
            ? [](double x, double y) { return relativeCloseness(x, y); }
            : [](double x, double y) { return std::abs(x - y); };

    for (int c = 0; c < lhs.cols(); ++c) {
      for (int r = 0; r < lhs.rows(); ++r) {
        double err = nearness_fn(lhs(r, c), rhs(r, c));

        if (!(err <= max_error)) {
          // inverted comparison,  so we also end up here if err or max_error
          // are NAN
          max_error = err;
          max_row_idx = r;
          max_col_idx = c;
        }
      }
    }
    if (max_error < thr) {
      // all errors below threshold
      return Success{};
    }
    return FARM_UNEXPECTED(
        "Not true: {} near {} [r:{},c:{}]; has error of {} (thr: {})\n"
        "LHS `{}` is:\n"
        "{}\n\n"
        "RHS `{}` is:\n"
        "{}\n\n",
        lhs_cstr,
        rhs_cstr,
        max_row_idx,
        max_col_idx,
        max_error,
        thr,
        lhs_cstr,
        lhs,
        rhs_cstr,
        rhs);
  }
};

template <class TBatch, int kRows, int kCols>
struct CheckNear<
    Eigen::Matrix<TBatch, kRows, kCols>,
    Eigen::Matrix<TBatch, kRows, kCols>,
    std::enable_if_t<::sophus2::BatchTrait<TBatch>::kIsBatch>> {
  static Expected<Success> check(
      bool check_relative,
      Eigen::Matrix<TBatch, kRows, kCols> const& lhs,
      char const* lhs_cstr,
      Eigen::Matrix<TBatch, kRows, kCols> const& rhs,
      char const* rhs_cstr,
      double const& thr) {
    double max_error = 0.0;
    int max_row_idx = -1;
    int max_col_idx = -1;
    int max_batch_idx = -1;

    auto nearness_fn =
        check_relative
            ? [](double x, double y) { return relativeCloseness(x, y); }
            : [](double x, double y) { return std::abs(x - y); };

    for (int c = 0; c < lhs.cols(); ++c) {
      for (int r = 0; r < lhs.rows(); ++r) {
        for (int batch = 0; batch < sophus2::BatchTrait<TBatch>::kBatchSize;
             ++batch) {
          double err =
              nearness_fn(lhs(r, c).lanes[batch], rhs(r, c).lanes[batch]);

          if (!(err <= max_error)) {
            // inverted comparison,  so we also end up here if err or max_error
            // are NAN
            max_error = err;
            max_row_idx = r;
            max_col_idx = c;
            max_batch_idx = batch;
          }
        }
      }
    }
    if (max_error < thr) {
      // all errors below threshold
      return Success{};
    }
    return FARM_UNEXPECTED(
        "Not true: {} near {} [r:{}c:{}b:{}]; has error of {} (thr: {})\n"
        "LHS `{}` is:\n"
        "{}\n\n"
        "RHS `{}` is:\n"
        "{}\n\n",
        lhs_cstr,
        rhs_cstr,
        max_row_idx,
        max_col_idx,
        max_batch_idx,
        max_error,
        thr,
        lhs_cstr,
        lhs,
        rhs_cstr,
        rhs);

    return Success{};
  }
};

template <class TScalar, class TScalar2>
inline Expected<Success> checkAbsNear(
    TScalar lhs,
    char const* lhs_cstr,
    TScalar2 rhs,
    char const* rhs_cstr,
    double thr) {
  return CheckNear<TScalar, TScalar2>::check(
      false, lhs, lhs_cstr, rhs, rhs_cstr, thr);
}

template <class TScalar, class TScalar2>
inline Expected<Success> checkRelativeNear(
    TScalar lhs,
    char const* lhs_cstr,
    TScalar2 rhs,
    char const* rhs_cstr,
    double thr) {
  if (!((thr >= 0.0) && (thr <= 1.0))) {
    FARM_WARN(
        "The threshold of the WITHIN_REL macro shall be in [0.0, 1.0], but "
        "we got `{}`.",
        thr);
  }
  return CheckNear<TScalar, TScalar2>::check(
      true, lhs, lhs_cstr, rhs, rhs_cstr, thr);
}
}  // namespace details
}  // namespace farm_ng

/// If it is false that `lhs` is near `rhs` according to threshold `thr`, print
/// formatted error message and then panic.
///
/// `lhs` and `rhs` are near, if relativeCloseness(lhs, rhs) < thr.
///
/// The threshold `thr` shall be in [0.0, 1.0].
///
/// A threshold ~0.0 implies that lhs is near rhs, and ~1.0 implies the lhs is
/// NOT near rhs.
#define FARM_ASSERT_WITHIN_REL(lhs, rhs, thr, ...)                        \
  do {                                                                    \
    Expected<Success> status =                                            \
        ::farm_ng::details::checkRelativeNear(lhs, #lhs, rhs, #rhs, thr); \
    if (!status) {                                                        \
      FARM_PANIC(                                                         \
          "FARM_ASSERT_WITHIN_REL failed: {}\n"                           \
          "Not true: `{}` near {}; (thr: {})\n"                           \
          "{}",                                                           \
          #lhs,                                                           \
          #rhs,                                                           \
          #thr,                                                           \
          status.error(),                                                 \
          FARM_FORMAT(__VA_ARGS__));                                      \
    }                                                                     \
  } while (false)

/// If it is false that `lhs` is near `rhs` according to threshold
/// `thr`, print formatted error message and then panic.
///
/// `lhs` and `rhs` are near, if std::abs(lhs - rhs) < thr.
#define FARM_ASSERT_WITHIN_ABS(lhs, rhs, thr, ...)                   \
  do {                                                               \
    Expected<Success> status =                                       \
        ::farm_ng::details::checkAbsNear(lhs, #lhs, rhs, #rhs, thr); \
    if (!status) {                                                   \
      FARM_PANIC(                                                    \
          "FARM_ASSERT_WITHIN_ABS failed: {}\n"                      \
          "Not true: `{}` near {}; (thr: {})\n"                      \
          "{}",                                                      \
          #lhs,                                                      \
          #rhs,                                                      \
          #thr,                                                      \
          status.error(),                                            \
          FARM_FORMAT(__VA_ARGS__));                                 \
    }                                                                \
  } while (false)
