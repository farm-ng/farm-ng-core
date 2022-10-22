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

#include "farm_ng/core/enum/enum_flags.h"

#include <gtest/gtest.h>

#include <sstream>

FARM_ENUMFLAGS(MyFlags, uint32_t, (foo, bar, daz));

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
  std::string_view type_name = getTypeName(MyFlags());
  EXPECT_EQ(type_name, "MyFlags");
  std::array<std::string_view, 3> strings = getNames(MyFlags());
  EXPECT_EQ(strings[0], "foo");
  EXPECT_EQ(strings[1], "bar");
  EXPECT_EQ(strings[2], "daz");
  std::string_view string_of_strings = getStringOfNames(MyFlags());
  EXPECT_EQ(string_of_strings, std::string("foo, bar, daz"));
  std::array<uint32_t, 3> values = getValues(MyFlags());
  EXPECT_EQ(values[0], 1);
  EXPECT_EQ(values[1], 2);
  EXPECT_EQ(values[2], 4);
  EXPECT_EQ(getCount(MyFlags()), 3);
}
