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

#define FARM_ERROR_CALLSTACK_DISABLED

#include "farm_ng/core/proto_conv/struct/conv.h"

#include "farm_ng/core/examples.pb.h"
#include "farm_ng/core/struct/struct.h"

#include <farm_pp/preprocessor/stringize.hpp>
#include <gtest/gtest.h>

namespace farm_ng {
FARM_STRUCT(StructExample1, ((int, integer, {1}), (float, f, {0.5})));
FARM_STRUCT(
    StructExample2, ((std::string, name, {}), (StructExample1, ex1, {})));

// this typically goes in proto_conv.h file
FARM_PROTO_CONV_TRAIT(StructExample1, core::proto::StructExample1);

// this typically goes in proto_conv.cc file
FARM_PROTO_CONV_IMPL(StructExample1, core::proto::StructExample1, (integer, f));

// this typically goes in proto_conv.h file
FARM_PROTO_CONV_TRAIT(StructExample2, core::proto::StructExample2);

// this typically goes in proto_conv.cc file
FARM_PROTO_CONV_IMPL_FN(
    StructExample2,
    core::proto::StructExample2,
    (name, ex1),
    [](StructExample2&& s, core::proto::StructExample2 const& proto)
        -> Expected<StructExample2> { return std::move(s); },
    [](core::proto::StructExample2&& proto, StructExample2 const& s) {
      return std::move(proto);
    });

}  // namespace farm_ng

TEST(FARM_STRUCT_DETAILS_FROM_PROTO, macro) {
  // clang-format off
  std::string expected_string =
        "if constexpr (::farm_ng::proto_conv_details::IsPrimitive< std::tuple_element_t<0, farm_ng::StructExample1::FieldTypes>>::value) { "
            "s.integer = proto.integer(); "
        "} else { "
            "auto mayberes = (::farm_ng::fromProt(proto.integer())); "
            "if (!mayberes) { "
                "auto error = mayberes.error(); "
                "error.details.emplace_back( "
                    "::farm_ng::ErrorDetail { "
                        ".file = \"n/a\", ."
                        "line = 0, "
                        ".msg = std::string(\"FARM_TRY propagated error.\\n\") }); "
                "return ::tl::make_unexpected(error); "
            "} auto res = ::std::move(*mayberes);; "
            "s.integer = res; "
        "}";
  // clang-format on
  EXPECT_EQ(
      FARM_PP_STRINGIZE(FARM_STRUCT_DETAILS_FROM_PROTO(
          dummy, farm_ng::StructExample1::FieldTypes, 0, (integer))),
      expected_string);

  // clang-format off
  expected_string =
        "static_assert( \"SKIP\" == std::string_view(\"SKIP\"));";
  // clang-format on
  EXPECT_EQ(
      FARM_PP_STRINGIZE(FARM_STRUCT_DETAILS_FROM_PROTO(
          dummy, farm_ng::StructExample1::FieldTypes, 0, (integer, SKIP))),
      expected_string);
}

TEST(FARM_STRUCT_DETAILS_TO_PROTO, macro) {
  // clang-format off
  std::string expected_string =
        "if constexpr (::farm_ng::proto_conv_details::IsPrimitive< std::tuple_element_t<1, farm_ng::StructExample1::FieldTypes>>::value) { "
            "proto.set_f( s.f); "
        "} else { "
            "*proto.mutable_f() = toProt(s.f); "
        "}";
  // clang-format on
  EXPECT_EQ(
      FARM_PP_STRINGIZE(FARM_STRUCT_DETAILS_TO_PROTO(
          dummy, farm_ng::StructExample1::FieldTypes, 1, (f))),
      expected_string);

  // clang-format off
  expected_string =
        "static_assert( \"SKIP\" == std::string_view(\"SKIP\"));";
  // clang-format on
  EXPECT_EQ(
      FARM_PP_STRINGIZE(FARM_STRUCT_DETAILS_TO_PROTO(
          dummy, farm_ng::StructExample1::FieldTypes, 1, (f, SKIP))),
      expected_string);
}

TEST(FARM_PROTO_CONV_IMPL, macro) {
  std::string expected_string =
      "static_assert( StructExample1::kNumFields == 1, "
      "\"Number of fields in FARM_PROTO_CONV_IMPL macro must match number of "
      "\" \"fields in \" \"StructExample1\" \" FARM_STRUCT\"); "  //
      "static_assert( StructExample1::kFieldNames[0] == \"integer\", "
      "\"Field\" \"0\" \" in FARM_PROTO_CONV_IMPL is `\" \"integer\" "
      "\"` but expected `\" \"StructExample1::kFieldNames[0]\" \"`.\");";
  std::string macro_start =
      std::string(FARM_PP_STRINGIZE(FARM_PROTO_CONV_IMPL(
                      StructExample1, core::proto::StructExample1, (integer))))
          .substr(0, 338);

  EXPECT_EQ(macro_start, expected_string);
}
