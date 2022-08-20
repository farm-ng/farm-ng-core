// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

// FNG_ENUM is a fork of MY_ENUM.
//
// https://github.com/facebookincubator/MY_ENUM/
//
// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <fng_core/enum/impl/enum_flag_details.h>

/* FNG_ENUMFLAGS_WITHOUT_OSTREAM_DEF(NAME, UINT_TYPE, ARGS)

  Enum flags type definition. Can be in global, and namespace scope only. See
  FNG_ENUMFLAGS_WITHOUT_OSTREAM below for details. */
#define FNG_ENUMFLAGS_WITHOUT_OSTREAM_DEF(NAME, UINT_TYPE, ...) \
  FNG_ENUMFLAGS_DEF_IMPL(NAME, UINT_TYPE, __VA_ARGS__)

/* FNG_ENUMFLAGS_ALIAS(NAME)

  Enum type alias. Can be in any scope. See FNG_ENUMFLAGS_WITHOUT_OSTREAM for
  details. */
#define FNG_ENUMFLAGS_ALIAS(NAME) using NAME = enum_wrapper_::NAME##Impl

/* FNG_ENUMFLAGS_WITHOUT_OSTREAM(NAME, UINT_TYPE, ARGS)

  Enum flag type. Can be in global, and namespace scope only.

      NAME:      Name of the enum flag class.
      UINT_TYPE: unsigned integral type.
      ARGS:      Tuple of enums, such as (foo, bar, da).

  For example ``FNG_ENUMFLAGS_WITHOUT_OSTREAM(MyFlags, uint32_t, (foo, bar,
  daz));`` defines an enum class

    enum class MyFlags : uint32_t {
      none = 0,
      foo = 1,
      bar = 2,
      daz = 4
    };

  with the following free functions:

    // Bitwise or
    MyFlags operator|(MyFlags left, MyFlags right);

    // Bitwise and
    MyFlags operator&(MyFlags left, MyFlags right);

    // Sets corresponding bit to 1, if string is single flag (i.e. "foo", "bar"
    // or "daz"). Returns false otherwise.
    bool trySetFlagFromString(MyFlags& value, const std::string& str);

    // Return true if ``value`` contains all flags in ``mask``.
    bool hasMask(MyFlags value, MyFlags mask);

    // Enables bits in mask.
    void setMask(MyFlags& value, MyFlags mask);

    // Clear bits in mask.
    void clearMask(MyFlags& value, MyFlags mask);

    // Toggle bits.
    void toggleMask(MyFlags& value, MyFlags mask);

    // Returns true if value is a single flag (i.e. Foo::foo, Foo::bar or
    // Foo::daz)
    bool isSingleFlag(MyFlags value);

    // Returns string representation of enabled flags
    // (e.g. {"foo", "bar"} for `MyFlags::foo | MyFlags::bar')
    std::vector<std::string> toStrings(MyFlags value);

    // Return pretty string representation
    // (e.g. "{bar (=2), daz (=4)}" for `MyFlags::bar | MyFlags::daz').
    std::string toPretty(MyFlags value);

    // Returns number of flags (i.e. 3).
    constexpr size_t getCount(MyFlags) {
      return 3u;
    }

    // Returns array of names:
    std::array<string_view, 3> getNames(MyFlags) {
      return {"foo", "bar", "daz"};
    }

    // Return string of names:
    constexpr string_view getStringOfNames(MyFlags) {
      return "{foo, bar, daz}";
    }

    // Returns array of values:
    constexpr std::array<uint32_t, 3u> getValues(MyFlags) {
      return {1, 2, 4};
    }

    string_view getTypeName(MyFlags) {
      return "MyFlags";
    }

   // Note: When adding new flags to an exisiting enum, we want to add such flag
   // at the end.

    FNG_ENUMFLAGS_WITHOUT_OSTREAM(MyFlags, uint32_t, (foo, bar));

      generates

    enum class MyFlags : uint32_t {
      none = 0,
      foo = 1,
      bar = 2
    };


    Now adding "daz" to the end, we get


    FNG_ENUMFLAGS_WITHOUT_OSTREAM(MyFlags, uint32_t, (foo, bar, daz));

      which generates

    enum class MyFlags : uint32_t {
      none = 0,
      foo = 1,
      bar = 2,
      daz = 4
    };

    And hence the order of exiting flags (foo, bar) is respected and their
    values do not change.
*/
#define FNG_ENUMFLAGS_WITHOUT_OSTREAM(NAME, UINT_TYPE, ...)       \
  FNG_ENUMFLAGS_WITHOUT_OSTREAM_DEF(NAME, UINT_TYPE, __VA_ARGS__) \
  FNG_ENUMFLAGS_ALIAS(NAME)
