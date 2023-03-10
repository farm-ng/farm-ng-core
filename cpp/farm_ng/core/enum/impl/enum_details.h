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

// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <farm_pp/preprocessor/comparison/equal.hpp>
#include <farm_pp/preprocessor/control/if.hpp>
#include <farm_pp/preprocessor/punctuation/remove_parens.hpp>
#include <farm_pp/preprocessor/seq/for_each.hpp>
#include <farm_pp/preprocessor/seq/for_each_i.hpp>
#include <farm_pp/preprocessor/seq/pop_front.hpp>
#include <farm_pp/preprocessor/stringize.hpp>
#include <farm_pp/preprocessor/tuple/elem.hpp>
#include <farm_pp/preprocessor/tuple/size.hpp>
#include <farm_pp/preprocessor/tuple/to_seq.hpp>

#include <array>
#include <string>
#include <string_view>

// Input:  (A,2)           A              (A)
// Output: ((A,2))         ((A))          ((A))
#define FARM_ENUM_DETAILS_OP_MAKE_TUPLE(dummy1, dummy2, ENUM) \
  ((FARM_PP_REMOVE_PARENS(ENUM)))

// Input:  (A, (B,2) , C)
// Output: ((A))((B,2))((C))
#define FARM_ENUM_DETAILS_TO_SEQ_OF_TUPLES(...) \
  FARM_PP_SEQ_FOR_EACH(                         \
      FARM_ENUM_DETAILS_OP_MAKE_TUPLE, _, FARM_PP_TUPLE_TO_SEQ(__VA_ARGS__))

// INPUT:  size_t, (a,b, ...)
// Output: size_t
//
// INPUT:  (a,b, ...)
// Output: int
#define FARM_ENUM_DETAILS_GET_INT_TYPE(...)                \
  FARM_PP_IF(                                              \
      FARM_PP_EQUAL(FARM_PP_TUPLE_SIZE((__VA_ARGS__)), 2), \
      FARM_PP_TUPLE_ELEM(0, (__VA_ARGS__)),                \
      int)

// INPUT:  size_t, (a,b, ...)
// Output: (a,b, ...)
//
// INPUT:  (a,b, ...)
// Output: (a,b, ...)
#define FARM_ENUM_DETAILS_GET_VARS(...)                    \
  FARM_PP_REMOVE_PARENS(FARM_PP_IF(                        \
      FARM_PP_EQUAL(FARM_PP_TUPLE_SIZE((__VA_ARGS__)), 2), \
      (FARM_PP_TUPLE_ELEM(1, (__VA_ARGS__))),              \
      (__VA_ARGS__)))

// INPUT:  (A,2)           B              (C,3)
// Output: A=2             B              C=3
#define FARM_ENUM_DETAILS_OP_VALUE_WITH_INIT(PAIR) \
  FARM_PP_IF(                                      \
      FARM_PP_EQUAL(FARM_PP_TUPLE_SIZE(PAIR), 1),  \
      FARM_PP_TUPLE_ELEM(0, PAIR),                 \
      FARM_PP_TUPLE_ELEM(0, PAIR) = FARM_PP_TUPLE_ELEM(1, PAIR))

// INPUT:  (A,2)           B              (C,3)
// Output:  NAMEImpl::A    NAMEImpl::B    NAMEImpl::C
#define FARM_ENUM_DETAILS_ENUM_NS_OP(NAME, PAIR) \
  NAME##Impl::FARM_PP_TUPLE_ELEM(0, PAIR)

// INPUT:  (A,2)           B              (C,3)
// Output: , A=2           , B            , C=3
#define FARM_ENUM_DETAILS_OP_COMMA_VALUE(dummy1, dummy2, PAIR) \
  , FARM_ENUM_DETAILS_OP_VALUE_WITH_INIT(PAIR)

// INPUT:  (A,2)           B              (C,3)
// Output: , A           , B            , C
#define FARM_ENUM_DETAILS_NS_OP_COMMA(dummy1, NAME, PAIR) \
  , FARM_ENUM_DETAILS_ENUM_NS_OP(NAME, PAIR)

