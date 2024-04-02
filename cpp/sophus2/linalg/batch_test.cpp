// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "sophus2/linalg/batch.h"

#include "sophus2/linalg/orthogonal.h"

#include <gtest/gtest.h>

using namespace sophus;

TEST(batch, unit) {
  int constexpr kNumBatches = 8;
  int constexpr kM = 7;
  int constexpr kN = 3;
  int constexpr kO = 5;

  std::vector<Eigen::Matrix<double, kM, kN>> mat7x3_vec;
  std::vector<Eigen::Matrix<double, kN, kO>> mat3x5_vec;
  std::vector<Eigen::Matrix<double, kM, kO>> mat7x5_vec;

  for (int b = 0; b < kNumBatches; ++b) {
    mat7x3_vec.push_back(Eigen::Matrix<double, kM, kN>::Random());
    mat3x5_vec.push_back(Eigen::Matrix<double, kN, kO>::Random());
  }

  Eigen::Matrix<Batch<double, kNumBatches>, kM, kN> batched_mat_7x3;
  for (int m = 0; m < kM; ++m) {
    for (int n = 0; n < kN; ++n) {
      Batch<double, kNumBatches>& batch = batched_mat_7x3(m, n);
      for (int b = 0; b < kNumBatches; ++b) {
        batch.b[b] = mat7x3_vec[b](m, n);
      }
    }
  }

  Eigen::Matrix<Batch<double, kNumBatches>, kN, kO> batched_mat_3x5;
  for (int n = 0; n < kN; ++n) {
    for (int o = 0; o < kO; ++o) {
      Batch<double, kNumBatches>& batch = batched_mat_3x5(n, o);
      for (int b = 0; b < kNumBatches; ++b) {
        batch.b[b] = mat3x5_vec[b](n, o);
      }
    }
  }

  for (int b = 0; b < kNumBatches; ++b) {
    mat7x5_vec.push_back(mat7x3_vec[b] * mat3x5_vec[b]);
  }
  Eigen::Matrix<Batch<double, kNumBatches>, kM, kO> batched_mat_7x5 =
      batched_mat_7x3 * batched_mat_3x5;

  for (int m = 0; m < kM; ++m) {
    for (int o = 0; o < kO; ++o) {
      Batch<double, kNumBatches>& batch = batched_mat_7x5(m, o);
      for (int b = 0; b < kNumBatches; ++b) {
        FARM_ASSERT_NEAR(batch.b[b], mat7x5_vec[b](m, o), 0.001);
      }
    }
  }
}
