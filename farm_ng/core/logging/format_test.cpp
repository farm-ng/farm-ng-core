// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng/core/logging/format.h"

#include "farm_ng/core/logging/logger.h"

#include <gtest/gtest.h>

using namespace farm_ng_core;

TEST(format, functional) {
  int i = 10;
  uint8_t hex = 0x1f;
  double d = -11.2348677;
  std::string s = "test-string-!@#$%^&*()";

  FARM_NG_CHECK_EQ(FARM_NG_FORMAT("i: {}", i), "i: " + std::to_string(i));
  FARM_NG_CHECK_EQ(
      FARM_NG_FORMAT("dec: {} | hex: {:02x}", hex, hex), "dec: 31 | hex: 1f");
  FARM_NG_CHECK_EQ(
      FARM_NG_FORMAT("d: {:0.3f} | s: {}", d, s), "d: -11.235 | s: " + s);
}

TEST(runtime_format, functional) {
  int i = 10;
  uint8_t hex = 0x1f;
  double d = -11.2348677;
  std::string s = "test-string-!@#$%^&*()";

  FARM_NG_CHECK_EQ(
      FARM_NG_RUNTIME_FORMAT("i: {}", i), "i: " + std::to_string(i));
  FARM_NG_CHECK_EQ(
      FARM_NG_RUNTIME_FORMAT("dec: {} | hex: {:02x}", hex, hex),
      "dec: 31 | hex: 1f");
  FARM_NG_CHECK_EQ(
      FARM_NG_RUNTIME_FORMAT("d: {:0.3f} | s: {}", d, s),
      "d: -11.235 | s: " + s);
}
