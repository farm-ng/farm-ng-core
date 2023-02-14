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

// Change FARM_LOG_LEVEL define to see the effect of compile-time log levels
// #define FARM_LOG_LEVEL FARM_LEVEL_TRACE
#define FARM_LOG_LEVEL FARM_LEVEL_DEBUG

#include "farm_ng/core/logging/logger.h"

#include <gtest/gtest.h>

#include <optional>
#include <regex>

class CaptureStdErr {
 public:
  CaptureStdErr() {
    orig_std_err_buffer_ = std::cerr.rdbuf();
    std::cerr.rdbuf(buffer_.rdbuf());
  }
  ~CaptureStdErr() { std::cerr.rdbuf(orig_std_err_buffer_); }

  std::string buffer() const { return buffer_.str(); }

 private:
  std::stringstream buffer_;
  std::streambuf* orig_std_err_buffer_;
};

void expectContains(std::string const& str, std::regex const& regex) {
  EXPECT_TRUE(std::regex_search(str, regex)) << str;
}

void expectNotContains(std::string const& str, std::regex const& regex) {
  EXPECT_FALSE(std::regex_search(str, regex)) << str;
}

void expectContainsIf(
    bool condition, std::string const& str, std::regex const& regex) {
  if (condition) {
    expectContains(str, regex);
  } else {
    expectNotContains(str, regex);
  }
}

TEST(logger, default_log_level) {
  CaptureStdErr capture;
  FARM_CRITICAL("0");
  FARM_ERROR("{}", 1);
  FARM_WARN("{}", 2);
  FARM_INFO("{}{}", 3, 4);
  FARM_DEBUG("{}", 5);
  FARM_TRACE("{}", 6);
  expectContains(capture.buffer(), std::regex{R"(\[FARM CRITICAL in.*0)"});
  expectContains(capture.buffer(), std::regex{R"(\[FARM ERROR in.*1)"});
  expectContains(capture.buffer(), std::regex{R"(\[FARM WARN in.*2)"});
  expectContains(capture.buffer(), std::regex{R"(\[FARM INFO in.*34)"});

  expectContainsIf(
      FARM_LOG_LEVEL <= FARM_LEVEL_DEBUG,
      capture.buffer(),
      std::regex{R"(\[FARM DEBUG in.*5)"});

  expectContainsIf(
      FARM_LOG_LEVEL <= FARM_LEVEL_TRACE,
      capture.buffer(),
      std::regex{R"(\[FARM TRACE in.*6)"});
}

TEST(logger, runtime_log_level) {
  CaptureStdErr capture;
  auto const orig_log_level = farm_ng::defaultLogger().getLogLevel();

  farm_ng::defaultLogger().setLogLevel(farm_ng::LogLevel::critical);
  FARM_CRITICAL("0");
  FARM_ERROR("{}", 1);
  expectContains(capture.buffer(), std::regex{R"(\[FARM CRITICAL in.*0)"});
  expectNotContains(capture.buffer(), std::regex{R"(\[FARM ERROR in.*1)"});

  farm_ng::defaultLogger().setLogLevel(farm_ng::LogLevel::trace);
  FARM_WARN("{}", 2);
  FARM_INFO("{}{}", 3, 4);
  FARM_DEBUG("{}", 5);
  FARM_TRACE("{}", 6);
  expectContains(capture.buffer(), std::regex{R"(\[FARM WARN in.*2)"});
  expectContains(capture.buffer(), std::regex{R"(\[FARM INFO in.*34)"});
  expectContains(capture.buffer(), std::regex{R"(\[FARM DEBUG in.*5)"});

  expectContainsIf(
      FARM_LOG_LEVEL <= FARM_LEVEL_DEBUG,
      capture.buffer(),
      std::regex{R"(\[FARM DEBUG in.*5)"});

  expectContainsIf(
      FARM_LOG_LEVEL <= FARM_LEVEL_TRACE,
      capture.buffer(),
      std::regex{R"(\[FARM TRACE in.*6)"});

  // Revert
  //
  // With a more expressive API for the log macros, e.g. LOG_WARN(logger, ...),
  // we could test against a local logger rather than defaultLogger().
  farm_ng::defaultLogger().setLogLevel(orig_log_level);
}

TEST(logger, header_format) {
  CaptureStdErr capture;
  auto const orig_header_format = farm_ng::defaultLogger().getHeaderFormat();

  // Example 1
  farm_ng::defaultLogger().setHeaderFormat("{level}!");
  FARM_CRITICAL("foo");
  expectContains(capture.buffer(), std::regex{R"(CRITICAL!foo)"});

  // Example 2, custom formatting
  farm_ng::defaultLogger().setHeaderFormat(
      "[{time:%Y-%m-%d %H:%M:%S}.{time_ms}]");
  FARM_INFO("bar");
  expectContains(
      capture.buffer(),
      std::regex{R"(\[\d{4}\-\d{2}\-\d{2} \d{2}:\d{2}:\d{2}.\d{3}\]bar)"});

  // Revert
  farm_ng::defaultLogger().setHeaderFormat(orig_header_format);
  FARM_CRITICAL("baz");
  expectContains(capture.buffer(), std::regex{R"(\[FARM CRITICAL in.*baz)"});
}

TEST(logger, unit) {
  FARM_ASSERT_NEAR(1.0, 1.01, 0.03);
  ASSERT_DEATH({ FARM_ASSERT_NEAR(1.0, 1.1, 0.05); }, "ASSERT_NEAR");

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
