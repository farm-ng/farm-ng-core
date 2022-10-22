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

#include "farm_ng/core/enum/enum_without_ostream.h"

#include <ostream>

#define FARM_ENUM_OSTREAM_OVERLOAD(EnumName)                                \
  namespace enum_wrapper_ {                                                 \
  inline std::ostream &operator<<(std::ostream &os, EnumName##Impl value) { \
    os << toPretty(value);                                                  \
    return os;                                                              \
  }                                                                         \
  }  // namespace enum_wrapper_

// Convenience marco which defines the enum plus alias and adds the ostream
// overload.
#define FARM_ENUM(EnumName, ...)                    \
  FARM_ENUM_WITHOUT_OSTREAM(EnumName, __VA_ARGS__); \
  FARM_ENUM_OSTREAM_OVERLOAD(EnumName)

// Convenience marco which defines the enum and adds the ostream overload.
#define FARM_ENUM_DEF(EnumName, ...)                    \
  FARM_ENUM_WITHOUT_OSTREAM_DEF(EnumName, __VA_ARGS__); \
  FARM_ENUM_OSTREAM_OVERLOAD(EnumName)
