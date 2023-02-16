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

// Note! Define a compile-time log level different than the default
#define FARM_LOG_LEVEL FARM_LEVEL_WARN
#include "farm_ng/core/logging/logger.h"
#include "farm_ng/core/logging/test_common.h"

#include <gtest/gtest.h>

#include <regex>

using namespace farm_ng;
using namespace farm_ng::testing;

void invokeAllLogMacros() {
  FARM_CRITICAL("5");
  FARM_ERROR("{}", 4);
  FARM_WARN("{}", 3);
  FARM_INFO("{}", 2);
  FARM_DEBUG("{}", 1);
  FARM_TRACE("{}", 0);
}

TEST(logger, compile_time_warn_runtime_default) {
  CaptureStdErr capture;
  invokeAllLogMacros();
  expectContains(capture.buffer(), std::regex{R"(\[FARM CRITICAL in.*5)"});
  expectContains(capture.buffer(), std::regex{R"(\[FARM ERROR in.*4)"});
  expectContains(capture.buffer(), std::regex{R"(\[FARM WARN in.*3)"});
  expectNotContains(capture.buffer(), std::regex{R"(\[FARM INFO in.*2)"});
  expectNotContains(capture.buffer(), std::regex{R"(\[FARM DEBUG in.*1)"});
  expectNotContains(capture.buffer(), std::regex{R"(\[FARM TRACE in.*0)"});
}

TEST(logger, compile_time_warn_runtime_critical) {
  CaptureStdErr capture;
  auto const orig_log_level = defaultLogger().getLogLevel();

  defaultLogger().setLogLevel(LogLevel::critical);
  invokeAllLogMacros();
  expectContains(capture.buffer(), std::regex{R"(\[FARM CRITICAL in.*5)"});
  expectNotContains(capture.buffer(), std::regex{R"(\[FARM ERROR in.*4)"});
  expectNotContains(capture.buffer(), std::regex{R"(\[FARM WARN in.*3)"});
  expectNotContains(capture.buffer(), std::regex{R"(\[FARM INFO in.*2)"});
  expectNotContains(capture.buffer(), std::regex{R"(\[FARM DEBUG in.*1)"});
  expectNotContains(capture.buffer(), std::regex{R"(\[FARM TRACE in.*0)"});

  defaultLogger().setLogLevel(orig_log_level);
}

TEST(logger, compile_time_warn_runtime_trace) {
  CaptureStdErr capture;
  auto const orig_log_level = defaultLogger().getLogLevel();

  defaultLogger().setLogLevel(LogLevel::trace);
  invokeAllLogMacros();
  expectContains(capture.buffer(), std::regex{R"(\[FARM CRITICAL in.*5)"});
  expectContains(capture.buffer(), std::regex{R"(\[FARM ERROR in.*4)"});
  expectContains(capture.buffer(), std::regex{R"(\[FARM WARN in.*3)"});
  expectNotContains(capture.buffer(), std::regex{R"(\[FARM INFO in.*2)"});
  expectNotContains(capture.buffer(), std::regex{R"(\[FARM DEBUG in.*1)"});
  expectNotContains(capture.buffer(), std::regex{R"(\[FARM TRACE in.*0)"});

  defaultLogger().setLogLevel(orig_log_level);
}