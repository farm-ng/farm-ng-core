// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sophus2/common/common.h"

namespace sophus2 {
template <class TScalar>
struct BatchTrait {
  using Scalar = TScalar;
  using ScalarBatch = TScalar;
  static int constexpr kBatchSize = 1;
  static bool constexpr kIsBatch = false;

  static bool anyLessEqual(TScalar const& lhs, TScalar const& rhs) {
    return lhs < rhs;
  }

  static bool allLessEqual(TScalar const& lhs, TScalar const& rhs) {
    return lhs < rhs;
  }
};
}  // namespace sophus2
