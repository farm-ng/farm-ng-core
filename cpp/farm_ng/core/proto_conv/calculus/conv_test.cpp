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

#include <gtest/gtest.h>

using namespace sophus;

using namespace farm_ng;
using namespace farm_ng::core;

TEST(conv_calculus, unit) {
  {
    sophus::RegionI region = sophus::RegionI::empty();
    auto proto = toProt(region);
    EXPECT_TRUE(proto.is_empty());
    auto maybe_region = fromProt<core::proto::RegionI32>(proto);
    auto region_copy = FARM_UNWRAP(maybe_region);
    EXPECT_TRUE(region_copy.isEmpty());

    region = sophus::RegionI::fromMinMax(1, 2);
    proto = toProt(region);
    EXPECT_FALSE(proto.is_empty());
    maybe_region = fromProt<core::proto::RegionI32>(proto);
    region_copy = FARM_UNWRAP(maybe_region);
    EXPECT_FALSE(region_copy.isEmpty());
    EXPECT_EQ(region_copy.min(), 1);
    EXPECT_EQ(region_copy.max(), 2);
  }
  {
    sophus::RegionF32 region = sophus::RegionF32::empty();
    auto proto = toProt(region);
    EXPECT_TRUE(proto.is_empty());
    auto maybe_region = fromProt<core::proto::RegionF32>(proto);
    auto region_copy = FARM_UNWRAP(maybe_region);
    EXPECT_TRUE(region_copy.isEmpty());

    region = sophus::RegionF32::fromMinMax(1, 2);
    proto = toProt(region);
    EXPECT_FALSE(proto.is_empty());
    maybe_region = fromProt<core::proto::RegionF32>(proto);
    region_copy = FARM_UNWRAP(maybe_region);
    EXPECT_FALSE(region_copy.isEmpty());
    EXPECT_EQ(region_copy.min(), 1);
    EXPECT_EQ(region_copy.max(), 2);
  }

  {
    sophus::RegionF64 region = sophus::RegionF64::empty();
    auto proto = toProt(region);
    EXPECT_TRUE(proto.is_empty());
    auto maybe_region = fromProt<core::proto::RegionF64>(proto);
    auto region_copy = FARM_UNWRAP(maybe_region);
    EXPECT_TRUE(region_copy.isEmpty());

    region = sophus::RegionF64::fromMinMax(1, 2);
    proto = toProt(region);
    EXPECT_FALSE(proto.is_empty());
    maybe_region = fromProt<core::proto::RegionF64>(proto);
    region_copy = FARM_UNWRAP(maybe_region);
    EXPECT_FALSE(region_copy.isEmpty());
    EXPECT_EQ(region_copy.min(), 1);
    EXPECT_EQ(region_copy.max(), 2);
  }
}
