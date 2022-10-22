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

#include "farm_ng/core/enum/enum.h"

#include <gtest/gtest.h>

#include <sstream>

FARM_ENUM(Fruits, size_t, ((apple, 2), banana, pear));

TEST(EnumTest, toString) {
  Fruits fruit = Fruits::apple;
  std::stringstream out;
  out << fruit;
  EXPECT_EQ(out.str(), "apple (=2)");

  fruit = Fruits::banana;
  std::stringstream out2;
  out2 << fruit;
  EXPECT_EQ(out2.str(), "banana (=3)");

  fruit = Fruits::pear;
  std::stringstream out3;
  out3 << fruit;
  EXPECT_EQ(out3.str(), "pear (=4)");
}

// Enum definition in global scope.
FARM_ENUM_DEF(Autos, int32_t, (van, truck, sedan));

struct Garage {
  // Enum alias can be in class scope
  FARM_ENUM_ALIAS(Autos);
};

TEST(EnumTest, trySetFromString) {
  Garage::Autos car;
  bool is_set = trySetFromString(car, "sedan");
  ASSERT_TRUE(is_set);

  std::stringstream out;
  out << car;
  EXPECT_EQ(out.str(), "sedan (=2)");
}

TEST(EnumTest, StringsAndValues) {
  std::string_view type_name = getTypeName(Fruits());
  EXPECT_EQ(type_name, "Fruits");
  std::array<std::string_view, 3> strings = getStrings(Fruits());
  EXPECT_EQ(strings[0], "apple");
  EXPECT_EQ(strings[1], "banana");
  EXPECT_EQ(strings[2], "pear");
  std::string_view string_of_names = getStringOfNames(Fruits());
  EXPECT_EQ(string_of_names, std::string("apple, banana, pear"));
  std::array<size_t, 3> values = getValues(Fruits());
  EXPECT_EQ(values[0], 2);
  EXPECT_EQ(values[1], 3);
  EXPECT_EQ(values[2], 4);
  EXPECT_EQ(getCount(Fruits()), 3);
}
