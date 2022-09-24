// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <farm_pp/preprocessor/comparison/equal.hpp>
#include <farm_pp/preprocessor/control/if.hpp>
#include <farm_pp/preprocessor/variadic/size.hpp>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include <functional>
#include <iostream>

#ifdef FARM_COMPILE_TIME_FMT
#define FARM_STRING(x) FMT_STRING(x)
#else
#define FARM_STRING(x) x
#endif

namespace farm_ng {
inline std::function<void(std::string const &)> &getLogLineFunction() {
  static std::function<void(std::string const &)> Static_Log_Function;
  return Static_Log_Function;
}

inline void setLogLineFunction(std::function<void(std::string const &)> f) {
  getLogLineFunction() = f;
}

inline void logLine(std::string const &line) {
  std::cerr << line << std::endl;
  if (getLogLineFunction()) {
    getLogLineFunction()(line);
  }
}
}  // namespace farm_ng

// Begin (Impl details)
#define FARM_IMPL_LOG_PRINTLN_ZERO()

#define FARM_IMPL_LOG_PRINTLN_ONE(cstr) farm_ng::logLine(FARM_STRING(cstr))

#define FARM_IMPL_LOG_PRINTLN_ARGS(cstr, ...) \
  farm_ng::logLine(::fmt::format(FMT_STRING(cstr), __VA_ARGS__))

#define FARM_IMPL_LOG_PRINTLN_VARG(cstr, ...)                       \
  FARM_PP_IF(                                                       \
      FARM_PP_EQUAL(FARM_PP_VARIADIC_SIZE(cstr, ##__VA_ARGS__), 1), \
      FARM_IMPL_LOG_PRINTLN_ONE(cstr),                              \
      FARM_IMPL_LOG_PRINTLN_ARGS(cstr, ##__VA_ARGS__))

#define FARM_IMPL_LOG_PRINTLN(...)                                   \
  FARM_PP_IF(                                                        \
      FARM_PP_EQUAL(FARM_PP_VARIADIC_SIZE(dummy, ##__VA_ARGS__), 1), \
      FARM_IMPL_LOG_PRINTLN_ZERO(),                                  \
      FARM_IMPL_LOG_PRINTLN_VARG(__VA_ARGS__))

#define FARM_IMPL_ABORT() ::std::abort();

// End (Impl details)

namespace farm_ng {
namespace details {

template <class... TArgs>
std::string runtimeFormatImpl(
    std::string const &file,
    int line,
    std::string const &str,
    TArgs &&...args) {
  try {
    return ::fmt::format(str, std::forward<TArgs>(args)...);
  } catch (::fmt::format_error &e) {
    FARM_IMPL_LOG_PRINTLN("[FARM_RUNTIME_FORMAT in {}:{}]", file, line);
    FARM_IMPL_LOG_PRINTLN(
        "Runtime format error thrown for {}.\nError string: {}", str, e.what());
  }
  FARM_IMPL_ABORT();
}

}  // namespace details
}  // namespace farm_ng

/// Formats the `cstr` using the libfmt library.
///
/// See here for details: https://fmt.dev/latest/syntax.html
///
/// If `str` is cstring literal, call FARM_FORMAT instead, and the format
/// will be checked at compile time.
#define FARM_RUNTIME_FORMAT(str, ...) \
  ::farm_ng::details::runtimeFormatImpl(__FILE__, __LINE__, str, __VA_ARGS__)

#ifdef FARM_COMPILE_TIME_FMT
#define FARM_IMPL_FORMAT_ARGS(cstr, ...) \
  ::fmt::format(FMT_STRING(cstr), __VA_ARGS__)
#else
#define FARM_IMPL_FORMAT_ARGS(cstr, ...) FARM_RUNTIME_FORMAT(cstr, __VA_ARGS__)
#endif

#define FARM_IMPL_FORMAT_VARG(cstr, ...)                            \
  FARM_PP_IF(                                                       \
      FARM_PP_EQUAL(FARM_PP_VARIADIC_SIZE(cstr, ##__VA_ARGS__), 1), \
      std::string(cstr),                                            \
      FARM_IMPL_FORMAT_ARGS(cstr, ##__VA_ARGS__))

/// Formats the `cstr` using the libfmt library.
///
/// See here for details: https://fmt.dev/latest/syntax.html
///
/// Note that compile-time format check is performed, and hence cstr needs to be
/// a string literal. If no compile-time check is required, and/or the string is
/// a variable, call ``FARM_RUNTIME_FORMAT(fmt, ...)`` instead.
///
/// FARM_COMPILE_TIME_FMT is not defined, the runtime version will be used,
/// to speed up compile times.
///
#define FARM_FORMAT(...)                                             \
  FARM_PP_IF(                                                        \
      FARM_PP_EQUAL(FARM_PP_VARIADIC_SIZE(dummy, ##__VA_ARGS__), 1), \
      std::string(),                                                 \
      FARM_IMPL_FORMAT_VARG(__VA_ARGS__))
