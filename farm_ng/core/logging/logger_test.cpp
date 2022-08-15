// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng/core/logging/logger.h"

#include "farm_ng/core/logging/eigen.h"

#include <gtest/gtest.h>

using namespace farm_ng_core;

TEST(logger, unit) {
  FARM_NG_CHECK_NEAR(1.0, 1.01, 0.03);
  ASSERT_DEATH({ FARM_NG_CHECK_NEAR(1.0, 1.1, 0.05); }, "CHECK_NEAR");

  FARM_NG_CHECK_NEAR(Eigen::Vector2d(0.1, 0.2), Eigen::Vector2d(0.2, 0.1), 0.2);
  ASSERT_DEATH(
      {
        FARM_NG_CHECK_NEAR(
            Eigen::Vector2d(0.1, 0.2), Eigen::Vector2d(0.2, 0.1), 0.01);
      },
      "CHECK_NEAR");

  std::optional<int> maybe_foo;
  maybe_foo = 2;
  int foo = FARM_NG_UNWRAP(maybe_foo);
  FARM_NG_CHECK_EQ(foo, 2);
  FARM_NG_UNWRAP(maybe_foo) = 1;
  FARM_NG_CHECK_EQ(*maybe_foo, 1);

  std::optional<int> maybe_bar;
  ASSERT_DEATH({ auto bar = FARM_NG_UNWRAP(maybe_bar); }, "UNWRAP");
  ASSERT_DEATH(
      { auto bar = FARM_NG_UNWRAP(maybe_bar, "custom-{}", "foo"); },
      "custom-foo");

  std::shared_ptr<float> ff(new float(42.42));
  FARM_NG_UNWRAP(ff) = 43.0;

  std::shared_ptr<float> fff;
  ASSERT_DEATH({ FARM_NG_UNWRAP(fff) = 42.0; }, "UNWRAP");

  {
    std::vector<int> ints(3u, 1);

    FARM_NG_AT(ints, 0) = 2;
    FARM_NG_CHECK_EQ(ints.at(0), 2);

    int val = FARM_NG_AT(ints, 2);
    FARM_NG_CHECK_EQ(val, 1);

    ASSERT_DEATH({ FARM_NG_AT(ints, 3) = 7.0; }, "FARM_NG_AT");

    ASSERT_DEATH({ int foo = FARM_NG_AT(ints, 3); }, "FARM_NG_AT");
    ASSERT_DEATH(
        { int foo = FARM_NG_AT(ints, 3, "custom{}", 42); }, "custom42");
  }

  {
    std::map<int, std::string> strings = {{2, "foo"}, {7, "bar"}};
    FARM_NG_GET(strings, 2).second = "daz";
    FARM_NG_CHECK_EQ(strings[2], "daz");

    std::string val = FARM_NG_GET(strings, 7).second;
    FARM_NG_CHECK_EQ(val, "bar");

    ASSERT_DEATH({ FARM_NG_GET(strings, 3).second = "blabla"; }, "FARM_NG_GET");
    ASSERT_DEATH({ FARM_NG_GET(strings, 3, "foo").second = "blabla"; }, "foo");

    ASSERT_DEATH(
        { std::string __ = FARM_NG_GET(strings, 1).second; }, "FARM_NG_GET");
  }
}
