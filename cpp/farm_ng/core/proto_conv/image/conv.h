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

#include "farm_ng/core/image.pb.h"
#include "farm_ng/core/proto_conv/traits.h"
#include "sophus2/image/dyn_image_types.h"

namespace farm_ng {

FARM_PROTO_CONV_TRAIT(sophus2::ImageSize, core::proto::ImageSize);
FARM_PROTO_CONV_TRAIT(sophus2::ImageLayout, core::proto::ImageLayout);
FARM_PROTO_CONV_TRAIT(sophus2::PixelFormat, core::proto::PixelFormat);
FARM_PROTO_CONV_TRAIT(sophus2::AnyImage<>, core::proto::DynImage);

/// Converts core::proto::DynImage to a sophus2::IntensityImage.
Expected<sophus2::IntensityImage<>> intensityImageFromProto(
    core::proto::DynImage const& proto);

/// Associates a proto type to a sophus2::IntensityImage<>.
template <>
struct ToProtoTrait<sophus2::IntensityImage<>> {
  /// proto type
  using ProtoType = core::proto::DynImage;
};

/// Converts a sophus2::IntensityImage<> to a proto.
template <>
auto toProt<sophus2::IntensityImage<>>(sophus2::IntensityImage<> const& cpp)
    -> core::proto::DynImage;

}  // namespace farm_ng
