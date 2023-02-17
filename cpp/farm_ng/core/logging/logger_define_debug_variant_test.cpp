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

#define FARM_LOG_LEVEL FARM_LOG_LEVEL_DEBUG
#include "farm_ng/core/logging/test_common.h"
#include "farm_ng/core/logging/trace_debug_log.h"

#include <gtest/gtest.h>

#include <optional>
#include <regex>

using namespace farm_ng;
using namespace farm_ng::testing;

void invokeInfoDebugTraceLogMacros() {
  FARM_INFO("{}", 2);
  FARM_DEBUG("{}", 1);
  FARM_TRACE("{}", 0);
}

TEST(logger, compile_time_debug_runtime_info) {
  CaptureStdErr capture;
  auto const orig_log_level = defaultLogger().getLogLevel();
  defaultLogger().setLogLevel(LogLevel::info);

  invokeInfoDebugTraceLogMacros();
  EXPECT_CONTAINS(capture.buffer(), std::regex{R"(\[FARM INFO in.*2)"});
  EXPECT_NOT_CONTAINS(capture.buffer(), std::regex{R"(\[FARM DEBUG in.*1)"});
  EXPECT_NOT_CONTAINS(capture.buffer(), std::regex{R"(\[FARM TRACE in.*0)"});

  defaultLogger().setLogLevel(orig_log_level);
}

TEST(logger, compile_time_debug_runtime_debug) {
  CaptureStdErr capture;
  auto const orig_log_level = defaultLogger().getLogLevel();
  defaultLogger().setLogLevel(LogLevel::debug);

  invokeInfoDebugTraceLogMacros();
  EXPECT_CONTAINS(capture.buffer(), std::regex{R"(\[FARM INFO in.*2)"});
  EXPECT_CONTAINS(capture.buffer(), std::regex{R"(\[FARM DEBUG in.*1)"});
  EXPECT_NOT_CONTAINS(capture.buffer(), std::regex{R"(\[FARM TRACE in.*0)"});

  defaultLogger().setLogLevel(orig_log_level);
}

TEST(logger, compile_time_debug_runtime_trace) {
  CaptureStdErr capture;
  auto const orig_log_level = defaultLogger().getLogLevel();
  defaultLogger().setLogLevel(LogLevel::trace);

  invokeInfoDebugTraceLogMacros();
  EXPECT_CONTAINS(capture.buffer(), std::regex{R"(\[FARM INFO in.*2)"});
  EXPECT_CONTAINS(capture.buffer(), std::regex{R"(\[FARM DEBUG in.*1)"});
  EXPECT_NOT_CONTAINS(capture.buffer(), std::regex{R"(\[FARM TRACE in.*0)"});

  defaultLogger().setLogLevel(orig_log_level);
}
