// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.
#include "farm_ng/core/logging/expected.h"

namespace farm_ng {

std::ostream& operator<<(std::ostream& os, Error const& error) {
  os << error.details.size() << "error details:\n";
  for (auto const& error : error.details) {
    os << FARM_FORMAT("[{}:{}] {}\n", error.file, error.line, error.msg);
  }
  return os;
}
}  // namespace farm_ng