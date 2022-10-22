// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

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

TEST(EnumTest, getAllFruit) {
  const std::array<Fruits, 3> all_vals = getAll(Fruits());
  EXPECT_EQ(all_vals[0], Fruits::apple);
  EXPECT_EQ(all_vals[1], Fruits::banana);
  EXPECT_EQ(all_vals[2], Fruits::pear);
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

TEST(EnumTest, getAllAutos) {
  const std::array<Garage::Autos, 3> all_vals = getAll(Garage::Autos());
  EXPECT_EQ(all_vals[0], Garage::Autos::van);
  EXPECT_EQ(all_vals[1], Garage::Autos::truck);
  EXPECT_EQ(all_vals[2], Garage::Autos::sedan);
}

TEST(EnumTest, foreach) {
  {
    size_t count = 0;
    for (Fruits f : getAll(Fruits())) {
      ++count;
    }
    EXPECT_EQ(count, 3);
  }

  {
    size_t count = 0;
    for (Garage::Autos a : getAll(Garage::Autos())) {
      ++count;
    }
    EXPECT_EQ(count, 3);
  }
}
