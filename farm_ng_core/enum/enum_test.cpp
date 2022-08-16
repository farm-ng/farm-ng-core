// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <farm_ng_core/enum/enum.h>

#include <gtest/gtest.h>

#include <sstream>

FNG_ENUM(Fruits, size_t, ((apple, 2), banana, pear));

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
FNG_ENUM_DEF_O(Autos, int32_t, (van, truck, sedan));

struct Garage {
  // Enum alias can be in class scope
  FNG_ENUM_ALIAS(Autos);
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
  std::string_view typeName = getTypeName(Fruits());
  EXPECT_EQ(typeName, "Fruits");
  std::array<std::string_view, 3> strings = getStrings(Fruits());
  EXPECT_EQ(strings[0], "apple");
  EXPECT_EQ(strings[1], "banana");
  EXPECT_EQ(strings[2], "pear");
  std::string_view stringOfNames = getStringOfNames(Fruits());
  EXPECT_EQ(stringOfNames, std::string("apple, banana, pear"));
  std::array<size_t, 3> values = getValues(Fruits());
  EXPECT_EQ(values[0], 2);
  EXPECT_EQ(values[1], 3);
  EXPECT_EQ(values[2], 4);
  EXPECT_EQ(getCount(Fruits()), 3);
}
