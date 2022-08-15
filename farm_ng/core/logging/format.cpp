// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng/core/logging/format.h"

#include <iostream>

namespace farm_ng_core {

std::function<void(const std::string&)>& getLogLineFunction() {
  static std::function<void(const std::string&)> Log_Function;
  return Log_Function;
}

void setLogLineFunction(std::function<void(const std::string&)> f) {
  getLogLineFunction() = f;
}

void logLine(const std::string& line) {
  std::cerr << line << std::endl;
  if (getLogLineFunction()) {
    getLogLineFunction()(line);
  }
}

}  // namespace farm_ng_core
