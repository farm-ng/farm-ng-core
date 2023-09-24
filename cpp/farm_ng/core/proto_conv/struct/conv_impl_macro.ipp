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

// NOTE: This file should typically only be included in *.cpp files - to keep
// compile times low.

#include "farm_ng/core/struct/macro_lib.h"

#include <farm_ng/core/logging/expected.h>
#include <farm_ng/core/logging/logger.h>
#include <farm_ng/core/proto_conv/traits.h>
#include <farm_pp/preprocessor/comparison/equal.hpp>
#include <farm_pp/preprocessor/control/if.hpp>
#include <farm_pp/preprocessor/seq/for_each_i.hpp>
#include <farm_pp/preprocessor/stringize.hpp>
#include <farm_pp/preprocessor/tuple/elem.hpp>
#include <farm_pp/preprocessor/tuple/size.hpp>

// Given a tuple, such as (Type, name, {}), return a static_assert which checks
// that the expected fields name matches the stringized name.
//
// Example:
//
// Inputs
//   Field_Names_:  TCpp::kFieldNames
//   Index_:        0
//   tuple:        (Type0, name0, {})
//
// Output: static_assert(TCpp::kFieldNames[i] == "name0");
#define FARM_STRUCT_DETAILS_ASSERT_FIELD_NAME(    \
    Dummy_, Field_Names_, Index_, Type_Name_Init) \
  static_assert(                                                             \
      Field_Names_[Index_] == \
      FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(0, Type_Name_Init)), \
      "Field #" FARM_PP_STRINGIZE(Index_) " in FARM_PROTO_CONV_IMPL is `"       \
      FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(0, Type_Name_Init)) "` but expected `"     \
      FARM_PP_STRINGIZE(Field_Names_[Index_]) "`.");

// Given a sequence of triplets, create a static_assert which checks that the
// expected fields names match the stringized names.
//
// Example
// Input:
//   Field_Names_:  TCpp::kFieldNames
//   field_seq:    ((Type0, name0, {})) ((Type1, name1, {}))
//
// Output:
//     static_assert(TCpp::kFieldNames[0] == "name0");
//     static_assert(TCpp::kFieldNames[1] == "name1");
#define FARM_STRUCT_DETAILS_ASSERT_FILED_NAME_LOOP(Cpp_Type_, Field_Seq_) \
  FARM_PP_SEQ_FOR_EACH_I(                                                 \
      FARM_STRUCT_DETAILS_ASSERT_FIELD_NAME,                              \
      Cpp_Type_::kFieldNames,                                             \
      Field_Seq_)

namespace farm_ng {
namespace proto_conv_details {

template <class Type>
struct IsPrimitive {
  static bool constexpr value =
      std::is_arithmetic_v<Type> || std::is_same_v<std::string, Type>;
};

}  // namespace proto_conv_details

template <class TCppTemplate, class TProtoTemp>
struct FromProtImpl;

template <class TCppTemplate, class TProtoTemp>
struct ToProtImpl;
}  // namespace farm_ng

// Given a tuple of tuple, either
//
//    if tuple size == 2, then skip the field (but also assert that
//    ${tuple}[1] == "SKIP")
//
//    else, create from-proto conversion code for the field
//
// See farm_ng_core/cpp/farm_ng/core/core/struct/proto_conv_test.cpp
// for examples.
#define FARM_STRUCT_DETAILS_FROM_PROTO(Dummy1_, Field_Types_, Index_, Tuple_) \
  FARM_PP_IF(                                                                 \
      FARM_PP_EQUAL(FARM_PP_TUPLE_SIZE(Tuple_), 2),                           \
      static_assert(                                                          \
          FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(1, Tuple_)) ==                 \
          std::string_view("SKIP"));                                          \
      ,                                                                       \
      if constexpr (::farm_ng::proto_conv_details::IsPrimitive<               \
                        std::tuple_element_t<Index_, Field_Types_>>::value) { \
        s.FARM_PP_TUPLE_ELEM(0, Tuple_) =                                     \
            proto.FARM_PP_TUPLE_ELEM(0, Tuple_)();                            \
      } else {                                                                \
        FARM_TRY(                                                             \
            auto,                                                             \
            res,                                                              \
            ::farm_ng::fromProt(proto.FARM_PP_TUPLE_ELEM(0, Tuple_)()));      \
        s.FARM_PP_TUPLE_ELEM(0, Tuple_) = res;                                \
      })

