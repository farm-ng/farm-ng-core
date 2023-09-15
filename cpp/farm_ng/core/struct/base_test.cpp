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

#include "farm_ng/core/struct/base.h"

#include <farm_pp/preprocessor/stringize.hpp>
#include <gtest/gtest.h>

TEST(struct_test, unit) {
  // clang-format off
  std::string expected_string = std::string(
      "static int constexpr kNumFields = 2; "
      "static std::array<std::string_view, kNumFields> constexpr kFieldNames = { \"i\" , \"d\" }; "
      "using FieldTypes = std::tuple<int , double >; "
      "int i {1}; "
      "double d {0.5};");
  // clang-format on
  EXPECT_EQ(
      FARM_PP_STRINGIZE(FARM_STRUCT_DETAILS_BASE(
          Foo, 2, ((int, i, {1}))((double, d, {0.5})))),
      expected_string);
}
