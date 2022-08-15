// Copyright (c) farm-ng, inc. All rights reserved.

#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace farm_ng_core {

/// Splits string into N tokens (substrings) based on a delimiter
std::vector<std::string> tokenize(const std::string& str, char delimiter = ' ');

}  // namespace farm_ng_core
