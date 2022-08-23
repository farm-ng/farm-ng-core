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

#include "fng_core/enum/enum_flags_without_ostream.h"

#include <ostream>

#define FNG_ENUMFLAGS_OSTREAM_OVERLOAD(NAME)                            \
  namespace enum_wrapper_ {                                             \
  inline std::ostream &operator<<(std::ostream &os, NAME##Impl value) { \
    os << toPretty(value);                                              \
    return os;                                                          \
  }                                                                     \
  }  // namespace enum_wrapper_

// Convenience marco which defines the enum flags plus alias and adds the
// ostream overload.
#define FNG_ENUMFLAGS(EnumName, UINT_TYPE, ...)                    \
  FNG_ENUMFLAGS_WITHOUT_OSTREAM(EnumName, UINT_TYPE, __VA_ARGS__); \
  FNG_ENUMFLAGS_OSTREAM_OVERLOAD(EnumName)

// Convenience marco which defines the enum flags and adds the ostream overload.
#define FNG_ENUMFLAGS_DEF(EnumName, UINT_TYPE, ...)                    \
  FNG_ENUMFLAGS_WITHOUT_OSTREAM_DEF(EnumName, UINT_TYPE, __VA_ARGS__); \
  FNG_ENUMFLAGS_OSTREAM_OVERLOAD(EnumName)
