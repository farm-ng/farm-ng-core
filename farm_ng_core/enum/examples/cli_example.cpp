// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CLI/CLI.hpp>

#include "../EnumO.h"

FNG_ENUM(MyEnum, int8_t, (foo, bar, daz));

int main(int argc, char **argv) {
  MyEnum e;
  std::string input_str;

  CLI::App app{"Enum Example"};

  auto view = getStringOfNames(MyEnum());
  app.add_option(
         "--input",
         input_str,
         std::string("valid inputs are: ") +
             std::string(view.begin(), view.end()))
      ->required();
  CLI11_PARSE(app, argc, argv);

  if (!trySetFromString(e, input_str)) {
    std::cout << "input has invalid value: " << input_str << std::endl;
    return 0;
  }

  if (e == MyEnum::daz) {
    e = MyEnum::foo;
  }
  std::cout << "Debug Log: Enum has now value: " << e << std::endl;
  // This outputs: "foo (=0)", or "bar (=1)".

  std::cout << e << std::endl;
}
