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

// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng/core/pipeline/context.h"

#include <gtest/gtest.h>

using namespace farm_ng;

TEST(Context, Construct) {  // NOLINT
  Context ctx1;
  Context ctx2;

  EXPECT_NE(&ctx1.getAsioIoContext(), &ctx2.getAsioIoContext());
  EXPECT_NE(ctx1.getAsioIoContextPtr(), ctx2.getAsioIoContextPtr());

  Context ctx3 = ctx2;
  EXPECT_EQ(&ctx2.getAsioIoContext(), &ctx3.getAsioIoContext());
  EXPECT_EQ(ctx2.getAsioIoContextPtr(), ctx3.getAsioIoContextPtr());
}

TEST(ContextStrand, Construct) {
  Context ctx1;
  ContextStrand strand1(ctx1);
  EXPECT_EQ(&ctx1.getAsioIoContext(), &strand1.getAsioIoContext());

  ContextStrand strand2 = strand1;
  EXPECT_EQ(&strand1.getAsioIoContext(), &strand2.getAsioIoContext());
  EXPECT_EQ(&strand1.getAsioStrand(), &strand2.getAsioStrand());
}
