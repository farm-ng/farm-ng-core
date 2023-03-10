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

#include <farm_ng/core/logging/format.h>

namespace farm_ng {

auto getLogLineFunction() -> std::function<void(std::string const &)> & {
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
