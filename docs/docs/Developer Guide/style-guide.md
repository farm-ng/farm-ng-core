---
sidebar_position: 1
---

# C++ Style Guide

This is the c++ style guide for farm-ng-coe.

## Coding standard

We aim for using modern c++. A general good resource for best practice are the
https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md.


## Code formatting

We use clang-format. See ``.clang-format`` file in the root directory for
details.

## Headers

Each header file shall start with a ``#pragma once`` statement (i.e. right after
the copyright / licensing statement).

### Header include order

Header includes should be ordered in groups, proceeding from "local" to "global".
This [helps ensure](https://stackoverflow.com/questions/2762568) that local
headers are self-sufficient, without implicit dependencies.

Example:

```cpp
// 1. Corresponding .* file, if we are in a  *.cpp file.
#include "farm_ng/core/prototools/proto_log_writer.h"


// 2. first party
#include "farm_ng/core/logging/expected.h"
#include "farm_ng/core/logging/logger.h"

// 3. third-party
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>

// 4. c++ standard library and posix
#include <filesystem>
#include <fstream>
```

First party headers are included with ``"path/from/root/foo.h"``, while
third-party packages are included with ``<package_name/module/file.h>``. This
include order (1., 2., 3., 4.) convention is enforced by `clang-format` if
``"brackets"`` (1., 2.) versus ``<brackets>`` (3., 4.) are used accordingly.


Notes:
 - In downstream dependencies, open source libraries such as "farm-ng-core"
and "sophus" are considered third-party and included with ``<brackets>``.
 - When using C library header one shall prefer C++ library version, over the
C-library version: ``#include <cmath>`` instead of ``#include <math.h>``.


## Naming convention

Most naming conventions are enforced by clang-tidy. See ``.clang-tidy`` in the
root for details.

### ``snake_case``


Variable names are typically written in ``snake_case``, e.g.:

```cpp
  int count = 0;
  std::vector<double> floating_point_numbers = {0.1, 0.3};
  size_t num_numbers = floating_point_numbers.size();
```

``snake_case`` is also used for namespaces, see below.


### ``trailing_snake_case_``

Private member variables of classes are written in ``trailing_snake_case_``,
e.g.:

```cpp
    class Foo {
     public:
      /* details */

     private:
      std::vector<int> integers_;
      Bar internal_state_;
    }
```




### ``PascalCase``

Type names, including class names, struct names and typedefs, are written in
``PascalCase``:

```cpp
    struct Foo {};

    class Bar {
      /* details */
    };

    using FooBarVariant = std::variant<Foo, Bar>;
```

### ``lowerCamelCase``

Function, including free functions and class methods are written in
``lowerCamelCase``:

```cpp
    void solveSubProblem(const Input& input);

    class Bar {
     public:
      DebugOutput debugOutput();
    };

    Foo fooFromBar(const Bar& bar);
```


Notes:
 - By default, don't use ``getPrefixes`` for getter function, e.g. use
   ``debugOutput`` and not ``getDebugOutput``.
 - Lambda expressions names are considered variables and hence we use
``snake_case``:

```cpp
    auto solve_sub_problem = [&input]() { return solveSubProblem(input); }
```


### ``kLowerCamelCase``

``kLowerCamelCase`` is used for global and static class constants, constexpr
constants and value class templates see below.


### ``UPPER_CASE``

Macros only. See below.

### Acronyms

Acronyms and contractions count as one word. That is we have ``SlamResult``,
``HttpRequest`` and ``Se3Group`` (and not ``SLAMResult``, ``HTTPRequest``,
``SE3Group``) in UpperCamelCase and ``slam_result`` and ``http_request`` in
snake_case.


## Classes and Structs


While C++ allows for a great flexibility, we impose some guidelines when to use
structs vs. classes.

For POD-like collections we use the struct keyword::

```cpp
    struct Collection {
      std::vector<int> integers;
      std::string description;
    };
```

Such structs must not have private members. They may or may not have have member
functions, and constructors. For the public member variables we use
``snake_case`` naming.

For entities with non-trivial type invariant, we use the class keyword::

```cpp
    template <class T>
    class UnitVector3 {
     public:
      // Precondition: v must be of unit length.
      static UnitVector3 fromUnitVector(const Eigen::Matrix<T, 3, 1>& v) {
        using std::abs;
        FARM_ASSERT_LE((v.squaredNorm() - T(1.0)),
                         Sophus::kEpsilon<TT>);
        UnitVector3 unit_vector;
        unit_vector.vector_ = v;
        return unit_vector;
      }


      static UnitVector3 fromVectorAndNormalize(
          const Eigen::Matrix<T, 3, 1>& v) {
        return fromUnitVector(v.normalized());
      }

      const Eigen::Matrix<T, 3, 1>& vector() const { return vector_; }

     private:
      UnitVector3() {}

      // Class invariant: v_ is of unit length.
      Eigen::Matrix<T, 3, 1> vector_;
    };
```


Classes shall start with all public member (constructors, destructors, member
functions etc.) then followed by private members (member functions then member
variables). Classes shall not have any public member variables. Use public
accessors/mutators instead:

```cpp
    class MyClass {
    public:
      /* details */

      // accessor
      [[nodiscard]] const std::vector<double>& rawValues() const {
        return raw_values_;
      }

      // mutator
      std::vector<double>& mutRawValues() {
        return raw_values_;
      }


    private:
      std::vector<double> raw_values_;
    };
```

Notes about class/struct methods:

 - Do not prefix an accessor with ``get``.
 - It is good practice to annotate a constant ref accessor with
   ``[[nodiscard]]``.
 - Consider prefixing mutators and other non-const methods with
   ``mut``. This is inspired by the ``mut`` postfix convention in rust (e.g.
   https://doc.rust-lang.org/std/vec/struct.Vec.html#method.last_mut) and it is
   similar to the rational of the introduction of ``cbegin/cend`` to the c++
   standard (as motivated here
   https://stackoverflow.com/questions/12001410/what-is-the-reason-behind-cbegin-cend).
 - For implementation details, hence code not part of a public API, such as
   trait classes, ceres::Cost functors, Impl classes (e.g. when using the Pimpl
   idiom: http://bitboom.github.io/pimpl-idiom), it is fine to a have a
   class/struct with all public member variables.


## Enums


Prefer use enum classes defined through the FARM_ENUM macro.

```cpp
    FARM_ENUM(VizMaterialType, (ambient, diffuse, phong));
```

Note: we use ``snake_case`` for enum value names, such that the
corresponding strings are more user-friendly, e.g. when passing
in values through CLI11:

```cpp
    std::string example_input = "ambient";
    VizMatrialType material;
    FARM_ASSERT(trySetFromString(material, example_input));
```

## Constants

For global and static class constants, we use the ``kLowerCamelCase`` naming
conventions. Examples::

```cpp
    constexpr double kPi = 3.14159265359;

    class Variable5Dof {
     public:
      static constexpr int kNumDegreesOfFreedom = 5;

      /* details */
    };
```

## Macros

Avoid using macros, especially if the same behaviors can be achieved through
constants or C++ templates.

For all marcos we use the ``FARM_UPPER_CASE`` naming style. In particular,
all farm-ng macro names shall be prefixed by ``FARM_`` (and for sister open
source libraries accordingly, ``SOPHUS_`` for Sophus etc.).
Example:

```cpp
    #define FARM_FORMAT(cstr, ...)                             \
        /* FMT_STRING is defined in the <fmt/format.h> header */  \
        ::fmt::format(FMT_STRING(cstr), __VA_ARGS__)
```

Note:
 - Marcos provided by farm-ng-core in generally provide functionality not
   supported otherwise, such as line number log output for ASSERT macros,
   to/from string conversions for enums etc.

## Namespaces

For namespace hierarchy, we believe more is less. That is most of the code shall
be defined within the top-level namespace, e.g. ``farm-ng`` for the
farm-ng-core, ``sophus`` for Sophus library, etc.

For implementation details (e.g. in header only libraries), we use the
``farm-ng::details`` namespace.

All namespace names are in ``snake_case``.


## Templates

Use reasonable. Compile time matters too. Non-type template parameters use
``kCamelCae`` while type parameters use ``TCamelCase``.

```cpp
template <class TScalar, int kRows, int kCols>
class MaxMetric<Eigen::Matrix<TScalar, kRows, kCols>> {
 public:
  using Scalar = TScalar;

  ...
}
```


## `class` versus `typename` in templates

tldr: We use `class`.

Both keywords are interchangeable in this context:

```cpp
template<class T>
class Foo {
}
```

versus

```cpp
template<typename T>
class Foo {
};
```

[*](https://stackoverflow.com/a/2024173).

We use the `class` key name in (class, function, variable) templates
always

 - to be consistent,
 - `class` is less letters to type than `typename`,
 - possibly easier to parse in complex expressions
   (`class` looks less similar to `template` keyword).
