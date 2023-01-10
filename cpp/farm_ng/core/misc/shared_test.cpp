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

using namespace farm_ng;

struct ErrorTag {};

struct Foo {
  Foo(int val) : val_(val) {}
  Foo(ErrorTag) { throw std::runtime_error("ConstructorThrows"); }
  int test_method() { return val_; }

  int val_;
};

ExpectShared<Foo> makeSomething(int val, bool succeed) {
  if (succeed) {
    return tryMakeShared<Foo>(val);
  } else {
    return tryMakeShared<Foo>(ErrorTag());
  }
}

TEST(Shared, Basic) {
  if (auto maybe_int = tryMakeShared<int>(5)) {
    int t = **maybe_int + 10;
    EXPECT_EQ(t, 15);
  } else {
    FAIL();
  }
}

TEST(Shared, Rethrow) {
  // User doesn't deal with error so we must throw
  if (auto t = makeSomething(42, true)) {
    EXPECT_EQ((*t)->test_method(), 42);
  } else {
    FAIL();
  }

  if (auto t = makeSomething(73, false)) {
    FAIL();
  } else {
    EXPECT_FALSE(t.has_value());
  }
}

TEST(Shared, Conversions) {
  struct Base {};
  struct Derived : public Base {};

  Shared<Base> x1 = Shared<Base>::make();
  Shared<Base> x2 = Shared<Derived>::make();
  Shared<Base> x3 = x1;
  Shared<Base> x4 = std::unique_ptr<Base>(new Base());
  Shared<Base> x5 = std::shared_ptr<Base>(new Base());
  Shared<Base> x6 = std::unique_ptr<Derived>(new Derived());
  Shared<Base> x7 = std::shared_ptr<Derived>(new Derived());

  EXPECT_ANY_THROW(
      []() { Shared<Base> x = std::unique_ptr<Derived>(nullptr); }());
  EXPECT_ANY_THROW(
      []() { Shared<Base> x = std::shared_ptr<Derived>(nullptr); }());
}