// Input:  ((A,2), B, C)
// Output: A=2, B, C
#define FARM_ENUM_DETAILS_CSV(...)                                          \
  FARM_ENUM_DETAILS_OP_VALUE_WITH_INIT(                                     \
      FARM_PP_SEQ_ELEM(0, FARM_ENUM_DETAILS_TO_SEQ_OF_TUPLES(__VA_ARGS__))) \
  FARM_PP_SEQ_FOR_EACH(                                                     \
      FARM_ENUM_DETAILS_OP_COMMA_VALUE,                                     \
      _,                                                                    \
      FARM_PP_SEQ_POP_FRONT(FARM_ENUM_DETAILS_TO_SEQ_OF_TUPLES(__VA_ARGS__)))

// Input:  ((A,2), B, C)
// Output: A, B, C
#define FARM_ENUM_NS_ENUM_CSV(NAME, ...)                                    \
  FARM_ENUM_DETAILS_ENUM_NS_OP(                                             \
      NAME,                                                                 \
      FARM_PP_SEQ_ELEM(0, FARM_ENUM_DETAILS_TO_SEQ_OF_TUPLES(__VA_ARGS__))) \
  FARM_PP_SEQ_FOR_EACH(                                                     \
      FARM_ENUM_DETAILS_NS_OP_COMMA,                                        \
      NAME,                                                                 \
      FARM_PP_SEQ_POP_FRONT(FARM_ENUM_DETAILS_TO_SEQ_OF_TUPLES(__VA_ARGS__)))

// Output:
//   case ${TYPE}::${PAIR}[0]: {
//     return "${PAIR}[0]";
//   }
#define FARM_ENUM_DETAILS_OP_TO_STRING_CASE(dummy1, TYPE, PAIR) \
  case TYPE::FARM_PP_TUPLE_ELEM(0, PAIR): {                     \
    return FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(0, PAIR));      \
  }

// Output:
//   case ${TYPE}::${PAIR}[0]: {
//     return "${PAIR}[0]" + std::string(" (=") +
//     std::to_string(INT_TYPE(value)) + std::string(")");
//   }
#define FARM_ENUM_DETAILS_OP_TO_PRETTY_CASE(dummy, types, PAIR)     \
  case FARM_PP_TUPLE_ELEM(0, types)::FARM_PP_TUPLE_ELEM(0, PAIR): { \
    return FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(0, PAIR)) +         \
           std::string(" (=") +                                     \
           std::to_string(FARM_PP_TUPLE_ELEM(1, types)(value)) +    \
           std::string(")");                                        \
  }

// Output:
//   if (str == "${PAIR}[0]") {
//     value = ${TYPE}::${PAIR}[0];
//     return true;
//   }
#define FARM_ENUM_DETAILS_OP_SET_VALUE_CASES(dummy1, TYPE, PAIR) \
  if (str == FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(0, PAIR))) {   \
    value = TYPE::FARM_PP_TUPLE_ELEM(0, PAIR);                   \
    return true;                                                 \
  }

// Output: , "${PAIR}[0]"
#define FARM_ENUM_DETAILS_OP_COMMA_STRING(dummy1, dummy2, PAIR) \
  , FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(0, PAIR))

// Input: (A, (B,3), (C,5))
// Output: "A", "B", "C"
#define FARM_ENUM_DETAILS_COMMA_SEP_STRINGS(...)                             \
  FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(                                      \
      0,                                                                     \
      FARM_PP_SEQ_ELEM(0, FARM_ENUM_DETAILS_TO_SEQ_OF_TUPLES(__VA_ARGS__)))) \
  FARM_PP_SEQ_FOR_EACH(                                                      \
      FARM_ENUM_DETAILS_OP_COMMA_STRING,                                     \
      _,                                                                     \
      FARM_PP_SEQ_POP_FRONT(FARM_ENUM_DETAILS_TO_SEQ_OF_TUPLES(__VA_ARGS__)))

// Output: ", ${PAIR}[0]"
#define FARM_ENUM_DETAILS_OP_COMMA_VALUE_STRING(dummy1, dummy2, PAIR) \
  ", " FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(0, PAIR))

// Output: case ${TYPE}::${PAIR}[0]: { return ${i}; }
#define FARM_ENUM_DETAILS_OP_POSITION_ICASE(dummy, TYPE, I, PAIR) \
  case TYPE::FARM_PP_TUPLE_ELEM(0, PAIR): {                       \
    return I;                                                     \
  }

