// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng_core/logging/expected.h"

#include <iostream>

namespace farm_ng_core {

std::ostream& operator<<(std::ostream& os, const Error& error) {
  os << error.details.size() << "error details:\n";
  for (const auto& error : error.details) {
    os << FNG_FORMAT("[{}:{}] {}\n", error.file, error.line, error.msg);
  }
  return os;
}

}  // namespace farm_ng_core
