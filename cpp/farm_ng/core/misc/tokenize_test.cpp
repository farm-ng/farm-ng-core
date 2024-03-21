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

#include "farm_ng/core/misc/tokenize.h"

#include <farm_ng/core/logging/expected.h>
#include <gtest/gtest.h>

TEST(tokenize, smoke) {
  using namespace farm_ng;

  std::vector<std::string> tokens = tokenize("foo bar");
  FARM_ASSERT_EQ(tokens.size(), 2);
  FARM_ASSERT_EQ(tokens[0], "foo");
  FARM_ASSERT_EQ(tokens[1], "bar");

  tokens = tokenize(",foo,,bar,", ',');
  FARM_ASSERT_EQ(tokens.size(), 5);
  FARM_ASSERT_EQ(tokens[0], "");
  FARM_ASSERT_EQ(tokens[1], "foo");
  FARM_ASSERT_EQ(tokens[2], "");
  FARM_ASSERT_EQ(tokens[3], "bar");
  FARM_ASSERT_EQ(tokens[4], "");

}  // NOLINT