// Input: (A, (B, 5), C)
// Output: "A, B, C"
#define FARM_ENUM_DETAILS_CSV_STRING(...)                                    \
  FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(                                      \
      0,                                                                     \
      FARM_PP_SEQ_ELEM(0, FARM_ENUM_DETAILS_TO_SEQ_OF_TUPLES(__VA_ARGS__)))) \
  FARM_PP_SEQ_FOR_EACH(                                                      \
      FARM_ENUM_DETAILS_OP_COMMA_VALUE_STRING,                               \
      _,                                                                     \
      FARM_PP_SEQ_POP_FRONT(FARM_ENUM_DETAILS_TO_SEQ_OF_TUPLES(__VA_ARGS__)))

// Output: , ${TYPES}[1](${TYPES}[0]::${PAIR}[0])
#define FARM_ENUM_DETAILS_OP_INTS(dummy, TYPES, PAIR) \
  , FARM_PP_TUPLE_ELEM(                               \
        1, TYPES)(FARM_PP_TUPLE_ELEM(0, TYPES)::FARM_PP_TUPLE_ELEM(0, PAIR))

// Input: (A, (B,5), (C,11))
// Output: 0, 5, 11
//
// Note: Here, "0, 5, 11" are the values of "A, B, C".
#define FARM_ENUM_DETAILS_COMMA_SEP_INTS(TYPE, INT_TYPE, ...)                \
  INT_TYPE(TYPE::FARM_PP_TUPLE_ELEM(                                         \
      0,                                                                     \
      FARM_PP_SEQ_ELEM(0, FARM_ENUM_DETAILS_TO_SEQ_OF_TUPLES(__VA_ARGS__)))) \
  FARM_PP_SEQ_FOR_EACH(                                                      \
      FARM_ENUM_DETAILS_OP_INTS,                                             \
      (TYPE, INT_TYPE),                                                      \
      FARM_PP_SEQ_POP_FRONT(FARM_ENUM_DETAILS_TO_SEQ_OF_TUPLES(__VA_ARGS__)))

