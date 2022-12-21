
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

#include <farm_ng/core/enum/enum.h>
#include <farm_ng/core/enum/enum_flags.h>
#include <farm_ng/core/logging/expected.h>
#include <farm_ng/core/logging/format.h>
#include <farm_ng/core/logging/logger.h>
#include <farm_ng/core/misc/erase.h>
#include <farm_ng/core/misc/filesystem.h>
#include <farm_ng/core/misc/stopwatch.h>
#include <farm_ng/core/misc/tokenize.h>
#include <farm_ng/core/misc/uri.h>
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
    return FARM_UNEXPECTED("a - error");
  }
  return A{.a = "a"};
}

Expected<Ab> makeAb(bool a_error, bool b_error) {
  FARM_TRY(A a, makeA(a_error));

  if (b_error) {
    return FARM_UNEXPECTED("b - {}", "error");
  }
  Ab ab;
  ab.a = a;
  ab.b = "bb";
  return ab;
}

Expected<Abc> makeAbc(bool a_error, bool b_error, bool c_error) {
  FARM_TRY(Ab ab, makeAb(a_error, b_error));

  if (c_error) {
    return FARM_UNEXPECTED("c - error - {}", 42);
  }
  Abc abc;
  abc.ab = ab;
  abc.c = "ccc";
  return abc;
}

FARM_ENUM(Fruits, size_t, ((apple, 2), banana, pear));
FARM_ENUMFLAGS_WITHOUT_OSTREAM(MyFlags, uint32_t, (foo, bar, daz));
}  // namespace farm_ng

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
  FARM_ASSERT_EQ(foo, 2);
  FARM_UNWRAP(maybe_foo) = 1;
  FARM_ASSERT_EQ(*maybe_foo, 1);
}
