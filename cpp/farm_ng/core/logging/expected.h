// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "farm_ng/core/logging/logger.h"

#include <tl/expected.hpp>

#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace farm_ng {

struct Error {
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
      tl::expected<TT, TE>& wrapper,
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

#define FARM_ERROR_DETAIL(...)                                          \
  ::farm_ng::ErrorDetail {                                              \
    .file = __FILE__, .line = __LINE__, .msg = FARM_FORMAT(__VA_ARGS__) \
  }

#define FARM_ERROR(cstr, ...) \
  ::tl::make_unexpected(      \
      ::farm_ng::Error{.details = {FARM_ERROR_DETAIL(cstr, ##__VA_ARGS__)}})

/// Assigns `*expected_val` to `Type_val`, but returns error if there is one.
#define FARM_TRY(Type_val, expected_val)                \
  if (!expected_val) {                                  \
    return ::tl::make_unexpected(expected_val.error()); \
  }                                                     \
  Type_val = ::std::move(*expected_val)

#define FARM_CHECK_OR_ERROR(condition, ...)                              \
  if (!(condition)) {                                                    \
    return FARM_ERROR(                                                   \
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
                  : FARM_ERROR("std::nullopt");
}

}  // namespace farm_ng
