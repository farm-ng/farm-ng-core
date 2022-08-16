// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <algorithm>

namespace farm_ng_core {

/// https://en.cppreference.com/w/cpp/container/deque/erase2
template <class Container, class Pred>
size_t erase_if(Container& c, Pred pred) {
  auto it = std::remove_if(c.begin(), c.end(), pred);
  auto r = std::distance(it, c.end());
  c.erase(it, c.end());
  return r;
}

}  // namespace farm_ng_core
