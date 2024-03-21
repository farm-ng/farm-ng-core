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

#include "farm_ng/core/struct/struct.h"

#include "farm_ng/core/examples.pb.h"

#include <farm_ng/core/logging/logger.h>
#include <gtest/gtest.h>

namespace farm_ng {
FARM_STRUCT(StructExample1, ((int, integer, {42}), (float, f, {0.5})));
}

TEST(struct_test, unit) {
  using namespace farm_ng;

  // clang-format off
  std::string expected_string = std::string(
      "struct Foo { "
        "static int constexpr kNumFields = 2; "
        "static std::array<std::string_view, kNumFields> constexpr kFieldNames "
          "= { \"i\" , \"d\" }; "
        "using FieldTypes = std::tuple<int , double >; "
        "static Foo fromTuple(FieldTypes&& tuple) noexcept { "
          "Foo s; "
          "s.i = std::move(std::get<0>(tuple)); "
          "s.d = std::move(std::get<1>(tuple)); "
            "return s; "
        "} "
        "auto toTuple() const noexcept -> FieldTypes { "
            "return std::make_tuple(this->i , this->d ); "
        "} "
        "int i {1}; "
        "double d {0.5}; "
      "}");
  // clang-format on
  EXPECT_EQ(
      FARM_PP_STRINGIZE(FARM_STRUCT(Foo, ((int, i, {1}), (double, d, {0.5})))),
      expected_string);

  StructExample1 instance;
  std::tuple<int, float> tuple = instance.toTuple();
  FARM_ASSERT_EQ(std::get<0>(tuple), instance.integer);
  FARM_ASSERT_EQ(std::get<1>(tuple), instance.f);
  tuple = std::make_tuple(-1, 0.25);
  StructExample1 instance2 = StructExample1::fromTuple(std::move(tuple));

  FARM_ASSERT_EQ(std::get<0>(tuple), instance2.integer);
  FARM_ASSERT_EQ(std::get<1>(tuple), instance2.f);
}
