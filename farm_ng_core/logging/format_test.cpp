// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <farm_ng_core/logging/format.h>
#include <farm_ng_core/logging/logger.h>
#include <gtest/gtest.h>

using namespace farm_ng_core;

TEST(format, functional) {
  int i = 10;
  uint8_t hex = 0x1f;
  double d = -11.2348677;
  std::string s = "test-string-!@#$%^&*()";

  FNG_CHECK_EQ(FNG_FORMAT("i: {}", i), "i: " + std::to_string(i));
  FNG_CHECK_EQ(
      FNG_FORMAT("dec: {} | hex: {:02x}", hex, hex), "dec: 31 | hex: 1f");
  FNG_CHECK_EQ(FNG_FORMAT("d: {:0.3f} | s: {}", d, s), "d: -11.235 | s: " + s);
}

TEST(runtime_format, functional) {
  int i = 10;
  uint8_t hex = 0x1f;
  double d = -11.2348677;
  std::string s = "test-string-!@#$%^&*()";

  FNG_CHECK_EQ(FNG_RUNTIME_FORMAT("i: {}", i), "i: " + std::to_string(i));
  FNG_CHECK_EQ(
      FNG_RUNTIME_FORMAT("dec: {} | hex: {:02x}", hex, hex),
      "dec: 31 | hex: 1f");
  FNG_CHECK_EQ(
      FNG_RUNTIME_FORMAT("d: {:0.3f} | s: {}", d, s), "d: -11.235 | s: " + s);
}
