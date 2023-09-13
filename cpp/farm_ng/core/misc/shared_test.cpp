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

#include "farm_ng/core/misc/shared.h"

#include <gtest/gtest.h>

#include <memory>

using namespace farm_ng;

struct ErrorTag {};

struct Foo {
  Foo(int val) : val(val) {}
  Foo(ErrorTag /*unused*/) { throw std::runtime_error("ConstructorThrows"); }
  [[nodiscard]] auto testMethod() const -> int { return val; }

  int val;
};

auto makeSomething(int val, bool succeed) -> ExpectShared<Foo> {
  if (succeed) {
    return tryMakeShared<Foo>(val);
  }
  return tryMakeShared<Foo>(ErrorTag());
}

TEST(Shared, Basic) {  // NOLINT
  if (auto maybe_int = tryMakeShared<int>(5)) {
    int t = **maybe_int + 10;
    EXPECT_EQ(t, 15);
  } else {
    FAIL();
  }
}

TEST(Shared, Rethrow) {  // NOLINT
  // User doesn't deal with error so we must throw
  if (auto t = makeSomething(42, true)) {
    EXPECT_EQ((*t)->testMethod(), 42);
  } else {
    FAIL();
  }

  auto t = makeSomething(73, false);
  if (t) {
    FAIL();
  }
  EXPECT_FALSE(t.has_value());
}

TEST(Shared, Conversions) {  // NOLINT
  struct Base {};
  struct Derived : public Base {};

  Shared<Base> x1 = Shared<Base>::make();
  Shared<Base> x2 = Shared<Derived>::make();
  Shared<Base> x3 = x1;
  Shared<Base> x4 = std::make_unique<Base>();
  Shared<Base> x5 = std::make_shared<Base>();
  Shared<Base> x6 = std::make_unique<Derived>();
  Shared<Base> x7 = std::make_shared<Derived>();

  EXPECT_DEATH({ Shared<Base> x = std::unique_ptr<Derived>(nullptr); }, "");
  EXPECT_DEATH({ Shared<Base> x = std::shared_ptr<Derived>(nullptr); }, "");
}
