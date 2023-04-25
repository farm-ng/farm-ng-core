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

#include "farm_ng/core/pipeline/component.h"
#include "farm_ng/core/pipeline/context.h"

#include <gtest/gtest.h>

using namespace farm_ng;

struct InputTestComponent : Component {
  InputTestComponent(Context const& ctx)
      : Component(ctx, "other", this, "input_test"),
        in_int(this, "int", [this](int a) { aaa = a; }) {}

  Input<int> in_int;
  int aaa = 1;
};

TEST(Component, Construct) {
  Context ctx;
  InputTestComponent comp(ctx);
  comp.in_int.send(4);
  ctx.clearWorkGuard();
  ctx.getAsioIoContext().run();
  EXPECT_EQ(comp.aaa, 4);
}
