// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include "farm_ng_core/enum/impl/compiler_attributes.h"
#include "farm_ng_core/logging/logger.h"

#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/punctuation/remove_parens.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/pop_front.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/size.hpp>
#include <boost/preprocessor/tuple/to_seq.hpp>

#include <array>
#include <string>
#include <string_view>

// Input:  (A,2)           A              (A)
// Output: ((A,2))         ((A))          ((A))
#define FNG_ENUM_DETAILS_OP_MAKE_TUPLE(dummy1, dummy2, ENUM) \
  ((BOOST_PP_REMOVE_PARENS(ENUM)))

// Input:  (A, (B,2) , C)
// Output: ((A))((B,2))((C))
#define FNG_ENUM_DETAILS_TO_SEQ_OF_TUPLES(...) \
  BOOST_PP_SEQ_FOR_EACH(                       \
      FNG_ENUM_DETAILS_OP_MAKE_TUPLE, _, BOOST_PP_TUPLE_TO_SEQ(__VA_ARGS__))

// INPUT:  size_t, (a,b, ...)
// Output: size_t
//
// INPUT:  (a,b, ...)
// Output: int
#define FNG_ENUM_DETAILS_GET_INT_TYPE(...)                   \
  BOOST_PP_IF(                                               \
      BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE((__VA_ARGS__)), 2), \
      BOOST_PP_TUPLE_ELEM(0, (__VA_ARGS__)),                 \
      int)

// INPUT:  size_t, (a,b, ...)
// Output: (a,b, ...)
//
// INPUT:  (a,b, ...)
// Output: (a,b, ...)
#define FNG_ENUM_DETAILS_GET_VARS(...)                       \
  BOOST_PP_REMOVE_PARENS(BOOST_PP_IF(                        \
      BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE((__VA_ARGS__)), 2), \
      (BOOST_PP_TUPLE_ELEM(1, (__VA_ARGS__))),               \
      (__VA_ARGS__)))

// INPUT:  (A,2)           B              (C,3)
// Output: , A=2           , B            , C=3
#define FNG_ENUM_DETAILS_OP_VALUE_WITH_INIT(PAIR)   \
  BOOST_PP_IF(                                      \
      BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE(PAIR), 1), \
      BOOST_PP_TUPLE_ELEM(0, PAIR),                 \
      BOOST_PP_TUPLE_ELEM(0, PAIR) = BOOST_PP_TUPLE_ELEM(1, PAIR))

// INPUT:  (A,2)           B              (C,3)
// Output: , A=2           , B            , C=3
#define FNG_ENUM_DETAILS_OP_COMMA_VALUE(dummy1, dummy2, PAIR) \
  , FNG_ENUM_DETAILS_OP_VALUE_WITH_INIT(PAIR)

// Input:  ((A,2), B, C)
// Output: A=2, B, C
#define FNG_ENUM_DETAILS_CSV(...)                                           \
  FNG_ENUM_DETAILS_OP_VALUE_WITH_INIT(                                      \
      BOOST_PP_SEQ_ELEM(0, FNG_ENUM_DETAILS_TO_SEQ_OF_TUPLES(__VA_ARGS__))) \
  BOOST_PP_SEQ_FOR_EACH(                                                    \
      FNG_ENUM_DETAILS_OP_COMMA_VALUE,                                      \
      _,                                                                    \
      BOOST_PP_SEQ_POP_FRONT(FNG_ENUM_DETAILS_TO_SEQ_OF_TUPLES(__VA_ARGS__)))

// Output:
//   case ${TYPE}::${PAIR}[0]: {
//     return "${PAIR}[0]";
//   }
#define FNG_ENUM_DETAILS_OP_TO_STRING_CASE(dummy1, TYPE, PAIR) \
  case TYPE::BOOST_PP_TUPLE_ELEM(0, PAIR): {                   \
    return BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(0, PAIR));   \
  }

// Output:
//   case ${TYPE}::${PAIR}[0]: {
//     return "${PAIR}[0]" + std::string(" (=") +
//     std::to_string(INT_TYPE(value)) + std::string(")");
//   }
#define FNG_ENUM_DETAILS_OP_TO_PRETTY_CASE(dummy, types, PAIR)        \
  case BOOST_PP_TUPLE_ELEM(0, types)::BOOST_PP_TUPLE_ELEM(0, PAIR): { \
    return BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(0, PAIR)) +         \
           std::string(" (=") +                                       \
           std::to_string(BOOST_PP_TUPLE_ELEM(1, types)(value)) +     \
           std::string(")");                                          \
  }

// Output:
//   if (str == "${PAIR}[0]") {
//     value = ${TYPE}::${PAIR}[0];
//     return true;
//   }
#define FNG_ENUM_DETAILS_OP_SET_VALUE_CASES(dummy1, TYPE, PAIR)  \
  if (str == BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(0, PAIR))) { \
    value = TYPE::BOOST_PP_TUPLE_ELEM(0, PAIR);                  \
    return true;                                                 \
  }

// Output: , "${PAIR}[0]"
#define FNG_ENUM_DETAILS_OP_COMMA_STRING(dummy1, dummy2, PAIR) \
  , BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(0, PAIR))

