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

#pragma once

#include <variant>
namespace farm_ng {

/// https://en.cppreference.com/w/cpp/utility/variant/visit
template <class... Ts>
struct Overload : Ts... {
  using Ts::operator()...;
};

template <class... Ts>
Overload(Ts...) -> Overload<Ts...>;

/// https://en.cppreference.com/w/cpp/utility/variant/visit
template <class>
inline bool constexpr AlwaysFalse = false;

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
bool constexpr has_type_v = HasType<T, Variant>::value;

}  // namespace farm_ng
