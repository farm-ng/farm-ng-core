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

#include "farm_ng/core/proto_conv/sensor/conv.h"

#include <gtest/gtest.h>

using namespace sophus2;

using namespace farm_ng;
using namespace farm_ng::core;

TEST(conv_sensor, unit) {
  {
    sophus2::CameraModel pinhole = createDefaultPinholeModel({64, 24});
    proto::CameraModel proto = toProt(pinhole);
    EXPECT_EQ(proto.image_size().width(), 64);
    EXPECT_EQ(proto.image_size().height(), 24);
    EXPECT_EQ(proto.distortion_type(), toString(pinhole.distortionType()));
    ASSERT_EQ(proto.params().size(), pinhole.params().size());
    for (int i = 0; i < proto.params().size(); ++i) {
      EXPECT_EQ(proto.params(i), pinhole.params()(i));
    }

    auto maybe_pinhole2 = fromProt(proto);
    sophus2::CameraModel pinhole2 = FARM_UNWRAP(maybe_pinhole2);
    EXPECT_EQ(pinhole2.imageSize(), pinhole.imageSize());
    ASSERT_EQ(pinhole2.params(), pinhole.params());
    EXPECT_EQ(pinhole2.distortionType(), pinhole.distortionType());
  }

  {
    std::vector<sophus2::CameraModel> models;
    models.push_back(createDefaultPinholeModel({64, 24}));
    models.push_back(createDefaultOrthoModel({128, 48}));
    proto::CameraModels proto = toProt(models);
    ASSERT_EQ(proto.camera_models_size(), models.size());

    auto maybe_models2 = fromProt(proto);
    std::vector<sophus2::CameraModel> models2 = FARM_UNWRAP(maybe_models2);
    ASSERT_EQ(models2.size(), models.size());
    for (size_t i = 0; i < models.size(); ++i) {
      EXPECT_EQ(models2[i].imageSize(), models[i].imageSize());
      ASSERT_EQ(models2[i].params(), models[i].params());
      EXPECT_EQ(models2[i].distortionType(), models[i].distortionType());
    }
  }
}
