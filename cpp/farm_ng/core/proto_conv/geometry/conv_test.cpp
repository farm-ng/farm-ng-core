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

#include "farm_ng/core/proto_conv/geometry/conv.h"

#include <gtest/gtest.h>

using namespace sophus2;

using namespace farm_ng;
using namespace farm_ng::core;

TEST(conv_geometry, unit) {
  {
    auto uvec =
        sophus2::UnitVector3F64::fromVectorAndNormalize({1.0, 2.0, 3.0});
    proto::UnitVec3F64 proto = toProt(uvec);
    EXPECT_EQ(proto.vec3().x(), uvec.vector().x());
    EXPECT_EQ(proto.vec3().y(), uvec.vector().y());
    EXPECT_EQ(proto.vec3().z(), uvec.vector().z());

    auto maybe_uvec2 = fromProt(proto);
    sophus2::UnitVector3F64 uvec2 = FARM_UNWRAP(maybe_uvec2);
    EXPECT_EQ(uvec2.vector(), uvec.vector());
    auto vec = uvec2.vector();
    EXPECT_NEAR(vec.norm(), 1.0, 1e-6);
    EXPECT_EQ(vec.x(), proto.vec3().x());
    EXPECT_EQ(vec.y(), proto.vec3().y());
    EXPECT_EQ(vec.z(), proto.vec3().z());
  }
}