// Input: (A, (B,3), (C,5))
// Output: "A", "B", "C"
#define FNG_ENUM_DETAILS_COMMA_SEP_STRINGS(...)                              \
  BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(                                    \
      0,                                                                     \
      BOOST_PP_SEQ_ELEM(0, FNG_ENUM_DETAILS_TO_SEQ_OF_TUPLES(__VA_ARGS__)))) \
  BOOST_PP_SEQ_FOR_EACH(                                                     \
      FNG_ENUM_DETAILS_OP_COMMA_STRING,                                      \
      _,                                                                     \
      BOOST_PP_SEQ_POP_FRONT(FNG_ENUM_DETAILS_TO_SEQ_OF_TUPLES(__VA_ARGS__)))

// Output: ", ${PAIR}[0]"
#define FNG_ENUM_DETAILS_OP_COMMA_VALUE_STRING(dummy1, dummy2, PAIR) \
  ", " BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(0, PAIR))

// Output: case ${TYPE}::${PAIR}[0]: { return ${i}; }
#define FNG_ENUM_DETAILS_OP_POSITION_ICASE(dummy, TYPE, I, PAIR) \
  case TYPE::BOOST_PP_TUPLE_ELEM(0, PAIR): {                     \
    return I;                                                    \
  }

// Input: (A, (B, 5), C)
// Output: "A, B, C"
#define FNG_ENUM_DETAILS_CSV_STRING(...)                                     \
  BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(                                    \
      0,                                                                     \
      BOOST_PP_SEQ_ELEM(0, FNG_ENUM_DETAILS_TO_SEQ_OF_TUPLES(__VA_ARGS__)))) \
  BOOST_PP_SEQ_FOR_EACH(                                                     \
      FNG_ENUM_DETAILS_OP_COMMA_VALUE_STRING,                                \
      _,                                                                     \
      BOOST_PP_SEQ_POP_FRONT(FNG_ENUM_DETAILS_TO_SEQ_OF_TUPLES(__VA_ARGS__)))

// Output: , ${TYPES}[1](${TYPES}[0]::${PAIR}[0])
#define FNG_ENUM_DETAILS_OP_INTS(dummy, TYPES, PAIR) \
  , BOOST_PP_TUPLE_ELEM(                             \
        1, TYPES)(BOOST_PP_TUPLE_ELEM(0, TYPES)::BOOST_PP_TUPLE_ELEM(0, PAIR))

// Input: (A, (B,5), (C,11))
// Output: 0, 5, 11
//
// Note: Here, "0, 5, 11" are the values of "A, B, C".
#define FNG_ENUM_DETAILS_COMMA_SEP_INTS(TYPE, INT_TYPE, ...)                 \
  INT_TYPE(TYPE::BOOST_PP_TUPLE_ELEM(                                        \
      0,                                                                     \
      BOOST_PP_SEQ_ELEM(0, FNG_ENUM_DETAILS_TO_SEQ_OF_TUPLES(__VA_ARGS__)))) \
  BOOST_PP_SEQ_FOR_EACH(                                                     \
      FNG_ENUM_DETAILS_OP_INTS,                                              \
      (TYPE, INT_TYPE),                                                      \
      BOOST_PP_SEQ_POP_FRONT(FNG_ENUM_DETAILS_TO_SEQ_OF_TUPLES(__VA_ARGS__)))

