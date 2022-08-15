// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng/core/utils/copy_on_write.h"

#include <gtest/gtest.h>

#include <filesystem>

using namespace farm_ng_core;

TEST(copy_on_write, unit) {
  struct FooStruct {
    int i = 8;
    std::vector<float> f = {0.f, 2.f};
  };
  FooStruct foo_struct;

  CopyOnWrite<FooStruct> foo(std::move(foo_struct));
  FARM_NG_CHECK_EQ(foo.get().i, 8);
  FARM_NG_CHECK_EQ(foo.get().f.size(), 2u);

  FARM_NG_CHECK(foo.isUnique());

  CopyOnWrite<FooStruct> foo2 = foo;
  CopyOnWrite<FooStruct> foo3 = foo;
  FARM_NG_CHECK(!foo.isUnique());

  foo.mutate([](FooStruct& foo_str) {
    foo_str.i = 7;
    foo_str.f.push_back(4.f);
  });

  FARM_NG_CHECK(foo.isUnique());
  FARM_NG_CHECK_EQ(foo.get().i, 7);
  FARM_NG_CHECK_EQ(foo.get().f.size(), 3u);

  FARM_NG_CHECK(!foo2.isUnique());
  FARM_NG_CHECK_EQ(foo2.get().i, 8);
  FARM_NG_CHECK_EQ(foo2.get().f.size(), 2u);
}
