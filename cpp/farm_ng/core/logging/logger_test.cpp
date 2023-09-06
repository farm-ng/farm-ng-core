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

#include "farm_ng/core/logging/logger.h"

#include "farm_ng/core/logging/test_common.h"

#include <gtest/gtest.h>

#include <optional>
#include <regex>

using namespace farm_ng;
using namespace farm_ng::testing;

void invokeAllLogMacros() {
  FARM_CRITICAL("5");
  FARM_ERROR("{}", 4);
  FARM_WARN("{}", 3);
  FARM_INFO("{}", 2);
}

TEST(logger, compile_time_default_runtime_default) {  // NOLINT
  CaptureStdErr capture;
  invokeAllLogMacros();
  EXPECT_CONTAINS(capture.buffer(), std::regex{R"(FARM CRITICAL in)"});
  EXPECT_CONTAINS(capture.buffer(), std::regex{R"(FARM ERROR in)"});
  EXPECT_CONTAINS(capture.buffer(), std::regex{R"(\[FARM WARN in.*3)"});
  // default log level is WARN and higher, so we do not expect to see INFO.
  EXPECT_NOT_CONTAINS(capture.buffer(), std::regex{R"(\[FARM INFO in.*2)"});
}

TEST(logger, compile_time_default_runtime_critical) {  // NOLINT
  CaptureStdErr capture;
  auto const orig_log_level = defaultLogger().getLogLevel();

  defaultLogger().setLogLevel(LogLevel::critical);
  invokeAllLogMacros();
  EXPECT_CONTAINS(capture.buffer(), std::regex{R"(FARM CRITICAL in)"});
  EXPECT_NOT_CONTAINS(capture.buffer(), std::regex{R"(\[FARM ERROR in.*4)"});
  EXPECT_NOT_CONTAINS(capture.buffer(), std::regex{R"(\[FARM WARN in.*3)"});
  EXPECT_NOT_CONTAINS(capture.buffer(), std::regex{R"(\[FARM INFO in.*2)"});
  defaultLogger().setLogLevel(orig_log_level);
}

TEST(logger, compile_time_default_runtime_trace) {  // NOLINT
  CaptureStdErr capture;
  auto const orig_log_level = defaultLogger().getLogLevel();

  defaultLogger().setLogLevel(LogLevel::info);
  invokeAllLogMacros();
  EXPECT_CONTAINS(capture.buffer(), std::regex{R"(FARM CRITICAL in)"});
  EXPECT_CONTAINS(capture.buffer(), std::regex{R"(\[FARM ERROR in)"});
  EXPECT_CONTAINS(capture.buffer(), std::regex{R"(FARM WARN in)"});
  EXPECT_CONTAINS(capture.buffer(), std::regex{R"(FARM INFO in)"});
  defaultLogger().setLogLevel(orig_log_level);
}

TEST(logger, header_format) {  // NOLINT
  CaptureStdErr capture;
  auto const orig_header_format = defaultLogger().getHeaderFormat();

  // Example 1
  defaultLogger().setHeaderFormat("{level}!");
  FARM_CRITICAL("foo");
  EXPECT_CONTAINS(capture.buffer(), std::regex{R"(CRITICAL!foo)"});

  // Revert
  defaultLogger().setHeaderFormat(orig_header_format);
  FARM_CRITICAL("baz");
  EXPECT_CONTAINS(capture.buffer(), std::regex{R"(FARM CRITICAL in)"});
}

TEST(logger, unit) {  // NOLINT
  FARM_ASSERT_NEAR(1.0, 1.01, 0.03);
  FARM_ASSERT_NEAR(999999.0, 999999.9, 0.001);
  ASSERT_DEATH({ FARM_ASSERT_NEAR(1.0, 1.1, 0.001); }, "ASSERT_NEAR relative");
  ASSERT_DEATH({ FARM_ASSERT_NEAR(1.0, -1.01, 0.03); }, "ASSERT_NEAR relative");
  ASSERT_DEATH(
      { FARM_ASSERT_NEAR(999999.0, 999999.9, 0.00000001); },
      "ASSERT_NEAR relative");
  ASSERT_DEATH(
      { FARM_ASSERT_NEAR(0.0, 0.001, 0.0000001); }, "ASSERT_NEAR absolute");

  std::optional<int> maybe_foo;
  maybe_foo = 2;
  int foo = FARM_UNWRAP(maybe_foo);
  FARM_ASSERT_EQ(foo, 2);

  FARM_UNWRAP(maybe_foo) = 1;
  FARM_ASSERT_EQ(*maybe_foo, 1);

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
    FARM_ASSERT_EQ(ints.at(0), 2);

    int val = FARM_AT(ints, 2);
    FARM_ASSERT_EQ(val, 1);

    ASSERT_DEATH({ FARM_AT(ints, 3) = 7.0; }, "FARM_AT");

    ASSERT_DEATH({ int foo = FARM_AT(ints, 3); }, "FARM_AT");
    ASSERT_DEATH({ int foo = FARM_AT(ints, 3, "custom{}", 42); }, "custom42");
  }

  {
    std::map<int, std::string> map_of_strings = {{2, "foo"}, {7, "bar"}};
    FARM_GET(map_of_strings, 2).second = "daz";
    FARM_ASSERT_EQ(map_of_strings[2], "daz");

    std::string val = FARM_GET(map_of_strings, 7).second;
    FARM_ASSERT_EQ(val, "bar");

    ASSERT_DEATH(
        { FARM_GET(map_of_strings, 3).second = "blabla"; }, "FARM_GET");
    ASSERT_DEATH(
        { FARM_GET(map_of_strings, 3, "foo").second = "blabla"; }, "foo");

    ASSERT_DEATH(
        { std::string __ = FARM_GET(map_of_strings, 1).second; }, "FARM_GET");

    auto it = FARM_MAP_INSERT(map_of_strings, 3, "baz");
    FARM_ASSERT(it == map_of_strings.find(3));

    ASSERT_DEATH(FARM_MAP_INSERT(map_of_strings, 3, "boo"), "boo");
  }
}
