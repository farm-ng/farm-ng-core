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

// NOTE: This file should typically only be included in *.cpp files - to keep
// compile times low.

#include "farm_ng/core/proto_conv/std/conv.h"

#include <farm_pp/preprocessor/tuple/elem.hpp>

#include <optional>

namespace farm_ng {

#define FARM_CONV_IMPL_PRIMITIVE_OPTIONAL(Proto_Type_, Cpp_Type_)          \
  template <>                                                              \
  auto fromProt<Proto_Type_>(Proto_Type_ const& proto)                     \
      -> Expected<std::optional<Cpp_Type_>> {                              \
    std::optional<Cpp_Type_> v;                                            \
    if (proto.has_value()) {                                               \
      v = proto.value();                                                   \
    } else {                                                               \
      v = std::nullopt;                                                    \
    }                                                                      \
    return v;                                                              \
  }                                                                        \
                                                                           \
  template <>                                                              \
  auto toProt<std::optional<Cpp_Type_>>(std::optional<Cpp_Type_> const& v) \
      -> Proto_Type_ {                                                     \
    Proto_Type_ proto;                                                     \
    if (v.has_value()) {                                                   \
      proto.set_value(v.value());                                          \
      proto.set_has_value(true);                                           \
    } else {                                                               \
      proto.set_has_value(false);                                          \
    }                                                                      \
    return proto;                                                          \
  }

}  // namespace farm_ng
