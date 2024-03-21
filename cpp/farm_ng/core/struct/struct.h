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

#include "farm_ng/core/struct/base.h"
#include "farm_ng/core/struct/macro_lib.h"

/// Takes in a struct name and a sequence of fields and generates a struct.
///
/// This is an implementation details. User shall call FARM_STRUCT(Struct_Name_,
/// ((Type0, name0, init0), ...))); instead;
///
/// This intermediate macro exists since the conversion from a list to a
/// sequence is likely somewhat expensive, so we want to do it once and reuse
/// it.
#define FARM_STRUCT_DETAIL_FROM_SEQ(Struct_Name_, Num_Fields_, Field_Seq_) \
  struct Struct_Name_ {                                                    \
    FARM_STRUCT_DETAILS_BASE(Struct_Name_, Num_Fields_, Field_Seq_)        \
  }

/// Takes in a struct name and a tuple of fields and generates a struct.
#define FARM_STRUCT(Struct_Name_, Tuple_Of_Fields_) \
  FARM_STRUCT_DETAIL_FROM_SEQ(                      \
      Struct_Name_,                                 \
      FARM_PP_TUPLE_SIZE(Tuple_Of_Fields_),         \
      FARM_MACROLIB_SEQ_OF_TUPLES_FROM_TUPLE(Tuple_Of_Fields_))
