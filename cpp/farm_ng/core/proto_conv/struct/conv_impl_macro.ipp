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
//   field_names:  TCpp::kFieldNames
//   index:        0
//   tuple:        (Type0, name0, {})
//
// Output: static_assert(TCpp::kFieldNames[i] == "name0");
#define FARM_STRUCT_DETAILS_ASSERT_FIELD_NAME(r, field_names, index, tuple) \
  static_assert(                                                             \
      field_names[index] == FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(0, tuple)), \
      "Field #" FARM_PP_STRINGIZE(index) " in FARM_PROTO_CONV_IMPL is `"       \
      FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(0, tuple)) "` but expected `"     \
      FARM_PP_STRINGIZE(field_names[index]) "`.");

// Given a sequence of triplets, create a static_assert which checks that the
// expected fields names match the stringized names.
//
// Example
// Input:
//   field_names:  TCpp::kFieldNames
//   field_seq:    ((Type0, name0, {})) ((Type1, name1, {}))
//
// Output:
//     static_assert(TCpp::kFieldNames[0] == "name0");
//     static_assert(TCpp::kFieldNames[1] == "name1");
#define FARM_STRUCT_DETAILS_ASSERT_FILED_NAME_LOOP(cpp, field_seq) \
  FARM_PP_SEQ_FOR_EACH_I(                                          \
      FARM_STRUCT_DETAILS_ASSERT_FIELD_NAME, cpp::kFieldNames, field_seq)

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
#define FARM_STRUCT_DETAILS_FROM_PROTO(dummy1, field_types, index, tuple)      \
  FARM_PP_IF(                                                                  \
      FARM_PP_EQUAL(FARM_PP_TUPLE_SIZE(tuple), 2),                             \
      static_assert(                                                           \
          FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(1, tuple)) ==                   \
          std::string_view("SKIP"));                                           \
      ,                                                                        \
      if constexpr (::farm_ng::proto_conv_details::IsPrimitive<                \
                        std::tuple_element_t<index, field_types>>::value) {    \
        s.FARM_PP_TUPLE_ELEM(0, tuple) = proto.FARM_PP_TUPLE_ELEM(0, tuple)(); \
      } else {                                                                 \
        FARM_TRY(                                                              \
            auto,                                                              \
            res,                                                               \
            ::farm_ng::fromProt(proto.FARM_PP_TUPLE_ELEM(0, tuple)()));        \
        s.FARM_PP_TUPLE_ELEM(0, tuple) = res;                                  \
      })

// Given a sequence of triplets, create from-proto conversion code for each
// field.
#define FARM_STRUCT_DETAILS_FROM_PROTO_LOOP(cpp, field_seq) \
  FARM_PP_SEQ_FOR_EACH_I(                                   \
      FARM_STRUCT_DETAILS_FROM_PROTO, typename cpp::FieldTypes, field_seq)

// Given a tuple of tuple, either
//
//    if tuple size == 2, then skip the field (but also assert that
//    ${tuple}[1] == "SKIP")
//
//    else, create to-proto conversion code for the field
//
// See farm_ng_core/cpp/farm_ng/core/core/struct/proto_conv_test.cpp
// for examples.
#define FARM_STRUCT_DETAILS_TO_PROTO(dummy1, field_types, index, tuple)     \
  FARM_PP_IF(                                                               \
      FARM_PP_EQUAL(FARM_PP_TUPLE_SIZE(tuple), 2),                          \
      static_assert(                                                        \
          FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(1, tuple)) ==                \
          std::string_view("SKIP"));                                        \
      ,                                                                     \
      if constexpr (::farm_ng::proto_conv_details::IsPrimitive<             \
                        std::tuple_element_t<index, field_types>>::value) { \
        proto.FARM_PP_CAT(set_, FARM_PP_TUPLE_ELEM(0, tuple))(              \
            s.FARM_PP_TUPLE_ELEM(0, tuple));                                \
      } else {                                                              \
        *proto.FARM_PP_CAT(mutable_, FARM_PP_TUPLE_ELEM(0, tuple))() =      \
            toProt(s.FARM_PP_TUPLE_ELEM(0, tuple));                         \
      })

