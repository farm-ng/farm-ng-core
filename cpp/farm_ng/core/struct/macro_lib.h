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

#include <farm_pp/preprocessor/punctuation/remove_parens.hpp>
#include <farm_pp/preprocessor/seq/for_each.hpp>
#include <farm_pp/preprocessor/seq/for_each_i.hpp>
#include <farm_pp/preprocessor/tuple/to_seq.hpp>

// Given an Entity, which might be a tuple, return a sequence containing a
// single tuple.
//
// Examples:
//
// Input:  (A,2)
// Output: ((A,2))
//
// Input:  A
// Output: ((A))
//
// Input:  (A)
// Output: (((A))
#define FARM_MACROLIB_DETAILS_MAKE_TUPLE(D1_, D2_, Entity_) \
  ((FARM_PP_REMOVE_PARENS(Entity_)))

// Given a tuple of entities, which might be tuples, return a sequence of
// tuples.
//
// Examples:
//
// Input:  (A, (B,2) , C)
// Output: ((A))((B,2))((C))
#define FARM_MACROLIB_SEQ_OF_TUPLES_FROM_TUPLE(Tuple_Of_Entities_) \
  FARM_PP_SEQ_FOR_EACH(                                            \
      FARM_MACROLIB_DETAILS_MAKE_TUPLE,                            \
      _,                                                           \
      FARM_PP_TUPLE_TO_SEQ(Tuple_Of_Entities_))
