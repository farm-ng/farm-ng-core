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
#include <type_traits>

// Output: , ${FLAG} = (1 << ($I + 1))
#define FARM_ENUMFLAGS_DETAILS_OP_INIT_FLAG(dummy1, dummy2, I, FLAG) \
  , FLAG = (1 << (I + 1))

// Input:  (A, B, C, D)
// Output: A=1, B=(1<<1), C=(1<<2), D=(1<<3)
#define FARM_ENUMFLAGS_DETAILS_FLAG_DEFINITIONS(...)             \
  FARM_PP_TUPLE_ELEM(0, __VA_ARGS__) = 1 FARM_PP_SEQ_FOR_EACH_I( \
      FARM_ENUMFLAGS_DETAILS_OP_INIT_FLAG,                       \
      _,                                                         \
      FARM_PP_SEQ_POP_FRONT(FARM_PP_TUPLE_TO_SEQ(__VA_ARGS__)))

// Output:
//   if (str == "${FLAGS}") {
//     value = ${TYPE}:${FLAG};
//     return true;
//   }
#define FARM_ENUMFLAG_DETAILS_OP_SET_ENUM_FROM_STRING(dummy1, TYPE, FLAG) \
  if (str == FARM_PP_STRINGIZE(FLAG)) {                                   \
    value = value | TYPE::FLAG;                                           \
    return true;                                                          \
  }

// Output:
//   case ${TYPE}::$FLAG}: {
//     static_assert(
//         std::popcount(uint64_t(${TYPE}::${FLAG})) == 1,
//         "${FLAG} must be a single bit flag");
//     return true;
//   }
#if (defined __linux__ || defined __APPLE__)
#define FARM_ENUMFLAG_DETAILS_OP_SINGLE_FLAG_CHECK(dummy1, TYPE, FLAG) \
  case TYPE::FLAG: {                                                   \
    static_assert(                                                     \
        __builtin_popcount(uint64_t(TYPE::FLAG)) == 1,                 \
        FARM_PP_STRINGIZE(FLAG) " must be a single bit flag");         \
    return true;                                                       \
  }
#else
#define FARM_ENUMFLAG_DETAILS_OP_SINGLE_FLAG_CHECK(dummy1, TYPE, FLAG) \
  case TYPE::FLAG: {                                                   \
    return true;                                                       \
  }
#endif

// Output:
//   if (hasMask(value, ${TYPE}::${FLAG})) {
//      strings.push_back("${FLAG}");
//   }
#define FARM_ENUMFLAG_DETAILS_OP_PUSHBACK_STRING(dummy1, TYPE, FLAG) \
  if (hasMask(value, TYPE::FLAG)) {                                  \
    strings.push_back(FARM_PP_STRINGIZE(FLAG));                      \
  }

// Output:
//   if (hasMask(value, ${TYPE}::${FLAG})) {
//      prettySet += "${FLAG}, ";
//   }
#define FARM_ENUMFLAG_DETAILS_OP_STRING_FOR_FLAG(dummy1, TYPE, FLAG)           \
  if (hasMask(value, TYPE::FLAG)) {                                            \
    prettySet +=                                                               \
        FARM_PP_STRINGIZE(FLAG) " (=" + std::to_string(uint64_t(TYPE::FLAG)) + \
        "), ";                                                                 \
  }

// Output: ", ${STRING}"
#define FARM_ENUMFLAGS_DETAILS_OP_COMMA_STRING(dummy1, dummy2, STRING) \
  , FARM_PP_STRINGIZE(STRING)

// Input: (A, B, C)
// Output: "A", "B", "C"
#define FARM_ENUMFLAGS_DETAILS_COMMA_SEP_STRINGS(...)   \
  FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(0, __VA_ARGS__)) \
  FARM_PP_SEQ_FOR_EACH(                                 \
      FARM_ENUMFLAGS_DETAILS_OP_COMMA_STRING,           \
      _,                                                \
      FARM_PP_SEQ_POP_FRONT(FARM_PP_TUPLE_TO_SEQ(__VA_ARGS__)))

// Output: ", ${STRING}"
#define FARM_ENUMFLAGS_DETAILS_OP_COMMA_VALUE_STRING(dummy1, dummy2, STRING) \
  ", " FARM_PP_STRINGIZE(STRING)

// Input: (A, B, C)
// Output: "A, B, C"
#define FARM_ENUMFLAGS_DETAILS_CSV_STRING(...)          \
  FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(0, __VA_ARGS__)) \
  FARM_PP_SEQ_FOR_EACH(                                 \
      FARM_ENUMFLAGS_DETAILS_OP_COMMA_VALUE_STRING,     \
      _,                                                \
      FARM_PP_SEQ_POP_FRONT(FARM_PP_TUPLE_TO_SEQ(__VA_ARGS__)))

// Output: , uint64_t(${TYPE}::${VAL})
#define FARM_ENUMFLAGS_DETAILS_OP_COMMA_INT(dummy1, TYPE, VAL) \
  , uint64_t(TYPE::VAL)

// Input:  (A, B, C)
// Output: 1, 2, 4
//
// Note: Here, "1, 2, 4" are the values of "A, B, C".
#define FARM_ENUMFLAGS_DETAILS_COMMA_SEP_INTS(TYPE, ...)                   \
  uint64_t(TYPE::FARM_PP_TUPLE_ELEM(0, __VA_ARGS__)) FARM_PP_SEQ_FOR_EACH( \
      FARM_ENUMFLAGS_DETAILS_OP_COMMA_INT,                                 \
      TYPE,                                                                \
      FARM_PP_SEQ_POP_FRONT(FARM_PP_TUPLE_TO_SEQ(__VA_ARGS__)))

