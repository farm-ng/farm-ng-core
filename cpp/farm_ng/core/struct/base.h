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
#define FARM_STRUCT_DETAILS_FIELD_DECLARATION(Dummy1_, Dummy2, Type_Name_Init) \
  FARM_PP_TUPLE_ELEM(0, Type_Name_Init)                                        \
  FARM_PP_TUPLE_ELEM(1, Type_Name_Init) FARM_PP_TUPLE_ELEM(2, Type_Name_Init);

// Given a sequence of triplets, create field declarations for
// each triplet.
//
// Example:
//
// Input:  ((Type0, name0, init0)) ((Type1, name1, init1)) ...
// Output: Type0 name0 {init0}; Type1 name1 {init1}; ...
#define FARM_STRUCT_DETAILS_FIELD_DECLARATION_LOOP(Field_Seq_) \
  FARM_PP_SEQ_FOR_EACH(FARM_STRUCT_DETAILS_FIELD_DECLARATION, _, Field_Seq_)

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
#define FARM_STRUCT_DETAILS_FIELD_NAME(Dummy1_, Dummy2, Type_Name_Init) \
  , FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(1, Type_Name_Init))

// Given a sequence of triplets, create a comma separated list of field name
// strings.
//
// Example:
//
// Input:  ((Type0, name0, init0)) ((Type1, name1, init1)) ... Output: ,
// Output: name0, name1 ...
#define FARM_STRUCT_DETAILS_FIELD_NAME_LOOP(Field_Seq_)                     \
  FARM_PP_STRINGIZE(FARM_PP_TUPLE_ELEM(1, FARM_PP_SEQ_ELEM(0, Field_Seq_))) \
  FARM_PP_SEQ_FOR_EACH(                                                     \
      FARM_STRUCT_DETAILS_FIELD_NAME, _, FARM_PP_SEQ_POP_FRONT(Field_Seq_))

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
#define FARM_STRUCT_DETAILS_FIELD_TYPE(Dummy1_, Dummy2, Type_Name_Init) \
  , FARM_PP_TUPLE_ELEM(0, Type_Name_Init)

// Given a sequence of triplets, create a comma separated list of field types.
//
// Example:
//
// Input: ((Type0, name0, init0)) ((Type1, name1, init1)) ...
// Output: , Type0 , Type1 ...
#define FARM_STRUCT_DETAILS_FIELD_TYPE_LOOP(Field_Seq_)  \
  FARM_PP_TUPLE_ELEM(0, FARM_PP_SEQ_ELEM(0, Field_Seq_)) \
  FARM_PP_SEQ_FOR_EACH(                                  \
      FARM_STRUCT_DETAILS_FIELD_TYPE, _, FARM_PP_SEQ_POP_FRONT(Field_Seq_))

#define FARM_STRUCT_DETAILS_BASE(Num_Fields_, Field_Seq_)                   \
  static int constexpr kNumFields = Num_Fields_;                            \
  static std::array<std::string_view, kNumFields> constexpr kFieldNames = { \
      FARM_STRUCT_DETAILS_FIELD_NAME_LOOP(Field_Seq_)};                     \
  using FieldTypes =                                                        \
      std::tuple<FARM_STRUCT_DETAILS_FIELD_TYPE_LOOP(Field_Seq_)>;          \
  FARM_STRUCT_DETAILS_FIELD_DECLARATION_LOOP(Field_Seq_)