#define FARM_ENUM_DEF_IMPL(NAME, ...)                                          \
  namespace enum_wrapper_ {                                                    \
  enum class NAME##Impl : FARM_ENUM_DETAILS_GET_INT_TYPE(__VA_ARGS__){         \
      FARM_ENUM_DETAILS_CSV(FARM_ENUM_DETAILS_GET_VARS(__VA_ARGS__))};         \
                                                                               \
  [[maybe_unused]] inline auto toString(NAME##Impl value) -> std::string {     \
    switch (value) {                                                           \
      FARM_PP_SEQ_FOR_EACH(                                                    \
          FARM_ENUM_DETAILS_OP_TO_STRING_CASE,                                 \
          NAME##Impl,                                                          \
          FARM_ENUM_DETAILS_TO_SEQ_OF_TUPLES(                                  \
              FARM_ENUM_DETAILS_GET_VARS(__VA_ARGS__)))                        \
    }                                                                          \
    std::abort();                                                              \
  }                                                                            \
                                                                               \
  [[maybe_unused]] inline auto constexpr toStringView(NAME##Impl value)        \
      -> std::string_view {                                                    \
    switch (value) {                                                           \
      FARM_PP_SEQ_FOR_EACH(                                                    \
          FARM_ENUM_DETAILS_OP_TO_STRING_CASE,                                 \
          NAME##Impl,                                                          \
          FARM_ENUM_DETAILS_TO_SEQ_OF_TUPLES(                                  \
              FARM_ENUM_DETAILS_GET_VARS(__VA_ARGS__)))                        \
    }                                                                          \
    std::abort();                                                              \
  }                                                                            \
                                                                               \
  [[maybe_unused]] inline auto toPretty(NAME##Impl value) -> std::string {     \
    switch (value) {                                                           \
      FARM_PP_SEQ_FOR_EACH(                                                    \
          FARM_ENUM_DETAILS_OP_TO_PRETTY_CASE,                                 \
          (NAME##Impl, FARM_ENUM_DETAILS_GET_INT_TYPE(__VA_ARGS__)),           \
          FARM_ENUM_DETAILS_TO_SEQ_OF_TUPLES(                                  \
              FARM_ENUM_DETAILS_GET_VARS(__VA_ARGS__)))                        \
    }                                                                          \
    std::abort();                                                              \
  }                                                                            \
                                                                               \
  [[maybe_unused]] [[nodiscard]] inline auto trySetFromString(                 \
      NAME##Impl &value, std::string const &str) -> bool {                     \
    FARM_PP_SEQ_FOR_EACH(                                                      \
        FARM_ENUM_DETAILS_OP_SET_VALUE_CASES,                                  \
        NAME##Impl,                                                            \
        FARM_ENUM_DETAILS_TO_SEQ_OF_TUPLES(                                    \
            FARM_ENUM_DETAILS_GET_VARS(__VA_ARGS__)))                          \
    return false;                                                              \
  }                                                                            \
                                                                               \
  [[maybe_unused]] [[nodiscard]] auto constexpr getCount(NAME##Impl)           \
      -> size_t {                                                              \
    return FARM_PP_TUPLE_SIZE(FARM_ENUM_DETAILS_GET_VARS(__VA_ARGS__));        \
  }                                                                            \
                                                                               \
  [[maybe_unused]] [[nodiscard]] inline auto getStrings(NAME##Impl)            \
      -> std::array<                                                           \
          std::string_view,                                                    \
          FARM_PP_TUPLE_SIZE(FARM_ENUM_DETAILS_GET_VARS(__VA_ARGS__))> {       \
    return {FARM_ENUM_DETAILS_COMMA_SEP_STRINGS(                               \
        FARM_ENUM_DETAILS_GET_VARS(__VA_ARGS__))};                             \
  }                                                                            \
                                                                               \
  [[maybe_unused]] [[nodiscard]] inline auto getStringOfNames(NAME##Impl)      \
      -> std::string_view {                                                    \
    return FARM_ENUM_DETAILS_CSV_STRING(                                       \
        FARM_ENUM_DETAILS_GET_VARS(__VA_ARGS__));                              \
  }                                                                            \
                                                                               \
  [[maybe_unused]] [[nodiscard]] auto constexpr getValues(NAME##Impl)          \
      -> std::array<                                                           \
          FARM_ENUM_DETAILS_GET_INT_TYPE(__VA_ARGS__),                         \
          FARM_PP_TUPLE_SIZE(FARM_ENUM_DETAILS_GET_VARS(__VA_ARGS__))> {       \
    return {FARM_ENUM_DETAILS_COMMA_SEP_INTS(                                  \
        NAME##Impl,                                                            \
        FARM_ENUM_DETAILS_GET_INT_TYPE(__VA_ARGS__),                           \
        FARM_ENUM_DETAILS_GET_VARS(__VA_ARGS__))};                             \
  }                                                                            \
                                                                               \
  [[maybe_unused]] [[nodiscard]] auto constexpr getAll(NAME##Impl)             \
      -> std::array<                                                           \
          NAME##Impl,                                                          \
          FARM_PP_TUPLE_SIZE(FARM_ENUM_DETAILS_GET_VARS(__VA_ARGS__))> {       \
    /* return {NAME##Impl::apple, NAME##Impl::banana, NAME##Impl::pear}; */    \
    return {                                                                   \
        FARM_ENUM_NS_ENUM_CSV(NAME, FARM_ENUM_DETAILS_GET_VARS(__VA_ARGS__))}; \
  }                                                                            \
                                                                               \
  [[maybe_unused]] [[nodiscard]] auto constexpr getPosition(NAME##Impl value)  \
      -> size_t {                                                              \
    switch (value) {                                                           \
      FARM_PP_SEQ_FOR_EACH_I(                                                  \
          FARM_ENUM_DETAILS_OP_POSITION_ICASE,                                 \
          NAME##Impl,                                                          \
          FARM_ENUM_DETAILS_TO_SEQ_OF_TUPLES(                                  \
              FARM_ENUM_DETAILS_GET_VARS(__VA_ARGS__)))                        \
    }                                                                          \
    std::abort();                                                              \
  }                                                                            \
                                                                               \
  [[maybe_unused]] [[nodiscard]] inline auto getTypeName(NAME##Impl)           \
      -> std::string_view {                                                    \
    return FARM_PP_STRINGIZE(NAME);                                            \
  }                                                                            \
  }  // namespace enum_wrapper_
