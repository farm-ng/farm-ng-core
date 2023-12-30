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

#include "farm_ng/core/enum/enum_without_iostream.h"

#include <fmt/ostream.h>

#include <istream>
#include <ostream>

/// Adds ostream overloads for the enum.
#define FARM_ENUM_IOSTREAM_OVERLOAD(EnumName)                                     \
  namespace enum_wrapper_ {                                                       \
  inline auto operator<<(std::ostream &os, EnumName##Impl value)                  \
      -> std::ostream & {                                                         \
    os << toPretty(value);                                                        \
    return os;                                                                    \
  }                                                                               \
  inline auto operator>>(std::istream &is, EnumName##Impl &value)                 \
      -> std::istream & {                                                         \
    std::string str;                                                              \
    is >> str;                                                                    \
    if (!trySetFromString(value, str)) {                                          \
      throw std::runtime_error(std::string("Bad Value: ") + str);                 \
    }                                                                             \
    return is;                                                                    \
  }                                                                               \
                                                                                  \
  auto inline format_as(EnumName##Impl value) { return toPretty(value); }         \
  }  // namespace enum_wrapper_

/// Convenience marco which defines the enum plus alias and adds the ostream
/// overload.
#define FARM_ENUM(EnumName, ...)                     \
  FARM_ENUM_WITHOUT_IOSTREAM(EnumName, __VA_ARGS__); \
  FARM_ENUM_IOSTREAM_OVERLOAD(EnumName)

/// Convenience marco which defines the enum and adds the ostream overload.
#define FARM_ENUM_DEF(EnumName, ...)                     \
  FARM_ENUM_WITHOUT_IOSTREAM_DEF(EnumName, __VA_ARGS__); \
  FARM_ENUM_IOSTREAM_OVERLOAD(EnumName)
