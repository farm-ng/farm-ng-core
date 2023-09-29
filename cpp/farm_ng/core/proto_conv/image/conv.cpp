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

#include "farm_ng/core/proto_conv/image/conv.h"

namespace farm_ng {

template <>
auto fromProt<core::proto::ImageSize>(core::proto::ImageSize const& proto)
    -> Expected<sophus::ImageSize> {
  return sophus::ImageSize(proto.width(), proto.height());
}

template <>
auto toProt<sophus::ImageSize>(sophus::ImageSize const& image_size)
    -> core::proto::ImageSize {
  core::proto::ImageSize proto;
  proto.set_width(image_size.width);
  proto.set_height(image_size.height);
  return proto;
}

template <>
auto fromProt<core::proto::ImageLayout>(core::proto::ImageLayout const& proto)
    -> Expected<sophus::ImageLayout> {
  FARM_TRY(auto, size, fromProt(proto.size()));
  return sophus::ImageLayout(size, proto.pitch_bytes());
}

template <>
auto toProt<sophus::ImageLayout>(sophus::ImageLayout const& layout)
    -> core::proto::ImageLayout {
  core::proto::ImageLayout proto;
  *proto.mutable_size() = toProt(layout.imageSize());
  proto.set_pitch_bytes(layout.pitchBytes());
  return proto;
}

template <>
auto fromProt<core::proto::PixelFormat>(core::proto::PixelFormat const& proto)
    -> Expected<sophus::PixelFormat> {
  sophus::PixelFormat format;
  FARM_TRY_ASSERT(trySetFromString(format.number_type, proto.number_type()));
  format.num_components = proto.num_components();
  format.num_bytes_per_component = proto.num_bytes_per_component();
  return format;
}

template <>
auto toProt<sophus::PixelFormat>(sophus::PixelFormat const& layout)
    -> core::proto::PixelFormat {
  core::proto::PixelFormat proto;
  proto.set_number_type(toString(layout.number_type));
  proto.set_num_components(layout.num_components);
  proto.set_num_bytes_per_component(layout.num_bytes_per_component);
  return proto;
}

template <>
auto fromProt<core::proto::DynImage>(core::proto::DynImage const& proto)
    -> Expected<sophus::AnyImage<>> {
  SOPHUS_TRY(sophus::PixelFormat, format, fromProt(proto.pixel_format()));
  SOPHUS_TRY(auto, layout, fromProt(proto.layout()));

  SOPHUS_ASSERT_EQ(size_t(layout.sizeBytes()), proto.data().size());
  SOPHUS_TRY(
      auto, mut_image, sophus::MutAnyImage<>::tryFromFormat(layout, format));
  std::memcpy(mut_image.rawMutPtr(), proto.data().data(), proto.data().size());
  return mut_image;
}

template <>
auto toProt<sophus::AnyImage<>>(sophus::AnyImage<> const& image)
    -> core::proto::DynImage {
  core::proto::DynImage proto;
  *proto.mutable_pixel_format() = toProt(image.pixelFormat());
  *proto.mutable_layout() = toProt(image.layout());
  proto.set_data(image.rawPtr(), image.layout().sizeBytes());
  return proto;
}

Expected<sophus::IntensityImage<>> intensityImageFromProto(
    core::proto::DynImage const& proto) {
  SOPHUS_TRY(sophus::AnyImage<>, any_image, fromProt(proto));
  return sophus::IntensityImage<>::tryFrom(any_image);
}

template <>
auto toProt<sophus::IntensityImage<>>(sophus::IntensityImage<> const& image)
    -> core::proto::DynImage {
  core::proto::DynImage proto;
  *proto.mutable_pixel_format() = toProt(image.pixelFormat());
  *proto.mutable_layout() = toProt(image.layout());
  proto.set_data(image.rawPtr(), image.layout().sizeBytes());
  return proto;
}

}  // namespace farm_ng
