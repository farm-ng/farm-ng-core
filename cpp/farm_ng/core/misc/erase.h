// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <algorithm>

namespace farm_ng {

/// https://en.cppreference.com/w/cpp/container/deque/erase2
template <class ContainerT, class PredT>
size_t eraseIf(ContainerT& c, PredT pred) {
  auto it = std::remove_if(c.begin(), c.end(), pred);
  auto r = std::distance(it, c.end());
  c.erase(it, c.end());
  return r;
}

}  // namespace farm_ng