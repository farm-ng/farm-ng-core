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

#include "farm_ng/core/enum/enum_flags_without_ostream.h"

#include <ostream>

#define FARM_ENUMFLAGS_OSTREAM_OVERLOAD(NAME)                \
  namespace enum_wrapper_ {                                  \
  inline auto operator<<(std::ostream &os, NAME##Impl value) \
      -> std::ostream & {                                    \
    os << toPretty(value);                                   \
    return os;                                               \
  }                                                          \
  }  // namespace enum_wrapper_

// Convenience marco which defines the enum flags plus alias and adds the
// ostream overload.
#define FARM_ENUMFLAGS(EnumName, UINT_TYPE, ...)                    \
  FARM_ENUMFLAGS_WITHOUT_OSTREAM(EnumName, UINT_TYPE, __VA_ARGS__); \
  FARM_ENUMFLAGS_OSTREAM_OVERLOAD(EnumName)

// Convenience marco which defines the enum flags and adds the ostream overload.
#define FARM_ENUMFLAGS_DEF(EnumName, UINT_TYPE, ...)                    \
  FARM_ENUMFLAGS_WITHOUT_OSTREAM_DEF(EnumName, UINT_TYPE, __VA_ARGS__); \
  FARM_ENUMFLAGS_OSTREAM_OVERLOAD(EnumName)
