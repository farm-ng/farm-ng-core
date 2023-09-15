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

#include "farm_ng/core/struct/macro_lib.h"

#include <farm_pp/preprocessor/comparison/equal.hpp>
#include <farm_pp/preprocessor/control/if.hpp>
#include <farm_pp/preprocessor/punctuation/remove_parens.hpp>
#include <farm_pp/preprocessor/seq/pop_front.hpp>
#include <farm_pp/preprocessor/tuple/elem.hpp>

// Given a triplet such as (Type, name, init), return a field declaration.
//
// ${triplet}
//
// => ${triplet}[0] ${triplet}[1] ${triplet}[2]
//
// Example:
//
// Input:  (Type0, name0, {init0})
// Output: Type name {init};
//
#define FARM_STRUCT_DETAILS_FIELD_DECLARATION(dummy1, dummy2, triplet) \
  FARM_PP_TUPLE_ELEM(0, triplet)                                       \
  FARM_PP_TUPLE_ELEM(1, triplet) FARM_PP_TUPLE_ELEM(2, triplet);

// Given a sequence of triplets, create field declarations for
// each triplet.
//
// Example:
//
// Input:  ((Type0, name0, init0)) ((Type1, name1, init1)) ...
// Output: Type0 name0 {init0}; Type1 name1 {init1}; ...
#define FARM_STRUCT_DETAILS_FIELD_DECLARATION_LOOP(field_seq) \
  FARM_PP_SEQ_FOR_EACH(FARM_STRUCT_DETAILS_FIELD_DECLARATION, _, field_seq)

// Given a triplet such as (Type, name, init), return the name as a string
// leading with a comma.
//
// ${triplet}
//
// => , "${triplet}[1]"
//
// Example:
//
// Input:  (Type0, name0, {init0})
// Output: , "name0"
#define FARM_STRUCT_DETAILS_FIELD_NAME(dummy1, dummy2, triplet) \
  , FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(1, triplet))

// Given a sequence of triplets, create a comma separated list of field name
// strings.
//
// Example:
//
// Input:  ((Type0, name0, init0)) ((Type1, name1, init1)) ... Output: ,
// Output: name0, name1 ...
#define FARM_STRUCT_DETAILS_FIELD_NAME_LOOP(field_seq)                     \
  FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(1, FARM_PP_SEQ_ELEM(0, field_seq))) \
  FARM_PP_SEQ_FOR_EACH(                                                    \
      FARM_STRUCT_DETAILS_FIELD_NAME, _, FARM_PP_SEQ_POP_FRONT(field_seq))

// Given a triplet such as (Type, name, init), return the type.
//
// ${triplet}
//
// => ${triplet}[0]
//
// Example:
//
// Input:  (Type0, name0, {init0})
// Output: , Type
#define FARM_STRUCT_DETAILS_FIELD_TYPE(dummy1, dummy2, TRIPLET) \
  , FARM_PP_TUPLE_ELEM(0, TRIPLET)

// Given a sequence of triplets, create a comma separated list of field types.
//
// Example:
//
// Input: ((Type0, name0, init0)) ((Type1, name1, init1)) ...
// Output: , Type0 , Type1 ...
#define FARM_STRUCT_DETAILS_FIELD_TYPE_LOOP(field_seq)  \
  FARM_PP_TUPLE_ELEM(0, FARM_PP_SEQ_ELEM(0, field_seq)) \
  FARM_PP_SEQ_FOR_EACH(                                 \
      FARM_STRUCT_DETAILS_FIELD_TYPE, _, FARM_PP_SEQ_POP_FRONT(field_seq))

#define FARM_STRUCT_DETAILS_BASE(StructName, num_fields, field_seq)         \
  static int constexpr kNumFields = num_fields;                             \
  static std::array<std::string_view, kNumFields> constexpr kFieldNames = { \
      FARM_STRUCT_DETAILS_FIELD_NAME_LOOP(field_seq)};                      \
  using FieldTypes =                                                        \
      std::tuple<FARM_STRUCT_DETAILS_FIELD_TYPE_LOOP(field_seq)>;           \
  FARM_STRUCT_DETAILS_FIELD_DECLARATION_LOOP(field_seq)
