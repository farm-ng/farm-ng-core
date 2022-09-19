// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "farm_ng/core/logging/logger.h"

#include <gtest/gtest.h>

#include <optional>

TEST(logger, unit) {
  FARM_CHECK_NEAR(1.0, 1.01, 0.03);
  ASSERT_DEATH({ FARM_CHECK_NEAR(1.0, 1.1, 0.05); }, "CHECK_NEAR");

  std::optional<int> maybe_foo;
  maybe_foo = 2;
  int foo = FARM_UNWRAP(maybe_foo);
  FARM_CHECK_EQ(foo, 2);
  FARM_UNWRAP(maybe_foo) = 1;
  FARM_CHECK_EQ(*maybe_foo, 1);

  std::optional<int> maybe_bar;
  ASSERT_DEATH({ auto bar = FARM_UNWRAP(maybe_bar); }, "UNWRAP");
  ASSERT_DEATH(
      { auto bar = FARM_UNWRAP(maybe_bar, "custom-{}", "foo"); }, "custom-foo");

  std::shared_ptr<float> ff(new float(42.42));
  FARM_UNWRAP(ff) = 43.0;

  std::shared_ptr<float> fff;
  ASSERT_DEATH({ FARM_UNWRAP(fff) = 42.0; }, "UNWRAP");

  {
    std::vector<int> ints(3u, 1);

    FARM_AT(ints, 0) = 2;
    FARM_CHECK_EQ(ints.at(0), 2);

    int val = FARM_AT(ints, 2);
    FARM_CHECK_EQ(val, 1);

    ASSERT_DEATH({ FARM_AT(ints, 3) = 7.0; }, "FARM_AT");

    ASSERT_DEATH({ int foo = FARM_AT(ints, 3); }, "FARM_AT");
    ASSERT_DEATH({ int foo = FARM_AT(ints, 3, "custom{}", 42); }, "custom42");
  }

  {
    std::map<int, std::string> map_of_strings = {{2, "foo"}, {7, "bar"}};
    FARM_GET(map_of_strings, 2).second = "daz";
    FARM_CHECK_EQ(map_of_strings[2], "daz");

    std::string val = FARM_GET(map_of_strings, 7).second;
    FARM_CHECK_EQ(val, "bar");

    ASSERT_DEATH(
        { FARM_GET(map_of_strings, 3).second = "blabla"; }, "FARM_GET");
    ASSERT_DEATH(
        { FARM_GET(map_of_strings, 3, "foo").second = "blabla"; }, "foo");

    ASSERT_DEATH(
        { std::string __ = FARM_GET(map_of_strings, 1).second; }, "FARM_GET");

    auto it = FARM_MAP_INSERT(map_of_strings, 3, "baz");
    FARM_CHECK(it == map_of_strings.find(3));

    ASSERT_DEATH(FARM_MAP_INSERT(map_of_strings, 3, "boo"), "boo");
  }
}
