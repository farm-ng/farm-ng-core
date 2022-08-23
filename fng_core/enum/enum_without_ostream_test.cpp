// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

// FNG_ENUM is a fork of MY_ENUM.
//
// https://github.com/facebookincubator/MY_ENUM/
//
// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "fng_core/enum/enum_without_ostream.h"

#include <gtest/gtest.h>

FNG_ENUM_WITHOUT_OSTREAM(Fruits, uint8_t, ((apple, 1), (banana, 3), pear));

TEST(EnumTest, defaultDeath) {
  Fruits fruit{};
  // We must initialize Fruits to a valid explicit value; the default (=0) would
  // not work here.
  EXPECT_DEATH(toString(fruit), "");
  EXPECT_DEATH(toPretty(fruit), "");
}

TEST(EnumTest, toString) {
  Fruits fruit = Fruits::apple;
  EXPECT_EQ(toString(fruit), "apple");
  EXPECT_EQ(toPretty(fruit), "apple (=1)");
  std::string_view apple_view = toStringView(fruit);
  EXPECT_EQ(apple_view, "apple");

  Fruits fruit2 = Fruits::banana;
  EXPECT_EQ(toString(fruit2), "banana");
  EXPECT_EQ(toPretty(fruit2), "banana (=3)");

  fruit2 = Fruits::pear;
  EXPECT_EQ(toString(fruit2), "pear");
  EXPECT_EQ(
      toPretty(fruit2),
      "pear (=4)");  // since 3+1 is 4, and pear is the successor of banana.
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
  std::array<uint8_t, 3> values = getValues(Fruits());
  EXPECT_EQ(values[0], 1);
  EXPECT_EQ(values[1], 3);
  EXPECT_EQ(values[2], 4);
  EXPECT_EQ(getCount(Fruits()), 3);

  EXPECT_EQ(getPosition(Fruits(values[0])), 0);
  EXPECT_EQ(getPosition(Fruits(values[1])), 1);
  EXPECT_EQ(getPosition(Fruits(values[2])), 2);
}

// Enum definition in global scope.
FNG_ENUM_WITHOUT_OSTREAM_DEF(Autos, int, (van, truck, (sedan, 4)));

struct Garage {
  // Enum alias can be in class scope
  FNG_ENUM_ALIAS(Autos);
};

TEST(EnumTest, trySetFromString) {
  Garage::Autos car;
  bool is_set = trySetFromString(car, "van");
  EXPECT_EQ(car, Garage::Autos::van);
  is_set = trySetFromString(car, "truck");
  ASSERT_TRUE(is_set);
  EXPECT_EQ(car, Garage::Autos::truck);
  is_set = trySetFromString(car, "sedan");
  ASSERT_TRUE(is_set);
  EXPECT_EQ(car, Garage::Autos::sedan);
  is_set = trySetFromString(car, "bike");
  ASSERT_FALSE(is_set);
}

// FNG_ENUM_WITHOUT_OSTREAM(Foos, ((foo, 2), (foo_alias, 2)));
//
// Won't compile, but will trigger an error such as:
// "error: duplicate case value: 'foo' and 'foo_alias' both equal '2'".
