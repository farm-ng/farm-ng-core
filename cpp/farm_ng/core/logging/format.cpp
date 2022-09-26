// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <farm_ng/core/logging/format.h>

namespace farm_ng {

std::function<void(std::string const &)> &getLogLineFunction() {
  static std::function<void(std::string const &)> Static_Log_Function;
  return Static_Log_Function;
}

void setLogLineFunction(std::function<void(std::string const &)> f) {
  getLogLineFunction() = f;
}

void logLine(std::string const &line) {
  std::cerr << line << std::endl;
  if (getLogLineFunction()) {
    getLogLineFunction()(line);
  }
}

}  // namespace farm_ng
