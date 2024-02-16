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

#include "farm_ng/core/proto_conv/calculus/conv.h"

#include "farm_ng/core/logging/fmt_eigen.h"
#include "farm_ng/core/proto_conv/linalg/conv.h"
#include "farm_ng/core/proto_conv/std/repeated_message_impl_macro.ipp"

namespace farm_ng {

template <>
auto fromProt<core::proto::RegionI32>(core::proto::RegionI32 const& proto)
    -> Expected<sophus::RegionI> {
  if (proto.is_empty()) {
    return sophus::RegionI::empty();
  }
  if (!(proto.min() <= proto.max())) {
    return FARM_UNEXPECTED(
        "min {} must be <= max {}", proto.min(), proto.max());
  }

  return sophus::RegionI::fromMinMax(proto.min(), proto.max());
}

template <>
auto toProt<sophus::RegionI>(sophus::RegionI const& region)
    -> core::proto::RegionI32 {
  core::proto::RegionI32 proto;
  if (region.isEmpty()) {
    proto.set_is_empty(true);
  } else {
    proto.set_is_empty(false);
    proto.set_min(region.min());
    proto.set_max(region.max());
  }
  return proto;
}

template <>
auto fromProt<core::proto::RegionF32>(core::proto::RegionF32 const& proto)
    -> Expected<sophus::RegionF32> {
  if (proto.is_empty()) {
    return sophus::RegionF32::empty();
  }
  if (!(proto.min() <= proto.max())) {
    return FARM_UNEXPECTED(
        "min {} must be <= max {}", proto.min(), proto.max());
  }

  return sophus::RegionF32::fromMinMax(proto.min(), proto.max());
}
template <>
auto toProt<sophus::RegionF32>(sophus::RegionF32 const& region)
    -> core::proto::RegionF32 {
  core::proto::RegionF32 proto;
  if (region.isEmpty()) {
    proto.set_is_empty(true);
  } else {
    proto.set_is_empty(false);
    proto.set_min(region.min());
    proto.set_max(region.max());
  }
  return proto;
}

template <>
auto fromProt<core::proto::RegionF64>(core::proto::RegionF64 const& proto)
    -> Expected<sophus::RegionF64> {
  if (proto.is_empty()) {
    return sophus::RegionF64::empty();
  }
  if (!(proto.min() <= proto.max())) {
    return FARM_UNEXPECTED(
        "min {} must be <= max {}", proto.min(), proto.max());
  }

  return sophus::RegionF64::fromMinMax(proto.min(), proto.max());
}

template <>
auto toProt<sophus::RegionF64>(sophus::RegionF64 const& region)
    -> core::proto::RegionF64 {
  core::proto::RegionF64 proto;
  if (region.isEmpty()) {
    proto.set_is_empty(true);
  } else {
    proto.set_is_empty(false);
    proto.set_min(region.min());
    proto.set_max(region.max());
  }
  return proto;
}

template <>
auto fromProt<core::proto::Region2F64>(core::proto::Region2F64 const& proto)
    -> Expected<sophus::Region2F64> {
  if (proto.is_empty()) {
    return sophus::Region2F64::empty();
  }
  FARM_TRY(auto, mi, fromProt(proto.min()));
  FARM_TRY(auto, ma, fromProt(proto.max()));
  if (!(mi.array() <= ma.array()).all()) {
    return FARM_UNEXPECTED(
        "min ({}) must be <= max ({})", mi.transpose(), ma.transpose());
  }
  return sophus::Region2F64::fromMinMax(mi, ma);
}

template <>
auto toProt<sophus::Region2F64>(sophus::Region2F64 const& region)
    -> core::proto::Region2F64 {
  core::proto::Region2F64 proto;
  if (region.isEmpty()) {
    proto.set_is_empty(true);
  } else {
    proto.set_is_empty(false);
    *proto.mutable_min() = toProt(region.min().matrix().eval());
    *proto.mutable_max() = toProt(region.max().matrix().eval());
  }
  return proto;
}

FARM_CONV_IMPL_REPEATED_MESSAGE(
    core::proto::RepeatedG0Region2F64, std::deque<sophus::Region2F64>)

}  // namespace farm_ng
