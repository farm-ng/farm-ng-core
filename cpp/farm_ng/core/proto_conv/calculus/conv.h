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

#include "farm_ng/core/calculus.pb.h"
#include "farm_ng/core/proto_conv/proto_conv.h"
#include "sophus/calculus/region.h"

namespace farm_ng {

FARM_PROTO_CONV_TRAIT(sophus::RegionI, core::proto::RegionI32);
FARM_PROTO_CONV_TRAIT(sophus::RegionF32, core::proto::RegionF32);
FARM_PROTO_CONV_TRAIT(sophus::RegionF64, core::proto::RegionF64);

FARM_PROTO_CONV_TRAIT(sophus::Region2F64, core::proto::Region2F64);

}  // namespace farm_ng
