
// Copyright (c) farm-ng, inc. All rights reserved.

#pragma once

#include "farm_ng/core/logging/format.h"

#include <CLI/CLI.hpp>

namespace farm_ng_core {
template <class Enum>
CLI::CheckedTransformer makeCliMyEnumTransformer() {
  std::map<std::string, Enum> enum_from_string_map;
  for (size_t i = 0; i < getCount(Enum{}); ++i) {
    Enum val = Enum(getValues(Enum{})[i]);
    enum_from_string_map.insert({toString(val), val});
  }
  return CLI::CheckedTransformer(enum_from_string_map, CLI::ignore_case);
}

template <class Enum>
CLI::Option* cliAddEnumOption(
    CLI::App& app,
    const std::string& option_name,
    Enum& e,
    const std::string& description =
        FARM_NG_FORMAT("{}", getTypeName(Enum{}))) {
  auto range = getStringOfNames(Enum{});
  std::string check_description =
      FARM_NG_FORMAT("{}", std::string(range.begin(), range.end()));
  return app.add_option(option_name, e, description)
      ->transform(
          makeCliMyEnumTransformer<Enum>().description(check_description));
}

}  // namespace farm_ng_core
