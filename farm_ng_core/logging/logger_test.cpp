// Copyright (c) farm-ng, inc. All rights reserved.

#include <farm_ng_core/logging/logger.h>

#include <gtest/gtest.h>

#include <optional>

using namespace farm_ng_core;

TEST(logger, unit) {
  FNG_CHECK_NEAR(1.0, 1.01, 0.03);
  ASSERT_DEATH({ FNG_CHECK_NEAR(1.0, 1.1, 0.05); }, "CHECK_NEAR");

  std::optional<int> maybe_foo;
  maybe_foo = 2;
  int foo = FNG_UNWRAP(maybe_foo);
  FNG_CHECK_EQ(foo, 2);
  FNG_UNWRAP(maybe_foo) = 1;
  FNG_CHECK_EQ(*maybe_foo, 1);

  std::optional<int> maybe_bar;
  ASSERT_DEATH({ auto bar = FNG_UNWRAP(maybe_bar); }, "UNWRAP");
  ASSERT_DEATH(
      { auto bar = FNG_UNWRAP(maybe_bar, "custom-{}", "foo"); }, "custom-foo");

  std::shared_ptr<float> ff(new float(42.42));
  FNG_UNWRAP(ff) = 43.0;

  std::shared_ptr<float> fff;
  ASSERT_DEATH({ FNG_UNWRAP(fff) = 42.0; }, "UNWRAP");

  {
    std::vector<int> ints(3u, 1);

    FNG_AT(ints, 0) = 2;
    FNG_CHECK_EQ(ints.at(0), 2);

    int val = FNG_AT(ints, 2);
    FNG_CHECK_EQ(val, 1);

    ASSERT_DEATH({ FNG_AT(ints, 3) = 7.0; }, "FNG_AT");

    ASSERT_DEATH({ int foo = FNG_AT(ints, 3); }, "FNG_AT");
    ASSERT_DEATH({ int foo = FNG_AT(ints, 3, "custom{}", 42); }, "custom42");
  }

  {
    std::map<int, std::string> strings = {{2, "foo"}, {7, "bar"}};
    FNG_GET(strings, 2).second = "daz";
    FNG_CHECK_EQ(strings[2], "daz");

    std::string val = FNG_GET(strings, 7).second;
    FNG_CHECK_EQ(val, "bar");

    ASSERT_DEATH({ FNG_GET(strings, 3).second = "blabla"; }, "FNG_GET");
    ASSERT_DEATH({ FNG_GET(strings, 3, "foo").second = "blabla"; }, "foo");

    ASSERT_DEATH({ std::string __ = FNG_GET(strings, 1).second; }, "FNG_GET");
  }
}
