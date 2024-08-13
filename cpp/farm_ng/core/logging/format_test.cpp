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

#include "farm_ng/core/logging/format.h"

#include "farm_ng/core/logging/logger.h"

#include <gtest/gtest.h>

TEST(format, functional) {  // NOLINT
  int i = 10;
  uint8_t hex = 0x1f;
  double d = -11.2348677;
  std::string s = "test-string-!@#$%^&*()";

  FARM_ASSERT_EQ(FARM_FORMAT("i: {}", i), "i: " + std::to_string(i));
  FARM_ASSERT_EQ(
      FARM_FORMAT("dec: {} | hex: {:02x}", hex, hex), "dec: 31 | hex: 1f");
  FARM_ASSERT_EQ(
      FARM_FORMAT("d: {:0.3f} | s: {}", d, s), "d: -11.235 | s: " + s);
}

TEST(format, format_error) {  // NOLINT
  int i = 10;
  float f = 10.0;

  FARM_ASSERT_EQ(
      FARM_FORMAT("i: {} {}", i), "i: {} {}; format error: argument not found");
  FARM_ASSERT_EQ(
      FARM_FORMAT("hex: {:02x}", f),
      "hex: {:02x}; format error: invalid format specifier");
}

TEST(runtime_format, functional) {  // NOLINT
  int i = 10;
  uint8_t hex = 0x1f;
  double d = -11.2348677;
  std::string s = "test-string-!@#$%^&*()";

  FARM_ASSERT_EQ(FARM_RUNTIME_FORMAT("i: {}", i), "i: " + std::to_string(i));
  FARM_ASSERT_EQ(
      FARM_RUNTIME_FORMAT("dec: {} | hex: {:02x}", hex, hex),
      "dec: 31 | hex: 1f");
  FARM_ASSERT_EQ(
      FARM_RUNTIME_FORMAT("d: {:0.3f} | s: {}", d, s), "d: -11.235 | s: " + s);
}