// Given a sequence of triplets, create to-proto conversion code for each
// field.
#define FARM_STRUCT_DETAILS_TO_PROTO_LOOP(cpp, field_seq) \
  FARM_PP_SEQ_FOR_EACH_I(                                 \
      FARM_STRUCT_DETAILS_TO_PROTO, typename cpp::FieldTypes, field_seq)

// Implementation details for FARM_PROTO_CONV_IMPL_FN. It takes a sequence of
// triplets, such as ((Type0, name0, init0)) ((Type1, name1, init1)) ... instead
// of a tuple of triplets.
#define FARM_PROTO_DETAILS_CONV_IMPL_FROM_SEQ(                                 \
    Cpp, Proto, from_proto_user_fn, to_proto_user_fn, num_fields, field_seq)   \
  static_assert(                                                               \
      Cpp::kNumFields == num_fields,                                           \
      "Number of fields in FARM_PROTO_CONV_IMPL macro must match number of "   \
      "fields in " FARM_PP_STRINGIZE(Cpp) " FARM_STRUCT");                     \
  FARM_STRUCT_DETAILS_ASSERT_FILED_NAME_LOOP(Cpp, field_seq)                   \
                                                                               \
  /* This indirection is required. "if constexpr" branches are only skipped */ \
  /* if the condition depends on a template parameter, e.g. TCpp.*/            \
  template <>                                                                  \
  struct FromProtImpl<Cpp, Proto> {                                            \
    template <class TProto = Proto, class TCpp = Cpp>                          \
    static auto impl(TProto const& proto) -> Expected<TCpp> {                  \
      TCpp s;                                                                  \
      FARM_STRUCT_DETAILS_FROM_PROTO_LOOP(TCpp, field_seq)                     \
      return from_proto_user_fn(std::move(s), proto);                          \
    }                                                                          \
  };                                                                           \
                                                                               \
  /* This indirection is required. See above.*/                                \
  template <>                                                                  \
  struct ToProtImpl<Cpp, Proto> {                                              \
    template <class TProto = Proto, class TCpp = Cpp>                          \
    static auto impl(TCpp const& s) -> TProto {                                \
      TProto proto;                                                            \
      FARM_STRUCT_DETAILS_TO_PROTO_LOOP(TCpp, field_seq)                       \
      return to_proto_user_fn(std::move(proto), s);                            \
    }                                                                          \
  };                                                                           \
                                                                               \
  template <>                                                                  \
  auto fromProt<Proto>(Proto const& proto)->Expected<Cpp> {                    \
    return FromProtImpl<Cpp, Proto>::impl(proto);                              \
  }                                                                            \
                                                                               \
  template <>                                                                  \
  auto toProt<Cpp>(Cpp const& s)->Proto {                                      \
    return ToProtImpl<Cpp, Proto>::impl(s);                                    \
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
#define FARM_PROTO_CONV_IMPL_FN(                                       \
    Cpp, Proto, tuple_of_fields, from_proto_user_fn, to_proto_user_fn) \
  FARM_PROTO_DETAILS_CONV_IMPL_FROM_SEQ(                               \
      Cpp,                                                             \
      Proto,                                                           \
      from_proto_user_fn,                                              \
      to_proto_user_fn,                                                \
      FARM_PP_TUPLE_SIZE(tuple_of_fields),                             \
      FARM_MACROLIB_SEQ_OF_TUPLES_FROM_TUPLE(tuple_of_fields))

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
#define FARM_PROTO_CONV_IMPL(Cpp, Proto, tuple_of_fields)                    \
  FARM_PROTO_DETAILS_CONV_IMPL_FROM_SEQ(                                     \
      Cpp,                                                                   \
      Proto,                                                                 \
      [](Cpp&& s, Proto const& proto) -> Expected<Cpp> {                     \
        return std::move(s);                                                 \
      },                                                                     \
      [](Proto&& proto, Cpp const& s) -> Proto { return std::move(proto); }, \
      FARM_PP_TUPLE_SIZE(tuple_of_fields),                                   \
      FARM_MACROLIB_SEQ_OF_TUPLES_FROM_TUPLE(tuple_of_fields))