// Given a sequence of triplets, create from-proto conversion code for each
// field.
#define FARM_STRUCT_DETAILS_FROM_PROTO_LOOP(Cpp_Type_, Field_Seq_) \
  FARM_PP_SEQ_FOR_EACH_I(                                          \
      FARM_STRUCT_DETAILS_FROM_PROTO,                              \
      typename Cpp_Type_::FieldTypes,                              \
      Field_Seq_)

// Given a tuple of tuple, either
//
//    if tuple size == 2, then skip the field (but also assert that
//    ${tuple}[1] == "SKIP")
//
//    else, create to-proto conversion code for the field
//
// See farm_ng_core/cpp/farm_ng/core/core/struct/proto_conv_test.cpp
// for examples.
#define FARM_STRUCT_DETAILS_TO_PROTO(Dummy1_, Field_Types_, Index_, Tuple_)   \
  FARM_PP_IF(                                                                 \
      FARM_PP_EQUAL(FARM_PP_TUPLE_SIZE(Tuple_), 2),                           \
      static_assert(                                                          \
          FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(1, Tuple_)) ==                 \
          std::string_view("SKIP"));                                          \
      ,                                                                       \
      if constexpr (::farm_ng::proto_conv_details::IsPrimitive<               \
                        std::tuple_element_t<Index_, Field_Types_>>::value) { \
        proto.FARM_PP_CAT(set_, FARM_PP_TUPLE_ELEM(0, Tuple_))(               \
            s.FARM_PP_TUPLE_ELEM(0, Tuple_));                                 \
      } else {                                                                \
        *proto.FARM_PP_CAT(mutable_, FARM_PP_TUPLE_ELEM(0, Tuple_))() =       \
            toProt(s.FARM_PP_TUPLE_ELEM(0, Tuple_));                          \
      })

// Given a sequence of triplets, create to-proto conversion code for each
// field.
#define FARM_STRUCT_DETAILS_TO_PROTO_LOOP(Cpp_Type_, Field_Seq_) \
  FARM_PP_SEQ_FOR_EACH_I(                                        \
      FARM_STRUCT_DETAILS_TO_PROTO,                              \
      typename Cpp_Type_::FieldTypes,                            \
      Field_Seq_)

// Implementation details for FARM_PROTO_CONV_IMPL_FN. It takes a sequence of
// triplets, such as ((Type0, name0, init0)) ((Type1, name1, init1)) ... instead
// of a tuple of triplets.
#define FARM_PROTO_DETAILS_CONV_IMPL_FROM_SEQ(                                 \
    Cpp_Type_,                                                                 \
    Proto_Type_,                                                               \
    From_Proto_User_Fn_,                                                       \
    To_Proto_User_Fn_,                                                         \
    Num_Fields_,                                                               \
    Field_Seq_)                                                                \
  static_assert(                                                               \
      Cpp_Type_::kNumFields == Num_Fields_,                                    \
      "Number of fields in FARM_PROTO_CONV_IMPL macro must match number of "   \
      "fields in " FARM_PP_STRINGIZE(Cpp_Type_) " FARM_STRUCT");               \
  FARM_STRUCT_DETAILS_ASSERT_FILED_NAME_LOOP(Cpp_Type_, Field_Seq_)            \
                                                                               \
  /* This indirection is required. "if constexpr" branches are only skipped */ \
  /* if the condition depends on a template parameter, e.g. TCpp.*/            \
  template <>                                                                  \
  struct FromProtImpl<Cpp_Type_, Proto_Type_> {                                \
    template <class TProto = Proto_Type_, class TCpp = Cpp_Type_>              \
    static auto impl(TProto const& proto) -> Expected<TCpp> {                  \
      TCpp s;                                                                  \
      FARM_STRUCT_DETAILS_FROM_PROTO_LOOP(TCpp, Field_Seq_)                    \
      return From_Proto_User_Fn_(std::move(s), proto);                         \
    }                                                                          \
  };                                                                           \
                                                                               \
  /* This indirection is required. See above.*/                                \
  template <>                                                                  \
  struct ToProtImpl<Cpp_Type_, Proto_Type_> {                                  \
    template <class TProto = Proto_Type_, class TCpp = Cpp_Type_>              \
    static auto impl(TCpp const& s) -> TProto {                                \
      TProto proto;                                                            \
      FARM_STRUCT_DETAILS_TO_PROTO_LOOP(TCpp, Field_Seq_)                      \
      return To_Proto_User_Fn_(std::move(proto), s);                           \
    }                                                                          \
  };                                                                           \
                                                                               \
  template <>                                                                  \
  auto fromProt<Proto_Type_>(Proto_Type_ const& proto)                         \
      -> Expected<Cpp_Type_> {                                                 \
    return FromProtImpl<Cpp_Type_, Proto_Type_>::impl(proto);                  \
  }                                                                            \
                                                                               \
  template <>                                                                  \
  auto toProt<Cpp_Type_>(Cpp_Type_ const& s) -> Proto_Type_ {                  \
    return ToProtImpl<Cpp_Type_, Proto_Type_>::impl(s);                        \
  }

