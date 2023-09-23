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

#include "farm_ng/core/proto_conv/traits.h"
#include "farm_ng/core/std.pb.h"

#include <filesystem>
#include <optional>

namespace farm_ng {

FARM_PROTO_CONV_TRAIT(std::optional<float>, core::proto::OptionalG0Float);
FARM_PROTO_CONV_TRAIT(std::optional<double>, core::proto::OptionalG0Double);
FARM_PROTO_CONV_TRAIT(std::optional<int32_t>, core::proto::OptionalG0Int32);
FARM_PROTO_CONV_TRAIT(std::optional<int64_t>, core::proto::OptionalG0Int64);
FARM_PROTO_CONV_TRAIT(
    std::optional<std::string>, core::proto::OptionalG0String);
FARM_PROTO_CONV_TRAIT(std::optional<bool>, core::proto::OptionalG0Bool);

FARM_PROTO_CONV_TRAIT(std::filesystem::path, core::proto::FileSystemPath);

}  // namespace farm_ng
