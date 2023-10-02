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

#pragma once

#include <gtest/gtest.h>

#include <regex>

namespace farm_ng::testing {

/// Captures stdout and stderr.
class CaptureStdErr {
 public:
  CaptureStdErr() {
    orig_std_err_buffer_ = std::cerr.rdbuf();
    std::cerr.rdbuf(buffer_.rdbuf());
  }
  ~CaptureStdErr() { std::cerr.rdbuf(orig_std_err_buffer_); }

  /// Returns the captured output.
  std::string buffer() const { return buffer_.str(); }

 private:
  std::stringstream buffer_;
  std::streambuf* orig_std_err_buffer_;
};

// These are macros so we see the line number in the test where the error
// occurs.
#define EXPECT_CONTAINS(str, regex)              \
  EXPECT_TRUE(std::regex_search((str), (regex))) \
      << FARM_FORMAT("str = `{}`", str)

#define EXPECT_NOT_CONTAINS(str, regex)           \
  EXPECT_FALSE(std::regex_search((str), (regex))) \
      << FARM_FORMAT("str = `{}`", str)
}  // namespace farm_ng::testing