#define FARM_ENUMFLAGS_DEF_IMPL(NAME, UINT_TYPE, ...)                          \
  namespace enum_wrapper_ {                                                    \
                                                                               \
  static_assert(                                                               \
      std::is_unsigned<UINT_TYPE>::value,                                      \
      FARM_PP_STRINGIZE(UINT_TYPE) " must be an unsigned integer type");       \
                                                                               \
  enum class NAME##Impl : UINT_TYPE{                                           \
      none = 0u, FARM_ENUMFLAGS_DETAILS_FLAG_DEFINITIONS(__VA_ARGS__)};        \
                                                                               \
  NAME##Impl inline operator|(NAME##Impl left, NAME##Impl right) {             \
    return NAME##Impl(UINT_TYPE(left) | UINT_TYPE(right));                     \
  }                                                                            \
                                                                               \
  NAME##Impl inline operator&(NAME##Impl left, NAME##Impl right) {             \
    return NAME##Impl(UINT_TYPE(left) & UINT_TYPE(right));                     \
  }                                                                            \
                                                                               \
  [[maybe_unused]] inline bool trySetFlagFromString(                           \
      NAME##Impl &value, std::string const &str) {                             \
    FARM_PP_SEQ_FOR_EACH(                                                      \
        FARM_ENUMFLAG_DETAILS_OP_SET_ENUM_FROM_STRING,                         \
        NAME##Impl,                                                            \
        FARM_PP_TUPLE_TO_SEQ(__VA_ARGS__))                                     \
    return false;                                                              \
  }                                                                            \
                                                                               \
  [[maybe_unused]] inline bool hasMask(NAME##Impl value, NAME##Impl mask) {    \
    return (value & mask) == mask;                                             \
  }                                                                            \
                                                                               \
  [[maybe_unused]] inline void setMask(NAME##Impl &value, NAME##Impl mask) {   \
    value = value | mask;                                                      \
  }                                                                            \
                                                                               \
  [[maybe_unused]] inline void clearMask(NAME##Impl &value, NAME##Impl mask) { \
    value = value & NAME##Impl(~UINT_TYPE(mask));                              \
  }                                                                            \
                                                                               \
  [[maybe_unused]] inline void toggleMask(                                     \
      NAME##Impl &value, NAME##Impl mask) {                                    \
    value = NAME##Impl(UINT_TYPE(value) ^ UINT_TYPE(mask));                    \
  }                                                                            \
                                                                               \
  [[maybe_unused]] inline bool isSingleFlag(NAME##Impl value) {                \
    switch (value) {                                                           \
      FARM_PP_SEQ_FOR_EACH(                                                    \
          FARM_ENUMFLAG_DETAILS_OP_SINGLE_FLAG_CHECK,                          \
          NAME##Impl,                                                          \
          FARM_PP_TUPLE_TO_SEQ(__VA_ARGS__))                                   \
      case NAME##Impl::none:                                                   \
        break;                                                                 \
    }                                                                          \
    return false;                                                              \
  }                                                                            \
                                                                               \
  [[maybe_unused]] inline std::vector<std::string> toStrings(                  \
      NAME##Impl value) {                                                      \
    std::vector<std::string> strings;                                          \
    FARM_PP_SEQ_FOR_EACH(                                                      \
        FARM_ENUMFLAG_DETAILS_OP_PUSHBACK_STRING,                              \
        NAME##Impl,                                                            \
        FARM_PP_TUPLE_TO_SEQ(__VA_ARGS__))                                     \
    return strings;                                                            \
  }                                                                            \
                                                                               \
  [[maybe_unused]] inline std::string toPretty(NAME##Impl value) {             \
    std::string prettySet("{");                                                \
    FARM_PP_SEQ_FOR_EACH(                                                      \
        FARM_ENUMFLAG_DETAILS_OP_STRING_FOR_FLAG,                              \
        NAME##Impl,                                                            \
        FARM_PP_TUPLE_TO_SEQ(__VA_ARGS__))                                     \
    prettySet += "} (=" + std::to_string(UINT_TYPE(value)) + ")";              \
    return prettySet;                                                          \
  }                                                                            \
                                                                               \
  [[maybe_unused]] [[nodiscard]] constexpr size_t getCount(NAME##Impl) {       \
    return FARM_PP_TUPLE_SIZE(__VA_ARGS__);                                    \
  }                                                                            \
                                                                               \
  [[maybe_unused]] [[nodiscard]] inline std::                                  \
      array<std::string_view, FARM_PP_TUPLE_SIZE(__VA_ARGS__)>                 \
      getNames(NAME##Impl) {                                                   \
    return {FARM_ENUMFLAGS_DETAILS_COMMA_SEP_STRINGS(__VA_ARGS__)};            \
  }                                                                            \
                                                                               \
  [[maybe_unused]] [[nodiscard]] inline std::string_view getStringOfNames(     \
      NAME##Impl) {                                                            \
    return FARM_ENUMFLAGS_DETAILS_CSV_STRING(__VA_ARGS__);                     \
  }                                                                            \
                                                                               \
  [[maybe_unused]] [[nodiscard]] constexpr std::                               \
      array<UINT_TYPE, FARM_PP_TUPLE_SIZE(__VA_ARGS__)>                        \
      getValues(NAME##Impl) {                                                  \
    return {FARM_ENUMFLAGS_DETAILS_COMMA_SEP_INTS(NAME##Impl, __VA_ARGS__)};   \
  }                                                                            \
                                                                               \
  [[maybe_unused]] [[nodiscard]] inline std::string_view getTypeName(          \
      NAME##Impl) {                                                            \
    return FARM_PP_STRINGIZE(NAME);                                            \
  }                                                                            \
  }  // namespace enum_wrapper_
