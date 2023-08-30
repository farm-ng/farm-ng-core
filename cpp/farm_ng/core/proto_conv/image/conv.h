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
#include "sophus/image/dyn_image_types.h"

namespace farm_ng::core {

sophus::ImageSize fromProto(proto::ImageSize const& proto);
proto::ImageSize toProto(sophus::ImageSize const& image_size);

sophus::ImageLayout fromProto(proto::ImageLayout const& proto);
proto::ImageLayout toProto(sophus::ImageLayout const& layout);

Expected<sophus::PixelFormat> fromProto(proto::PixelFormat const& proto);
proto::PixelFormat toProto(sophus::PixelFormat const& layout);

Expected<sophus::AnyImage<>> fromProto(proto::DynImage const& proto);
Expected<sophus::IntensityImage<>> intensityImageFromProto(
    proto::DynImage const& proto);

template <class TPredicate>
proto::DynImage toProto(sophus::DynImage<TPredicate> const& image) {
  proto::DynImage proto;
  *proto.mutable_layout() = toProto(image.layout());
  *proto.mutable_pixel_format() = toProto(image.pixelFormat());
  proto.set_data(
      std::string(image.rawPtr(), image.rawPtr() + image.sizeBytes()));

  SOPHUS_ASSERT_EQ(size_t(image.layout().sizeBytes()), proto.data().size());

  return proto;
}
}  // namespace farm_ng::core
