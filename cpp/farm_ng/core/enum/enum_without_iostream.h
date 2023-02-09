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

// FARM_ENUM is a fork of MY_ENUM.
//
// https://github.com/facebookincubator/MY_ENUM/
//
// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include "farm_ng/core/enum/impl/enum_details.h"

/* FARM_ENUM_WITHOUT_IOSTREAM_DEF(NAME, [INT_TYPE], ARGS ...)

  Enum type definition. Can be in global, and namespace scope only. See
  FARM_ENUM_WITHOUT_IOSTREAM below for details. */
#define FARM_ENUM_WITHOUT_IOSTREAM_DEF(NAME, ...) \
  FARM_ENUM_DEF_IMPL(NAME, __VA_ARGS__)

/* FARM_ENUM_ALIAS(NAME)

  Enum type alias. Can be in any scope. See FARM_ENUM_WITHOUT_IOSTREAM below for
  details. */
#define FARM_ENUM_ALIAS(NAME) using NAME = enum_wrapper_::NAME##Impl

/* FARM_ENUM_WITHOUT_IOSTREAM(NAME, [INT_TYPE], ARGS ...)
  Enum type. Can be in global, and namespace scope only.

      NAME:     Name of the enum class.
      INT_TYPE: integral type, optional argument which defaults to ``int``.
      ARGS:     Tuple of enums. Could be just names, such as (foo, bar), or
                also name/value pairs, such as ((one,1), (two,2)).

  For example ``FARM_ENUM_WITHOUT_IOSTREAM(FooBar, int, (foo, bar));`` defines
  an enum class

    enum class FooBar : int {
      foo,
      bar
    };

  with the following free functions:

    // Returns corresponding string of given value
    //
    // Preconditions: value must be a valid enum value, i.e. FooBar::foo, or
    //                FooBar::bar.
    //
    std::string toString(FooBar value);

    // Returns corresponding string view of given value
    //
    // Preconditions: value must be a valid enum value, i.e. FooBar::foo, or
    //                FooBar::bar.
    //
    string_view toStringView(FooBar value);

    // Returns pretty string representation of given value
    //
    // Preconditions: value must be a valid enum value, i.e. FooBar::foo, or
    //                FooBar::bar.
    //
    std::string toPretty(FooBar value);

    // Sets enum given corresponding string, if string is valid (i.e. "foo" or
    // "bar"). Returns false otherwise.
    //
    bool trySetFromString(FooBar& value, std::string str);

    // Return count of enum type. First argument is needed for ADL only.
    //
    constexpr size_t getCount(FooBar) {
      return 2;
    }

    // Return string views of enum type. First argument is needed for ADL only.
    std::array<string_view, 2> getStrings(FooBar) {
      return {"foo", "bar"};
    }

    // Return string of enum names. First argument is needed for ADL only.
    std::array<string_view, 2> getStrings(FooBar) {
      return "foo, bar";
    }

    // Return values of enum type. First argument is needed for ADL only.
    constexpr std::array<int, 2> getValues(FooBar) {
      return {0, 1};
    }

    // Returns the position of enum value in the enum class. This is the inverse
    // of `getValues(FooBar)[i]`.
    constexpr size_t getPosition(FooBar value) {
      switch(value) {
        case FooBar::foo: { return 0; }
        case FooBar::bar: { return 1; }
      }
    }

    // Return string representation of type name. First argument is needed for
    // ADL only.
    string_view getTypeName(FooBar) {
      return "FooBar";
    }
*/
#define FARM_ENUM_WITHOUT_IOSTREAM(NAME, ...)       \
  FARM_ENUM_WITHOUT_IOSTREAM_DEF(NAME, __VA_ARGS__) \
  FARM_ENUM_ALIAS(NAME)
