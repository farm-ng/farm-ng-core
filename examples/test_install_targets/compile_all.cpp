
// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <farm_ng/core/enum/enum.h>
#include <farm_ng/core/enum/enum_flags.h>
#include <farm_ng/core/logging/expected.h>
#include <farm_ng/core/logging/format.h>
#include <farm_ng/core/logging/logger.h>
#include <farm_ng/core/misc/erase.h>
#include <farm_ng/core/misc/filesystem.h>
#include <farm_ng/core/misc/stopwatch.h>
#include <farm_ng/core/misc/tokenize.h>
#include <farm_ng/core/misc/void.h>

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

}  // namespace farm_ng

FARM_ENUM(Fruits, size_t, ((apple, 2), banana, pear));
FARM_ENUMFLAGS_WITHOUT_OSTREAM(MyFlags, uint32_t, (foo, bar, daz));

int main() {
  Fruits fruit = Fruits::apple;
  std::stringstream out;
  out << fruit;

  MyFlags flags = MyFlags::foo | MyFlags::daz;
  setMask(flags, MyFlags::bar);

  Expected<Abc> abc = makeAbc(true, false, true);

  std::optional<int> maybe_foo;
  maybe_foo = 2;
  int foo = FARM_UNWRAP(maybe_foo);
  FARM_CHECK_EQ(foo, 2);
  FARM_UNWRAP(maybe_foo) = 1;
  FARM_CHECK_EQ(*maybe_foo, 1);
}
