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

#include "farm_ng/core/pipeline/output.h"

#include "farm_ng/core/pipeline/context.h"

#include <gtest/gtest.h>

using namespace farm_ng;

struct OutputTestComponent : Component {
  OutputTestComponent(Context const& ctx)
      : Component(ctx, "other", this, "output_test"), out_int(this, "int") {}

  Output<int&> out_int;
};

TEST(Component, Construct) {  // NOLINT
  Context ctx;
  OutputTestComponent comp(ctx);

  ctx.clearWorkGuard();
  ctx.getAsioIoContext().run();
}

TEST(Component, Signal) {  // NOLINT
  Context ctx;
  OutputTestComponent comp(ctx);

  auto& sig = comp.out_int.signal();
  sig.connect([](int& a) { a = 3; });

  int a = 2;
  sig(a);
  EXPECT_EQ(a, 3);
}
