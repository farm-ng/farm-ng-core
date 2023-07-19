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

#include "farm_ng/core/pipeline/input.h"
#include "farm_ng/core/pipeline/output.h"
#include "farm_ng/core/pipeline/thread_pool.h"

#include <farm_ng/core/logging/logger.h>
#include <gtest/gtest.h>

namespace farm_ng {
class Adder : public Component {
 public:
  Adder(Context const& ctx)
      : Component(ctx, "other", this, "component_test"),
        in_a_(this, "a", std::bind(&Adder::onA, this, std::placeholders::_1)),
        in_b_(this, "b", std::bind(&Adder::onB, this, std::placeholders::_1)),
        out_sum_(this, "sum") {}

  Input<int>& inA() { return in_a_; }
  Input<int>& inB() { return in_b_; }
  Output<int>& outSum() { return out_sum_; }

 private:
  void onA(int a) {
    a_ = a;
    out_sum_.send(a_ + b_);
  }

  void onB(int b) {
    b_ = b;
    out_sum_.send(a_ + b_);
  }

  Input<int> in_a_;
  Input<int> in_b_;
  Output<int> out_sum_;

  int a_ = 0;
  int b_ = 0;
};

class Printer : public Component {
 public:
  Printer(Context const& ctx)
      : Component(ctx, "other", this, "component_test"),
        in_x(this, "x", std::bind(&Printer::onX, this, std::placeholders::_1)) {
  }

  Input<int> in_x;

 private:
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void onX(int x) { std::cout << "x: " << x << std::endl; }
};

}  // namespace farm_ng

using namespace farm_ng;

TEST(Component, Connect) {
  Context ctx;
  Adder adder(ctx);
  FARM_ASSERT_EQ(adder.uri(), "other://[farm_ng::Adder]/component_test");
  Printer printer(ctx);
  FARM_ASSERT_EQ(printer.uri(), "other://[farm_ng::Printer]/component_test");
  adder.outSum().connect(printer.in_x);
  ctx.clearWorkGuard();
  ctx.getAsioIoContext().run();
  adder.inA().send(1);
  adder.inB().send(2);
}

TEST(Component, TwoThreadsTest) {
  Context ctx1;
  Adder adder(ctx1);
  Context ctx2;

  // add thread id checker
  Printer printer(ctx2);

  adder.outSum().connect(printer.in_x);

  ThreadPool pool1(ctx1.getAsioIoContextPtr());
  ThreadPool pool2(ctx2.getAsioIoContextPtr());

  pool1.start(1);
  pool2.start(1);

  adder.inA().send(1);
  adder.inB().send(2);

  ctx1.clearWorkGuard();
  ctx2.clearWorkGuard();
  pool1.join();
  pool2.join();
}
