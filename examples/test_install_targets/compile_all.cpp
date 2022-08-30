
#include <fng_core/enum/enum.h>
#include <fng_core/enum/enum_flags.h>
#include <fng_core/logging/expected.h>
#include <fng_core/logging/format.h>
#include <fng_core/logging/logger.h>
#include <fng_core/utils/erase.h>
#include <fng_core/utils/filesystem.h>
#include <fng_core/utils/stopwatch.h>
#include <fng_core/utils/tokenize.h>
#include <fng_core/utils/void.h>

using namespace fng_core;

namespace fng_core {
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

}  // namespace fng_core

FNG_ENUM(Fruits, size_t, ((apple, 2), banana, pear));
FNG_ENUMFLAGS_WITHOUT_OSTREAM(MyFlags, uint32_t, (foo, bar, daz));

int main() {
  Fruits fruit = Fruits::apple;
  std::stringstream out;
  out << fruit;

  MyFlags flags = MyFlags::foo | MyFlags::daz;
  setMask(flags, MyFlags::bar);

  Expected<Abc> abc = makeAbc(true, false, true);

    std::optional<int> maybe_foo;
  maybe_foo = 2;
  int foo = FNG_UNWRAP(maybe_foo);
  FNG_CHECK_EQ(foo, 2);
  FNG_UNWRAP(maybe_foo) = 1;
  FNG_CHECK_EQ(*maybe_foo, 1);
}