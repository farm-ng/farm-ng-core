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

#pragma once

#include <filesystem>
#include <sstream>
#include <string>
#include <vector>

namespace farm_ng {

/// Splits string into N tokens (substrings) based on a delimiter
inline std::vector<std::string> tokenize(
    std::string const& str, char delimiter = ' ') {
  std::vector<std::string> tokens;

  std::stringstream strstream(str);
  std::string token;

  while (getline(strstream, token, delimiter)) {
    tokens.push_back(token);
  }

  return tokens;
}

}  // namespace farm_ng
