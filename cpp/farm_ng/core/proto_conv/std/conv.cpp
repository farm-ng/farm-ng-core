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
#include "farm_ng/core/proto_conv/std/conv.h"

#include "farm_ng/core/proto_conv/lie/conv.h"
#include "farm_ng/core/proto_conv/std/optional_primitive_impl_macro.ipp"
#include "farm_ng/core/proto_conv/std/repeated_primitive_impl_macro.ipp"

namespace farm_ng {

FARM_CONV_IMPL_PRIMITIVE_OPTIONAL(core::proto::OptionalG0Float, float);
FARM_CONV_IMPL_PRIMITIVE_OPTIONAL(core::proto::OptionalG0Double, double);
FARM_CONV_IMPL_PRIMITIVE_OPTIONAL(core::proto::OptionalG0Int32, int32_t);
FARM_CONV_IMPL_PRIMITIVE_OPTIONAL(core::proto::OptionalG0Int64, int64_t);
FARM_CONV_IMPL_PRIMITIVE_OPTIONAL(core::proto::OptionalG0String, std::string);
FARM_CONV_IMPL_PRIMITIVE_OPTIONAL(core::proto::OptionalG0Bool, bool);

FARM_CONV_IMPL_REPEATED_PRIMITIVE(
    core::proto::RepeatedG0Float, std::vector<float>);

template <>
auto fromProt<core::proto::FileSystemPath>(
    core::proto::FileSystemPath const& proto)
    -> Expected<std::filesystem::path> {
  std::filesystem::path s;
  s = proto.path_string();
  return s;
}

template <>
auto toProt<std::filesystem::path>(std::filesystem::path const& path)
    -> core::proto::FileSystemPath {
  core::proto::FileSystemPath proto;
  proto.set_path_string(path.string());
  return proto;
}

}  // namespace farm_ng
