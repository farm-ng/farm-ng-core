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

#include "farm_ng/core/logging/logger.h"

#include <tl/expected.hpp>

#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace farm_ng {

/// An error, meant to be used with `Expected`.
struct Error {
  /// A list (or stack) of error details.
  std::vector<ErrorDetail> details;
};

std::ostream& operator<<(std::ostream& os, Error const& error);

struct Success {};

template <class TT, class TE = Error>
using Expected = tl::expected<TT, TE>;

namespace details {

template <class TT, class TE>
struct UnwrapImpl<tl::expected<TT, TE>> {
  static auto impl(
      tl::expected<TT, TE>&& wrapper,
      char const* wrapper_cstr,
      ::farm_ng::ErrorDetail detail) -> decltype(*wrapper) {
    if (!bool(wrapper)) {
      FARM_IMPL_LOG_PRINTLN(
          "[FARM_UNWRAP failed in {}:{}]", detail.file, detail.line);
      FARM_IMPL_LOG_PRINTLN(
          "expected type `{}` does not contain a valid value", wrapper_cstr);
      if (!detail.msg.empty()) {
        ::fmt::print(stderr, "{}", detail.msg);
      }
      ::fmt::print(stderr, "{}", wrapper.error());
      FARM_IMPL_ABORT();
    }
    return *wrapper;
  }
};
}  // namespace details
}  // namespace farm_ng

// You might want to disable callstacks in tests where one wishes to compare
// exact strings of error messages.
#ifdef FARM_ERROR_CALLSTACK_DISABLED

#define FARM_ERROR_DETAIL(...)                                \
  ::farm_ng::ErrorDetail {                                    \
    .file = "n/a", .line = 0, .msg = FARM_FORMAT(__VA_ARGS__) \
  }

#else

#define FARM_ERROR_DETAIL(...)                                          \
  ::farm_ng::ErrorDetail {                                              \
    .file = __FILE__, .line = __LINE__, .msg = FARM_FORMAT(__VA_ARGS__) \
  }
#endif

#define FARM_ERROR_REPORT(cstr, ...)                      \
  ::farm_ng::Error {                                      \
    .details = { FARM_ERROR_DETAIL(cstr, ##__VA_ARGS__) } \
  }

#define FARM_UNEXPECTED(cstr, ...) \
  ::tl::make_unexpected(FARM_ERROR_REPORT(cstr, ##__VA_ARGS__))

/// Assigns `*expression` to `var` of `Type`, but returns error if there is
/// one.
#define FARM_TRY(Type, var, expression)                     \
  auto maybe##var = (expression);                           \
  if (!maybe##var) {                                        \
    auto error = maybe##var.error();                        \
    error.details.emplace_back(                             \
        FARM_ERROR_DETAIL("FARM_TRY propagated error.\n")); \
    return ::tl::make_unexpected(error);                    \
  }                                                         \
  Type var = ::std::move(*maybe##var);

#define FARM_TRY_ASSERT(condition, ...)                                  \
  if (!(condition)) {                                                    \
    return FARM_UNEXPECTED(                                              \
        "bool({}) not true.\n{}", #condition, FARM_FORMAT(__VA_ARGS__)); \
  }                                                                      \
  do {                                                                   \
  } while (false)

namespace farm_ng {
template <class TT, class TE = Error>
std::optional<TT> fromExpected(Expected<TT, TE> expected) {
  return expected ? std::optional<TT>(std::move(*expected)) : std::nullopt;
}

template <class TT>
Expected<TT> fromOptional(std::optional<TT> optional) {
  return optional ? Expected<TT>(std::move(*optional))
                  : FARM_UNEXPECTED("std::nullopt");
}

}  // namespace farm_ng
