// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <farm_ng_core/enum/enum_flags.h>
#include <gtest/gtest.h>

#include <sstream>

FNG_ENUMFLAGS_O(MyFlags, uint32_t, (foo, bar, daz));

TEST(EnumTest, toString) {
  MyFlags flags = MyFlags::foo;
  std::stringstream out;
  out << flags;
  EXPECT_EQ(out.str(), "{foo (=1), } (=1)");
  flags = MyFlags::none;
  std::stringstream out2;
  out2 << flags;
  EXPECT_EQ(out2.str(), "{} (=0)");
  flags = MyFlags::bar | MyFlags::daz;
  std::stringstream out3;
  out3 << flags;
  EXPECT_EQ(out3.str(), "{bar (=2), daz (=4), } (=6)");
}

TEST(EnumFlags, StringsAndValues) {
  std::string_view typeName = getTypeName(MyFlags());
  EXPECT_EQ(typeName, "MyFlags");
  std::array<std::string_view, 3> strings = getNames(MyFlags());
  EXPECT_EQ(strings[0], "foo");
  EXPECT_EQ(strings[1], "bar");
  EXPECT_EQ(strings[2], "daz");
  std::string_view stringOfStrings = getStringOfNames(MyFlags());
  EXPECT_EQ(stringOfStrings, std::string("foo, bar, daz"));
  std::array<uint32_t, 3> values = getValues(MyFlags());
  EXPECT_EQ(values[0], 1);
  EXPECT_EQ(values[1], 2);
  EXPECT_EQ(values[2], 4);
  EXPECT_EQ(getCount(MyFlags()), 3);
}