#define FNG_ENUM_DEF_IMPL(NAME, ...)                                     \
  namespace enum_wrapper_ {                                              \
  enum class NAME##Impl : FNG_ENUM_DETAILS_GET_INT_TYPE(__VA_ARGS__){    \
      FNG_ENUM_DETAILS_CSV(FNG_ENUM_DETAILS_GET_VARS(__VA_ARGS__))};     \
                                                                         \
  FNG_ENUM_MAYBE_UNUSED inline std::string toString(NAME##Impl value) {  \
    switch (value) {                                                     \
      BOOST_PP_SEQ_FOR_EACH(                                             \
          FNG_ENUM_DETAILS_OP_TO_STRING_CASE,                            \
          NAME##Impl,                                                    \
          FNG_ENUM_DETAILS_TO_SEQ_OF_TUPLES(                             \
              FNG_ENUM_DETAILS_GET_VARS(__VA_ARGS__)))                   \
    }                                                                    \
    FNG_FATAL(                                                           \
        BOOST_PP_STRINGIZE(NAME) " does contain invalid value: {}",      \
        FNG_ENUM_DETAILS_GET_INT_TYPE(__VA_ARGS__)(value));              \
  }                                                                      \
                                                                         \
  FNG_ENUM_MAYBE_UNUSED inline std::string_view toStringView(            \
      NAME##Impl value) {                                                \
    switch (value) {                                                     \
      BOOST_PP_SEQ_FOR_EACH(                                             \
          FNG_ENUM_DETAILS_OP_TO_STRING_CASE,                            \
          NAME##Impl,                                                    \
          FNG_ENUM_DETAILS_TO_SEQ_OF_TUPLES(                             \
              FNG_ENUM_DETAILS_GET_VARS(__VA_ARGS__)))                   \
    }                                                                    \
    FNG_FATAL(                                                           \
        BOOST_PP_STRINGIZE(NAME) " does contain invalid value: {}",      \
        FNG_ENUM_DETAILS_GET_INT_TYPE(__VA_ARGS__)(value));              \
  }                                                                      \
                                                                         \
  FNG_ENUM_MAYBE_UNUSED inline std::string toPretty(NAME##Impl value) {  \
    switch (value) {                                                     \
      BOOST_PP_SEQ_FOR_EACH(                                             \
          FNG_ENUM_DETAILS_OP_TO_PRETTY_CASE,                            \
          (NAME##Impl, FNG_ENUM_DETAILS_GET_INT_TYPE(__VA_ARGS__)),      \
          FNG_ENUM_DETAILS_TO_SEQ_OF_TUPLES(                             \
              FNG_ENUM_DETAILS_GET_VARS(__VA_ARGS__)))                   \
    }                                                                    \
    FNG_FATAL(                                                           \
        BOOST_PP_STRINGIZE(NAME) " does contain invalid value: {}",      \
        FNG_ENUM_DETAILS_GET_INT_TYPE(__VA_ARGS__)(value));              \
  }                                                                      \
                                                                         \
  FNG_ENUM_MAYBE_UNUSED FNG_ENUM_NODISCARD inline bool trySetFromString( \
      NAME##Impl &value, const std::string &str) {                       \
    BOOST_PP_SEQ_FOR_EACH(                                               \
        FNG_ENUM_DETAILS_OP_SET_VALUE_CASES,                             \
        NAME##Impl,                                                      \
        FNG_ENUM_DETAILS_TO_SEQ_OF_TUPLES(                               \
            FNG_ENUM_DETAILS_GET_VARS(__VA_ARGS__)))                     \
    return false;                                                        \
  }                                                                      \
                                                                         \
  FNG_ENUM_MAYBE_UNUSED FNG_ENUM_NODISCARD constexpr size_t getCount(    \
      NAME##Impl) {                                                      \
    return BOOST_PP_TUPLE_SIZE(FNG_ENUM_DETAILS_GET_VARS(__VA_ARGS__));  \
  }                                                                      \
                                                                         \
  FNG_ENUM_MAYBE_UNUSED                                                  \
  FNG_ENUM_NODISCARD inline std::array<                                  \
      std::string_view,                                                  \
      BOOST_PP_TUPLE_SIZE(FNG_ENUM_DETAILS_GET_VARS(__VA_ARGS__))>       \
  getStrings(NAME##Impl) {                                               \
    return {FNG_ENUM_DETAILS_COMMA_SEP_STRINGS(                          \
        FNG_ENUM_DETAILS_GET_VARS(__VA_ARGS__))};                        \
  }                                                                      \
                                                                         \
  FNG_ENUM_MAYBE_UNUSED FNG_ENUM_NODISCARD inline std::string_view       \
  getStringOfNames(NAME##Impl) {                                         \
    return FNG_ENUM_DETAILS_CSV_STRING(                                  \
        FNG_ENUM_DETAILS_GET_VARS(__VA_ARGS__));                         \
  }                                                                      \
                                                                         \
  FNG_ENUM_MAYBE_UNUSED FNG_ENUM_NODISCARD constexpr std::array<         \
      FNG_ENUM_DETAILS_GET_INT_TYPE(__VA_ARGS__),                        \
      BOOST_PP_TUPLE_SIZE(FNG_ENUM_DETAILS_GET_VARS(__VA_ARGS__))>       \
  getValues(NAME##Impl) {                                                \
    return {FNG_ENUM_DETAILS_COMMA_SEP_INTS(                             \
        NAME##Impl,                                                      \
        FNG_ENUM_DETAILS_GET_INT_TYPE(__VA_ARGS__),                      \
        FNG_ENUM_DETAILS_GET_VARS(__VA_ARGS__))};                        \
  }                                                                      \
                                                                         \
  FNG_ENUM_MAYBE_UNUSED FNG_ENUM_NODISCARD constexpr size_t getPosition( \
      NAME##Impl value) {                                                \
    switch (value) {                                                     \
      BOOST_PP_SEQ_FOR_EACH_I(                                           \
          FNG_ENUM_DETAILS_OP_POSITION_ICASE,                            \
          NAME##Impl,                                                    \
          FNG_ENUM_DETAILS_TO_SEQ_OF_TUPLES(                             \
              FNG_ENUM_DETAILS_GET_VARS(__VA_ARGS__)))                   \
    }                                                                    \
    FNG_FATAL(                                                           \
        BOOST_PP_STRINGIZE(NAME) " does contain invalid value: {}",      \
        FNG_ENUM_DETAILS_GET_INT_TYPE(__VA_ARGS__)(value));              \
  }                                                                      \
                                                                         \
  FNG_ENUM_MAYBE_UNUSED FNG_ENUM_NODISCARD inline std::string_view       \
  getTypeName(NAME##Impl) {                                              \
    return BOOST_PP_STRINGIZE(NAME);                                     \
  }                                                                      \
  }  // namespace enum_wrapper_
