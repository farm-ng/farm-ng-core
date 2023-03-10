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

#include "farm_ng/core/logging/expected.h"

namespace farm_ng {

auto operator<<(std::ostream& os, Error const& error) -> std::ostream& {
  os << error.details.size() << "error details:\n";
  for (auto const& error : error.details) {
    os << FARM_FORMAT("[{}:{}] {}\n", error.file, error.line, error.msg);
  }
  return os;
}
}  // namespace farm_ng
