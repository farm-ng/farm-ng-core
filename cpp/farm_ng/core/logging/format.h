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

#include <farm_ng/core/logging/backtrace.h>
#include <farm_pp/preprocessor/comparison/equal.hpp>
#include <farm_pp/preprocessor/control/if.hpp>
#include <farm_pp/preprocessor/variadic/size.hpp>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/std.h>

#include <functional>
#include <iostream>

#ifdef FARM_COMPILE_TIME_FMT
#define FARM_STRING(x) FMT_STRING(x)
#else
#define FARM_STRING(x) x
#endif

namespace farm_ng {
std::function<void(std::string const &)> &getLogLineFunction();

void setLogLineFunction(std::function<void(std::string const &)> f);

void logLine(std::string const &line);
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

#define FARM_IMPL_ABORT()      \
  do {                         \
    farm_ng::printBacktrace(); \
    ::std::abort();            \
  } while (false)

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
    return ::fmt::format(fmt::runtime(str), std::forward<TArgs>(args)...);
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
