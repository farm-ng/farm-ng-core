// Copyright (c) farm-ng, inc. All rights reserved.

#pragma once

#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/variadic/size.hpp>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include <functional>

#ifdef FARM_NG_COMPILE_TIME_FMT
#define FARM_NG_STRING(x) FMT_STRING(x)
#else
#define FARM_NG_STRING(x) x
#endif

namespace farm_ng_core {
void setLogLineFunction(std::function<void(const std::string&)> f);
void logLine(const std::string& line);
}  // namespace farm_ng_core

// Begin (Impl details)
#define FARM_NG_IMPL_LOG_PRINTLN_ZERO()

#define FARM_NG_IMPL_LOG_PRINTLN_ONE(cstr) \
  farm_ng_core::logLine(FARM_NG_STRING(cstr))

#define FARM_NG_IMPL_LOG_PRINTLN_ARGS(cstr, ...) \
  farm_ng_core::logLine(::fmt::format(FMT_STRING(cstr), __VA_ARGS__))

#define FARM_NG_IMPL_LOG_PRINTLN_VARG(cstr, ...)                      \
  BOOST_PP_IF(                                                        \
      BOOST_PP_EQUAL(BOOST_PP_VARIADIC_SIZE(cstr, ##__VA_ARGS__), 1), \
      FARM_NG_IMPL_LOG_PRINTLN_ONE(cstr),                             \
      FARM_NG_IMPL_LOG_PRINTLN_ARGS(cstr, ##__VA_ARGS__))

#define FARM_NG_IMPL_LOG_PRINTLN(...)                                  \
  BOOST_PP_IF(                                                         \
      BOOST_PP_EQUAL(BOOST_PP_VARIADIC_SIZE(dummy, ##__VA_ARGS__), 1), \
      FARM_NG_IMPL_LOG_PRINTLN_ZERO(),                                 \
      FARM_NG_IMPL_LOG_PRINTLN_VARG(__VA_ARGS__))

#define FARM_NG_IMPL_ABORT() ::std::abort();

// End (Impl details)

namespace farm_ng_core {
namespace details {

template <class... Args>
std::string runtimeFormatImpl(
    const std::string& file, int line, const std::string& str, Args&&... args) {
  try {
    return ::fmt::format(str, std::forward<Args>(args)...);
  } catch (::fmt::format_error& e) {
    FARM_NG_IMPL_LOG_PRINTLN("[FARM_NG_RUNTIME_FORMAT in {}:{}]", file, line);
    FARM_NG_IMPL_LOG_PRINTLN(
        "Runtime format error thrown for {}.\nError string: {}", str, e.what());
  }
  FARM_NG_IMPL_ABORT();
}

}  // namespace details
}  // namespace farm_ng_core

/// Formats the `cstr` using the libfmt library.
///
/// See here for details: https://fmt.dev/latest/syntax.html
///
/// If `str` is cstring literal, call FARM_NG_FORMAT instead, and the format
/// will be checked at compile time.
#define FARM_NG_RUNTIME_FORMAT(str, ...) \
  ::farm_ng_core::details::runtimeFormatImpl(__FILE__, __LINE__, str, __VA_ARGS__)

#ifdef FARM_NG_COMPILE_TIME_FMT
#define FARM_NG_IMPL_FORMAT_ARGS(cstr, ...) \
  ::fmt::format(FMT_STRING(cstr), __VA_ARGS__)
#else
#define FARM_NG_IMPL_FORMAT_ARGS(cstr, ...) \
  FARM_NG_RUNTIME_FORMAT(cstr, __VA_ARGS__)
#endif

#define FARM_NG_IMPL_FORMAT_VARG(cstr, ...)                           \
  BOOST_PP_IF(                                                        \
      BOOST_PP_EQUAL(BOOST_PP_VARIADIC_SIZE(cstr, ##__VA_ARGS__), 1), \
      std::string(cstr),                                              \
      FARM_NG_IMPL_FORMAT_ARGS(cstr, ##__VA_ARGS__))

/// Formats the `cstr` using the libfmt library.
///
/// See here for details: https://fmt.dev/latest/syntax.html
///
/// Note that compile-time format check is performed, and hence cstr needs to be
/// a string literal. If no compile-time check is required, and/or the string is
/// a variable, call ``FARM_NG_RUNTIME_FORMAT(fmt, ...)`` instead.
///
/// FARM_NG_COMPILE_TIME_FMT is not defined, the runtime version will be used,
/// to speed up compile times.
///
#define FARM_NG_FORMAT(...)                                            \
  BOOST_PP_IF(                                                         \
      BOOST_PP_EQUAL(BOOST_PP_VARIADIC_SIZE(dummy, ##__VA_ARGS__), 1), \
      std::string(),                                                   \
      FARM_NG_IMPL_FORMAT_VARG(__VA_ARGS__))
