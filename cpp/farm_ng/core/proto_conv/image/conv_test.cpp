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

#include "farm_ng/core/logging/fmt_eigen.h"

#include <gtest/gtest.h>

namespace farm_ng::test {

using namespace sophus2;

template <class TPixel>
void testPixelFormat() {
  auto format_in = PixelFormat::fromTemplate<TPixel>();
  auto maybe_format = fromProt(toProt(format_in));
  PixelFormat format_out = SOPHUS_UNWRAP(maybe_format);
  SOPHUS_ASSERT_EQ(format_in, format_out);
}

template <class TPixel>
void testDynImage(std::vector<TPixel> const& pixels) {
  MutImage<TPixel> mut_image(ImageSize(7, 4));
  size_t i = 0;
  for (int y = 0; y < 4; ++y) {
    for (int x = 0; x < 7; ++x) {
      mut_image.mut(x, y) = pixels[i];
      i = (i + 1) % pixels.size();
    }
  }
  DynImage<> dyn_image_in(std::move(mut_image));
  auto proto = toProt(dyn_image_in);
  auto maybe_dyn_image = fromProt(proto);

  DynImage<> dyn_image_out = SOPHUS_UNWRAP(maybe_dyn_image);
  SOPHUS_ASSERT_IMAGE_EQ(
      dyn_image_in.image<TPixel>(), dyn_image_out.image<TPixel>());
}

template <class TPixel>
void testIntensityImage(std::vector<TPixel> const& pixels) {
  MutImage<TPixel> mut_image(ImageSize(7, 4));
  size_t i = 0;
  for (int y = 0; y < 4; ++y) {
    for (int x = 0; x < 7; ++x) {
      mut_image.mut(x, y) = pixels[i];
      i = (i + 1) % pixels.size();
    }
  }
  IntensityImage<> dyn_image_in(std::move(mut_image));
  auto proto = toProt(dyn_image_in);
  auto maybe_dyn_image = intensityImageFromProto(proto);
  IntensityImage<> dyn_image_out = SOPHUS_UNWRAP(maybe_dyn_image);

  SOPHUS_ASSERT_IMAGE_EQ(
      dyn_image_in.image<TPixel>(), dyn_image_out.image<TPixel>());
}

TEST(conv_image, roundtrip) {
  ImageSize size_in(600, 480);
  ImageSize size_out = *fromProt(toProt(size_in));
  SOPHUS_ASSERT_EQ(size_in, size_out);

  ImageLayout layout_in(size_in, 640);
  ImageLayout layout_out = *fromProt(toProt(layout_in));
  SOPHUS_ASSERT_EQ(layout_in, layout_out);

  testPixelFormat<float>();
  testPixelFormat<uint16_t>();
  testPixelFormat<uint8_t>();
  testPixelFormat<Eigen::Vector2f>();
  testPixelFormat<Eigen::Vector3<uint16_t>>();

  testDynImage<float>({0.0, 1.0, 0.5});
  testDynImage<uint16_t>({0u, 1111u, 34u});
  testDynImage<Eigen::Vector3f>(
      {Eigen::Vector3f::Ones(),
       Eigen::Vector3f::Zero(),
       Eigen::Vector3f(0.1, 0.2, 0.3)});

  testIntensityImage<float>({0.0, 1.0, 0.5});
  testIntensityImage<uint16_t>({0u, 1111u, 34u});
  testIntensityImage<Eigen::Vector3f>(
      {Eigen::Vector3f::Ones(),
       Eigen::Vector3f::Zero(),
       Eigen::Vector3f(0.1, 0.2, 0.3)});
}
}  // namespace farm_ng::test
