// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <filesystem>
#include <sstream>
#include <string>
#include <vector>

namespace farm_ng_core {

/// Splits string into N tokens (substrings) based on a delimiter
inline std::vector<std::string> tokenize(
    const std::string& str, char delimiter = ' ') {
  std::vector<std::string> tokens;

  std::stringstream strstream(str);
  std::string token;

  while (getline(strstream, token, delimiter)) {
    tokens.push_back(token);
  }

  return tokens;
}

}  // namespace farm_ng_core
