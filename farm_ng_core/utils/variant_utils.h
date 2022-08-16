// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <variant>
namespace farm_ng_core {

/// https://en.cppreference.com/w/cpp/utility/variant/visit
template <class... Ts>
struct Overload : Ts... {
  using Ts::operator()...;
};

template <class... Ts>
Overload(Ts...) -> Overload<Ts...>;

/// https://en.cppreference.com/w/cpp/utility/variant/visit
template <class>
inline constexpr bool AlwaysFalse = false;

/// https://gist.github.com/s3rvac/d1f30364ce1f732d75ef0c89a1c8c1ef
template <class Variant, typename... Alternatives>
decltype(auto) visit_variant(
    Variant&& variant, Alternatives&&... alternatives) {
  return std::visit(
      Overload{std::forward<Alternatives>(alternatives)...},
      std::forward<Variant>(variant));
}

// https://stackoverflow.com/a/41171291
template <class T, class Variant>
struct HasType;

template <class T, class... Us>
struct HasType<T, std::variant<Us...>>
    : std::disjunction<std::is_same<T, Us>...> {};

template <class T, class Variant>
constexpr bool has_type_v = HasType<T, Variant>::value;

}  // namespace farm_ng_core
