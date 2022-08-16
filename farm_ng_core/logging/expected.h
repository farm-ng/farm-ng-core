

// Copyright (c) farm-ng, inc. All rights reserved.

#pragma once

#include "farm_ng_core/logging/logger.h"

#include <tl/expected.hpp>

#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace farm_ng_core {

struct Error {
  std::vector<ErrorDetail> details;
};

inline std::ostream& operator<<(std::ostream& os, const Error& error) {
  os << error.details.size() << "error details:\n";
  for (const auto& error : error.details) {
    os << FNG_FORMAT("[{}:{}] {}\n", error.file, error.line, error.msg);
  }
  return os;
}

struct Success {};

template <class T, class E = Error>
using Expected = tl::expected<T, E>;

namespace details {

template <class T, class E>
struct UnwrapImpl<tl::expected<T, E>> {
  static auto impl(
      tl::expected<T, E>& wrapper,
      const char* wrapper_cstr,
      ::farm_ng_core::ErrorDetail detail) -> decltype(*wrapper) {
    if (!bool(wrapper)) {
      FNG_IMPL_LOG_PRINTLN(
          "[FNG_UNWRAP failed in {}:{}]", detail.file, detail.line);
      FNG_IMPL_LOG_PRINTLN(
          "expected type `{}` does not contain a valid value", wrapper_cstr);
      if (!detail.msg.empty()) {
        ::fmt::print(stderr, detail.msg);
      }
      ::fmt::print(stderr, "{}", wrapper.error());
      FNG_IMPL_ABORT();
    }
    return *wrapper;
  }
};
}  // namespace details
}  // namespace farm_ng_core

#define FNG_ERROR_DETAIL(...)                                          \
  ::farm_ng_core::ErrorDetail {                                        \
    .file = __FILE__, .line = __LINE__, .msg = FNG_FORMAT(__VA_ARGS__) \
  }

#define FNG_ERROR(cstr, ...) \
  ::tl::make_unexpected(     \
      Error{.details = {FNG_ERROR_DETAIL(cstr, ##__VA_ARGS__)}})

/// Assigns `*expected_val` to `Type_val`, but returns error if there is one.
#define FNG_TRY(Type_val, expected_val)                 \
  if (!expected_val) {                                  \
    return ::tl::make_unexpected(expected_val.error()); \
  }                                                     \
  Type_val = ::std::move(*expected_val)

#define FNG_CHECK_OR_ERROR(condition, ...)                              \
  if (!(condition)) {                                                   \
    return FNG_ERROR(                                                   \
        "bool({}) not true.\n{}", #condition, FNG_FORMAT(__VA_ARGS__)); \
  }                                                                     \
  do {                                                                  \
  } while (false)

namespace farm_ng_core {
template <class T, class E = Error>
std::optional<T> fromExpected(Expected<T, E> expected) {
  return expected ? std::optional<T>(std::move(*expected)) : std::nullopt;
}

template <class T>
Expected<T> fromOptional(std::optional<T> optional) {
  return optional ? Expected<T>(std::move(*optional))
                  : FNG_ERROR("std::nullopt");
}

}  // namespace farm_ng_core
