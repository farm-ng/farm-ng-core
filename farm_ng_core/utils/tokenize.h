// Copyright (c) farm-ng, inc. All rights reserved.

#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <sstream>

namespace farm_ng_core {

/// Splits string into N tokens (substrings) based on a delimiter
inline std::vector<std::string> tokenize(const std::string& str, char delimiter = ' '){
  std::vector<std::string> tokens;

  std::stringstream strstream(str);
  std::string token;

  while (getline(strstream, token, delimiter)) {
    tokens.push_back(token);
  }

  return tokens;
}

}  // namespace farm_ng_core
