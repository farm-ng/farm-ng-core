// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng_core/utils/tokenize.h"

#include <filesystem>
#include <iostream>
#include <sstream>

namespace farm_ng_core {

std::vector<std::string> tokenize(const std::string& str, char delimiter) {
  std::vector<std::string> tokens;

  std::stringstream strstream(str);
  std::string token;

  while (getline(strstream, token, delimiter)) {
    tokens.push_back(token);
  }

  return tokens;
}

}  // namespace farm_ng_core
