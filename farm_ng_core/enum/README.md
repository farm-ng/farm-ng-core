![CI](https://github.com/facebookincubator/FNG_ENUM_WITHOUT_OSTREAM/workflows/CI/badge.svg)

# FNG_ENUM_WITHOUT_OSTREAM

FNG_ENUM_WITHOUT_OSTREAM is a small c++ macro library to add compile-time introspection to c++
enum classes.

## Examples

For example ``FNG_ENUM_WITHOUT_OSTREAM(FooBar, int, (foo, bar));`` defines an enum class

```cpp
enum class FooBar : int {
  foo,
  bar
};
```

with the following free functions:

```cpp
// Returns corresponding string of given value
//
// Preconditions: value must be a valid enum value, i.e. FooBar::foo, or
//                FooBar::bar.
//
std::string toString(FooBar value);

// Returns corresponding string view of given value
//
// Preconditions: value must be a valid enum value, i.e. FooBar::foo, or
//                FooBar::bar.
//
string_view toStringView(FooBar value);

// Returns pretty string representation of given value
//
// Preconditions: value must be a valid enum value, i.e. FooBar::foo, or
//                FooBar::bar.
//
std::string toPretty(FooBar value);

// Sets enum given corresponding string, if string is valid (i.e. "foo" or
// "bar"). Returns false otherwise.
//
bool trySetFromString(FooBar& value, std::string str);

// Return count of enum type. First argument is needed for ADL only.
//
constexpr size_t getCount(FooBar) {
  return 2;
}

// Return string views of enum type. First argument is needed for ADL only.
std::array<string_view, 2> getStrings(FooBar) {
  return {"foo", "bar"};
}

// Return string of enum names. First argument is needed for ADL only.
std::array<string_view, 2> getStrings(FooBar) {
  return "foo, bar";
}

// Return values of enum type. First argument is needed for ADL only.
constexpr std::array<int, 2> getValues(FooBar) {
  return {0, 1};
}

// Returns the position of enum value in the enum class. This is the inverse
// of `getValues(FooBar)[i]`.
constexpr size_t getPosition(FooBar value) {
  switch(value) {
    case FooBar::foo: { return 0; }
    case FooBar::bar: { return 1; }
  }
}

// Return string representation of type name. First argument is needed for
// ADL only.
string_view getTypeName(FooBar) {
  return "FooBar";
}
```

## Requirements
FNG_ENUM_WITHOUT_OSTREAM is a c++ macro library.
* CMake configuration and build steps are tested on MacOS and Linux.
* It depends on the BOOST preprocessor library.
* It depends on a string_view implementation: either std::string_view (c++17),
  or fmt::string_view from the [{fmt}](https://github.com/fmtlib/fmt) library.
* FNG_ENUM_WITHOUT_OSTREAM tests depend on [googletest](https://github.com/google/googletest.git)
  which are pulled in through git sub-modules.
* The FNG_ENUM_WITHOUT_OSTREAM CliExample depends on
  [CLI11](https://github.com/CLIUtils/CLI11.git).

## Building FNG_ENUM_WITHOUT_OSTREAM

In the following, we detail how to build FNG_ENUM_WITHOUT_OSTREAM for Ubuntu 18.04.

Install BOOST preprocessor headers:

```
sudo apt install libboost-system-dev
```

Install CLI11 if you want to build the CliExample application:

```
git clone https://github.com/CLIUtils/CLI11.git
cd CLI11
git checkout v1.9.0
mkdir build
cd build
cmake .. -DCLI11_BUILD_TESTS=OFF
make -j2
sudo make install
```

Now checkout FNG_ENUM_WITHOUT_OSTREAM from the repo and build the FNG_ENUM_WITHOUT_OSTREAM tests and example:

```
git clone https://github.com/facebook/FNG_ENUM_WITHOUT_OSTREAM
cd FNG_ENUM_WITHOUT_OSTREAM
git submodule update --init --recursive
mkdir build
cd build
cmake ..
make -j2
```

## Join the FNG_ENUM_WITHOUT_OSTREAM community
* file issues here: [https://github.com/facebook/FNG_ENUM_WITHOUT_OSTREAM/issues](https://github.com/facebook/FNG_ENUM_WITHOUT_OSTREAM/issues)

See the [CONTRIBUTING](CONTRIBUTING.md) file for how to help out.


## License
FNG_ENUM_WITHOUT_OSTREAM is MIT licensed, as found in the LICENSE file.
