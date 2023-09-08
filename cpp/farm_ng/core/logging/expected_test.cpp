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

auto makeA(bool a_error) -> Expected<A> {
  if (a_error) {
    return FARM_UNEXPECTED("a - error");
  }
  return A{.a = "a"};
}

auto makeAb(bool a_error, bool b_error) -> Expected<Ab> {
  FARM_TRY(A, a, makeA(a_error));

  if (b_error) {
    return FARM_UNEXPECTED("b - {}", "error");
  }
  Ab ab;
  ab.a = a;
  ab.b = "bb";
  return ab;
}

auto makeAbc(bool a_error, bool b_error, bool c_error) -> Expected<Abc> {
  FARM_TRY(Ab, ab, makeAb(a_error, b_error));

  if (c_error) {
    return FARM_UNEXPECTED("c - error - {}", 42);
  }
  Abc abc;
  abc.ab = ab;
  abc.c = "ccc";
  return abc;
}

auto makeAbcAtOnce(bool a_error, bool b_error, bool c_error) -> Expected<Abc> {
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

auto sum(Expected<A> maybe_left, Expected<A> maybe_right) -> Expected<A> {
  FARM_TRY(A, lhs, maybe_left);
  FARM_TRY(A, rhs, maybe_right);

  A s;
  s.a = lhs.a + rhs.a;
  return s;
}
}  // namespace farm_ng

TEST(expected, success) {  // NOLINT
  Expected<Abc> abc = makeAbc(false, false, false);
  FARM_ASSERT(abc);

  abc = makeAbcAtOnce(false, false, false);
  FARM_ASSERT(abc);
}

TEST(expected, single_error) {  // NOLINT
  Expected<Abc> abc = makeAbc(true, false, true);
  FARM_ASSERT(!abc);
  FARM_INFO("{}", abc.error());
  FARM_ASSERT_EQ(abc.error().details.size(), 3, "foo ");

  abc = makeAbc(false, true, false);
  FARM_ASSERT(!abc);
  FARM_ASSERT_EQ(abc.error().details.size(), 2);

  abc = makeAbc(false, false, true);
  FARM_ASSERT(!abc);
  FARM_ASSERT_EQ(abc.error().details.size(), 1);
}

TEST(expected, multi_error) {  // NOLINT
  auto abc = makeAbcAtOnce(true, false, true);
  FARM_ASSERT(!abc);
  FARM_ASSERT_EQ(abc.error().details.size(), 2);

  Expected<A> a_good = makeA(false);
  FARM_ASSERT(a_good);
  Expected<A> a_bad = makeA(true);
  FARM_ASSERT(!a_bad);

  auto s = sum(a_good, a_bad);
  FARM_ASSERT(!s);
}

TEST(expected, unwrap) {  // NOLINT
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

TEST(expected, optional) {  // NOLINT
  Expected<double> expected_double = 3;
  std::optional<double> optional_double = fromExpected(expected_double);
  FARM_ASSERT(optional_double);
  FARM_ASSERT_EQ(FARM_UNWRAP(optional_double), 3);

  expected_double = FARM_UNEXPECTED("error");
  optional_double = fromExpected(expected_double);
  FARM_ASSERT(!optional_double);

  std::optional<int> optional_int = 42;
  Expected<int> expected_int = fromOptional(optional_int);
  FARM_ASSERT(expected_int);
  FARM_ASSERT_EQ(FARM_UNWRAP(expected_int), 42);

  optional_int = std::nullopt;
  expected_int = fromOptional(optional_int);
  FARM_ASSERT(!expected_int);

  EXPECT_DEATH(
      {
        std::optional<int> optional_int;
        Expected<int> expected_int = fromOptional(optional_int);
        FARM_UNWRAP(expected_int);
      },
      "expected type `expected_int` does not contain a valid.*std::nullopt");
}

TEST(expected, fancy_error) {  // NOLINT
  struct MyFancyError : Error {
    MyFancyError(Error error) : Error(error) {}
    int diagnostics;
  };
  Expected<double, MyFancyError> with_fancy_error = FARM_UNEXPECTED("failed");
  with_fancy_error.error().diagnostics = 42;
  FARM_ASSERT(!with_fancy_error);

  using FancyExpected = Expected<double, MyFancyError>;
  EXPECT_DEATH(
      {
        FancyExpected with_fancy_error = FARM_UNEXPECTED("failed");
        with_fancy_error.error().diagnostics = 42;
        FARM_UNWRAP(with_fancy_error);
      },
      "expected type `with_fancy_error` does not contain a valid.*failed");
}
