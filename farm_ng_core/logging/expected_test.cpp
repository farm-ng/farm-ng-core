// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng_core/logging/expected.h"

#include "farm_ng_core/logging/logger.h"

#include <gtest/gtest.h>
#include <optional>

using namespace farm_ng_core;

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
    return FNG_ERROR("a - error");
  }
  return A{.a = "a"};
}

Expected<Ab> makeAb(bool a_error, bool b_error) {
  FNG_TRY(A a, makeA(a_error));

  if (b_error) {
    return FNG_ERROR("b - {}", "error");
  }
  Ab ab;
  ab.a = a;
  ab.b = "bb";
  return ab;
}

Expected<Abc> makeAbc(bool a_error, bool b_error, bool c_error) {
  FNG_TRY(Ab ab, makeAb(a_error, b_error));

  if (c_error) {
    return FNG_ERROR("c - error - {}", 42);
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
    error.details.push_back(FNG_ERROR_DETAIL("a - error"));
  } else {
    abc.ab.a.a = "aaa";
  }

  if (b_error) {
    error.details.push_back(FNG_ERROR_DETAIL("b - error"));
  } else {
    abc.ab.b = "BB";
  }
  if (c_error) {
    error.details.push_back(FNG_ERROR_DETAIL("c - error"));
  } else {
    abc.c = "cCc";
  }

  if (!error.details.empty()) {
    return tl::unexpected(error);
  }
  return abc;
}

Expected<A> sum(Expected<A> maybe_left, Expected<A> maybe_right) {
  FNG_TRY(A lhs, maybe_left);
  FNG_TRY(A rhs, maybe_right);

  A s;
  s.a = lhs.a + rhs.a;
  return s;
}

TEST(expected, success) {
  Expected<Abc> abc = makeAbc(false, false, false);
  FNG_CHECK(abc);

  abc = makeAbcAtOnce(false, false, false);
  FNG_CHECK(abc);
}

TEST(expected, single_error) {
  Expected<Abc> abc = makeAbc(true, false, true);
  FNG_CHECK(!abc);
  FNG_CHECK_EQ(abc.error().details.size(), 1);

  abc = makeAbc(false, true, false);
  FNG_CHECK(!abc);
  FNG_CHECK_EQ(abc.error().details.size(), 1);

  abc = makeAbc(false, false, true);
  FNG_CHECK(!abc);
  FNG_CHECK_EQ(abc.error().details.size(), 1);
}

TEST(expected, multi_error) {
  auto abc = makeAbcAtOnce(true, false, true);
  FNG_CHECK(!abc);
  FNG_CHECK_EQ(abc.error().details.size(), 2);

  Expected<A> a_good = makeA(false);
  FNG_CHECK(a_good);
  Expected<A> a_bad = makeA(true);
  FNG_CHECK(!a_bad);

  auto s = sum(a_good, a_bad);
  FNG_CHECK(!s);
}

TEST(expected, unwrap) {
  EXPECT_DEATH(
      {
        Expected<Abc> abc = makeAbc(true, false, true);

        Abc f = FNG_UNWRAP(abc);
      },
      "expected type `abc` does not contain a valid.*a - error");

  EXPECT_DEATH(
      {
        Expected<Abc> abc = makeAbcAtOnce(true, false, true);

        Abc f = FNG_UNWRAP(abc);
      },
      "expected type `abc` does not contain a valid.*a - error.*c - error");
}

TEST(expected, optional) {
  Expected<double> expected_double = 3;
  std::optional<double> optional_double = fromExpected(expected_double);
  FNG_CHECK(optional_double);
  FNG_CHECK_EQ(FNG_UNWRAP(optional_double), 3);

  expected_double = FNG_ERROR("error");
  optional_double = fromExpected(expected_double);
  FNG_CHECK(!optional_double);

  std::optional<int> optional_int = 42;
  Expected<int> expected_int = fromOptional(optional_int);
  FNG_CHECK(expected_int);
  FNG_CHECK_EQ(FNG_UNWRAP(expected_int), 42);

  optional_int = std::nullopt;
  expected_int = fromOptional(optional_int);
  FNG_CHECK(!expected_int);

  EXPECT_DEATH(
      {
        std::optional<int> optional_int;
        Expected<int> expected_int = fromOptional(optional_int);
        FNG_UNWRAP(expected_int);
      },
      "expected type `expected_int` does not contain a valid.*std::nullopt");
}

TEST(expected, fancy_error) {
  struct MyFancyError : Error {
    MyFancyError(Error error) : Error(error) {}
    int diagnostics;
  };
  Expected<double, MyFancyError> with_fancy_error = FNG_ERROR("failed");
  with_fancy_error.error().diagnostics = 42;
  FNG_CHECK(!with_fancy_error);

  using FancyExpected = Expected<double, MyFancyError>;
  EXPECT_DEATH(
      {
        FancyExpected with_fancy_error = FNG_ERROR("failed");
        with_fancy_error.error().diagnostics = 42;
        FNG_UNWRAP(with_fancy_error);
      },
      "expected type `with_fancy_error` does not contain a valid.*failed");
}