// Given a Cpp type, a Proto type and a tuple of field triplets, and two user
// provided functions, create from-proto and to-proto conversion code. Fields
// with the SKIP tag, no default conversion code is generated. Instead, the
// user-provided functions can be used to provide custom conversion mechanisms
// if desired.
//
// Example:
//
//  FARM_PROTO_CONV_IMPL_FN(
//      RobotPose,
//      proto::RobotPose,
//      ((debug_bucket, SKIP)
//       world_from_from_robot)
//      [](RobotPose& s,
//         proto::RobotPose const& proto)
//          -> Expected<Success> {
//        FARM_TRY(
//            auto,
//            debug_bucket,
//            customDebugExtractor(proto.debug_blob()));
//        s.debug_bucket = debug_bucket;
//        return Success{};
//      },
//      [](proto::RobotPose& proto,
//         RobotPose const& s) {
//       *proto.mutable_debug_blob() = customDebugInjector(s.debug_bucket);
//      });
//
// In this example, world_from_from_robot is serialized using the default
// mechanism, but debug_bucket is serialized using user provided custom
// conversion functions.
#define FARM_PROTO_CONV_IMPL_FN(            \
    Cpp_Type_,                              \
    Proto_Type_,                            \
    Tuple_Of_Fields_,                       \
    From_Proto_User_Fn_,                    \
    To_Proto_User_Fn_)                      \
  FARM_PROTO_DETAILS_CONV_IMPL_FROM_SEQ(    \
      Cpp_Type_,                            \
      Proto_Type_,                          \
      From_Proto_User_Fn_,                  \
      To_Proto_User_Fn_,                    \
      FARM_PP_TUPLE_SIZE(Tuple_Of_Fields_), \
      FARM_MACROLIB_SEQ_OF_TUPLES_FROM_TUPLE(Tuple_Of_Fields_))

// Given a Cpp type, a Proto type and a tuple of field triplets, create
// from-proto and to-proto conversion code. For fields with the SKIP tag, no
// conversion code is generated.
//
// Example:
//
//  FARM_PROTO_CONV_IMPL(
//      RobotPose,
//      proto::RobotPose,
//      ((debug_bucket, SKIP)
//       world_from_from_robot));
//
// Note: In this example, debug_bucket is not serialized. Use
// FARM_PROTO_CONV_IMPL_FN if you like to implement custom serialization for
// debug_bucket instead.
#define FARM_PROTO_CONV_IMPL(Cpp_Type_, Proto_Type_, Tuple_Of_Fields_)     \
  FARM_PROTO_DETAILS_CONV_IMPL_FROM_SEQ(                                   \
      Cpp_Type_,                                                           \
      Proto_Type_,                                                         \
      [](Cpp_Type_&& s, Proto_Type_ const& proto) -> Expected<Cpp_Type_> { \
        return std::move(s);                                               \
      },                                                                   \
      [](Proto_Type_&& proto, Cpp_Type_ const& s) -> Proto_Type_ {         \
        return std::move(proto);                                           \
      },                                                                   \
      FARM_PP_TUPLE_SIZE(Tuple_Of_Fields_),                                \
      FARM_MACROLIB_SEQ_OF_TUPLES_FROM_TUPLE(Tuple_Of_Fields_))
