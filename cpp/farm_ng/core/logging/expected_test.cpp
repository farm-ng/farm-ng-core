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

#include "farm_ng/core/logging/expected.h"

#include "farm_ng/core/logging/logger.h"

#include <gtest/gtest.h>

#include <optional>

using namespace farm_ng;

namespace farm_ng {
struct A {
  std::string a;
};

struct Ab {
  A a;
  std::string b;
};

struct Abc {
  Ab ab;
  std::string c;
};

Expected<A> makeA(bool a_error) {
  if (a_error) {
    return FARM_ERROR("a - error");
  }
  return A{.a = "a"};
}

Expected<Ab> makeAb(bool a_error, bool b_error) {
  FARM_TRY(A a, makeA(a_error));

  if (b_error) {
    return FARM_ERROR("b - {}", "error");
  }
  Ab ab;
  ab.a = a;
  ab.b = "bb";
  return ab;
}

Expected<Abc> makeAbc(bool a_error, bool b_error, bool c_error) {
  FARM_TRY(Ab ab, makeAb(a_error, b_error));

  if (c_error) {
    return FARM_ERROR("c - error - {}", 42);
  }
  Abc abc;
  abc.ab = ab;
  abc.c = "ccc";
  return abc;
}

Expected<Abc> makeAbcAtOnce(bool a_error, bool b_error, bool c_error) {
  Abc abc;
  Error error;

  if (a_error) {
    error.details.push_back(FARM_ERROR_DETAIL("a - error"));
  } else {
    abc.ab.a.a = "aaa";
  }

  if (b_error) {
    error.details.push_back(FARM_ERROR_DETAIL("b - error"));
  } else {
    abc.ab.b = "BB";
  }
  if (c_error) {
    error.details.push_back(FARM_ERROR_DETAIL("c - error"));
  } else {
    abc.c = "cCc";
  }

  if (!error.details.empty()) {
    return tl::unexpected(error);
  }
  return abc;
}

Expected<A> sum(Expected<A> maybe_left, Expected<A> maybe_right) {
  FARM_TRY(A lhs, maybe_left);
  FARM_TRY(A rhs, maybe_right);

  A s;
  s.a = lhs.a + rhs.a;
  return s;
}
}  // namespace farm_ng

TEST(expected, success) {
  Expected<Abc> abc = makeAbc(false, false, false);
  FARM_CHECK(abc);

  abc = makeAbcAtOnce(false, false, false);
  FARM_CHECK(abc);
}

TEST(expected, single_error) {
  Expected<Abc> abc = makeAbc(true, false, true);
  FARM_CHECK(!abc);
  FARM_CHECK_EQ(abc.error().details.size(), 1);

  abc = makeAbc(false, true, false);
  FARM_CHECK(!abc);
  FARM_CHECK_EQ(abc.error().details.size(), 1);

  abc = makeAbc(false, false, true);
  FARM_CHECK(!abc);
  FARM_CHECK_EQ(abc.error().details.size(), 1);
}

TEST(expected, multi_error) {
  auto abc = makeAbcAtOnce(true, false, true);
  FARM_CHECK(!abc);
  FARM_CHECK_EQ(abc.error().details.size(), 2);

  Expected<A> a_good = makeA(false);
  FARM_CHECK(a_good);
  Expected<A> a_bad = makeA(true);
  FARM_CHECK(!a_bad);

  auto s = sum(a_good, a_bad);
  FARM_CHECK(!s);
}

TEST(expected, unwrap) {
  EXPECT_DEATH(
      {
        Expected<Abc> abc = makeAbc(true, false, true);

        Abc f = FARM_UNWRAP(abc);
      },
      "expected type `abc` does not contain a valid.*a - error");

  EXPECT_DEATH(
      {
        Expected<Abc> abc = makeAbcAtOnce(true, false, true);

        Abc f = FARM_UNWRAP(abc);
      },
      "expected type `abc` does not contain a valid.*a - error.*c - error");
}

TEST(expected, optional) {
  Expected<double> expected_double = 3;
  std::optional<double> optional_double = fromExpected(expected_double);
  FARM_CHECK(optional_double);
  FARM_CHECK_EQ(FARM_UNWRAP(optional_double), 3);

  expected_double = FARM_ERROR("error");
  optional_double = fromExpected(expected_double);
  FARM_CHECK(!optional_double);

  std::optional<int> optional_int = 42;
  Expected<int> expected_int = fromOptional(optional_int);
  FARM_CHECK(expected_int);
  FARM_CHECK_EQ(FARM_UNWRAP(expected_int), 42);

  optional_int = std::nullopt;
  expected_int = fromOptional(optional_int);
  FARM_CHECK(!expected_int);

  EXPECT_DEATH(
      {
        std::optional<int> optional_int;
        Expected<int> expected_int = fromOptional(optional_int);
        FARM_UNWRAP(expected_int);
      },
      "expected type `expected_int` does not contain a valid.*std::nullopt");
}

TEST(expected, fancy_error) {
  struct MyFancyError : Error {
    MyFancyError(Error error) : Error(error) {}
    int diagnostics;
  };
  Expected<double, MyFancyError> with_fancy_error = FARM_ERROR("failed");
  with_fancy_error.error().diagnostics = 42;
  FARM_CHECK(!with_fancy_error);

  using FancyExpected = Expected<double, MyFancyError>;
  EXPECT_DEATH(
      {
        FancyExpected with_fancy_error = FARM_ERROR("failed");
        with_fancy_error.error().diagnostics = 42;
        FARM_UNWRAP(with_fancy_error);
      },
      "expected type `with_fancy_error` does not contain a valid.*failed");
}
