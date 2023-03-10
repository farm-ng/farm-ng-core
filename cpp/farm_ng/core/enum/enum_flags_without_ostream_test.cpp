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

// FARM_ENUM is a fork of MY_ENUM.
//
// https://github.com/facebookincubator/MY_ENUM/
//
// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "farm_ng/core/enum/enum_flags_without_ostream.h"

#include <gtest/gtest.h>

FARM_ENUMFLAGS_WITHOUT_OSTREAM(MyFlags, uint32_t, (foo, bar, daz));

TEST(EnumFlags, none) {  // NOLINT
  MyFlags flags{};
  EXPECT_FALSE(hasMask(flags, MyFlags::foo));
  EXPECT_FALSE(hasMask(flags, MyFlags::bar));
  EXPECT_FALSE(hasMask(flags, MyFlags::daz));
  EXPECT_EQ(flags, MyFlags::none);

  MyFlags flags2 = MyFlags::none;
  EXPECT_FALSE(hasMask(flags2, MyFlags::foo));
  EXPECT_FALSE(hasMask(flags2, MyFlags::bar));
  EXPECT_FALSE(hasMask(flags2, MyFlags::daz));
  EXPECT_EQ(flags2, MyFlags::none);

  EXPECT_TRUE(isSingleFlag(MyFlags::foo));
  EXPECT_TRUE(isSingleFlag(MyFlags::bar));
  EXPECT_TRUE(isSingleFlag(MyFlags::daz));
  EXPECT_FALSE(isSingleFlag(MyFlags::none));
  EXPECT_FALSE(isSingleFlag(MyFlags::bar | MyFlags::foo));
  EXPECT_FALSE(isSingleFlag(MyFlags::daz | MyFlags::foo));
}

FARM_ENUMFLAGS_WITHOUT_OSTREAM(
    EightFlags, uint8_t, (bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7));

TEST(EnumFlags, bits) {  // NOLINT
  EXPECT_EQ(uint8_t(EightFlags::none), 0);
  EXPECT_EQ(uint8_t(EightFlags::bit0), 1);
  EXPECT_EQ(uint8_t(EightFlags::bit1), 2);
  EXPECT_EQ(uint8_t(EightFlags::bit2), 4);
  EXPECT_EQ(uint8_t(EightFlags::bit3), 8);
  EXPECT_EQ(uint8_t(EightFlags::bit4), 16);
  EXPECT_EQ(uint8_t(EightFlags::bit5), 32);
  EXPECT_EQ(uint8_t(EightFlags::bit6), 64);
  EXPECT_EQ(uint8_t(EightFlags::bit7), 128);
}

TEST(EnumFlags, trySetFlagFromString) {  // NOLINT
  MyFlags flags{};
  EXPECT_TRUE(hasMask(flags, MyFlags::none));

  bool is_set = trySetFlagFromString(flags, "foo");
  EXPECT_TRUE(is_set);
  EXPECT_EQ(flags, MyFlags::foo);

  EXPECT_TRUE(hasMask(flags, MyFlags::foo));
  EXPECT_FALSE(hasMask(flags, MyFlags::bar));

  is_set = trySetFlagFromString(flags, "bar");
  EXPECT_TRUE(is_set);
  EXPECT_EQ(flags, MyFlags::foo | MyFlags::bar);
  EXPECT_TRUE(hasMask(flags, MyFlags::bar));
  EXPECT_TRUE(hasMask(flags, MyFlags::foo));

  is_set = trySetFlagFromString(flags, "daz");
  EXPECT_TRUE(is_set);
  EXPECT_EQ(flags, MyFlags::foo | MyFlags::bar | MyFlags::daz);

  is_set = trySetFlagFromString(flags, "wombaz");
  EXPECT_TRUE(!is_set);
  EXPECT_EQ(flags, MyFlags::foo | MyFlags::bar | MyFlags::daz);
}

TEST(EnumFlags, setClearToggle) {  // NOLINT
  MyFlags flags = MyFlags::foo | MyFlags::daz;
  setMask(flags, MyFlags::bar);
  EXPECT_EQ(flags, MyFlags::foo | MyFlags::bar | MyFlags::daz);
  setMask(flags, MyFlags::bar);
  EXPECT_EQ(flags, MyFlags::foo | MyFlags::bar | MyFlags::daz);
  clearMask(flags, MyFlags::foo | MyFlags::bar);
  EXPECT_EQ(flags, MyFlags::daz);
  toggleMask(flags, MyFlags::daz);
  EXPECT_EQ(flags, MyFlags::none);
  toggleMask(flags, MyFlags::foo);
  EXPECT_EQ(flags, MyFlags::foo);
}

TEST(EnumFlags, toString) {  // NOLINT
  MyFlags flags = MyFlags::foo | MyFlags::bar;
  EXPECT_EQ(toPretty(flags), "{foo (=1), bar (=2), } (=3)");
  auto str = toStrings(flags);
  ASSERT_EQ(str.size(), 2u);
  EXPECT_EQ(str[0], "foo");
  EXPECT_EQ(str[1], "bar");
}

// Enum definition in global scope.
FARM_ENUMFLAGS_WITHOUT_OSTREAM_DEF(Bike, uint8_t, (frontWheel, backWheel));

struct Garage {
  // Enum alias can be in class scope
  FARM_ENUMFLAGS_ALIAS(Bike);
};

TEST(EnumTest, nestedEnumFlags) {  // NOLINT
  Garage::Bike bike{};
  bool is_set = trySetFlagFromString(bike, "frontWheel");
  ASSERT_TRUE(is_set);
  EXPECT_EQ(bike, Garage::Bike::frontWheel);
}
