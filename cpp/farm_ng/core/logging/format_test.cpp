// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "farm_ng/core/logging/format.h"

#include "farm_ng/core/logging/logger.h"

#include <gtest/gtest.h>

TEST(format, functional) {
  int i = 10;
  uint8_t hex = 0x1f;
  double d = -11.2348677;
  std::string s = "test-string-!@#$%^&*()";

  FARM_CHECK_EQ(FARM_FORMAT("i: {}", i), "i: " + std::to_string(i));
  FARM_CHECK_EQ(
      FARM_FORMAT("dec: {} | hex: {:02x}", hex, hex), "dec: 31 | hex: 1f");
  FARM_CHECK_EQ(
      FARM_FORMAT("d: {:0.3f} | s: {}", d, s), "d: -11.235 | s: " + s);
}

TEST(runtime_format, functional) {
  int i = 10;
  uint8_t hex = 0x1f;
  double d = -11.2348677;
  std::string s = "test-string-!@#$%^&*()";

  FARM_CHECK_EQ(FARM_RUNTIME_FORMAT("i: {}", i), "i: " + std::to_string(i));
  FARM_CHECK_EQ(
      FARM_RUNTIME_FORMAT("dec: {} | hex: {:02x}", hex, hex),
      "dec: 31 | hex: 1f");
  FARM_CHECK_EQ(
      FARM_RUNTIME_FORMAT("d: {:0.3f} | s: {}", d, s), "d: -11.235 | s: " + s);
}
