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

#include "farm_ng/core/enum/enum.h"
#include "farm_ng/core/logging/format.h"

#include <farm_pp/preprocessor/comma.hpp>
#include <farm_pp/preprocessor/empty.hpp>

#include <filesystem>

#define FARM_LEVEL_TRACE 0
#define FARM_LEVEL_DEBUG 1
#define FARM_LEVEL_INFO 2
#define FARM_LEVEL_WARN 3
#define FARM_LEVEL_ERROR 4
#define FARM_LEVEL_CRITICAL 5
#define FARM_LEVEL_OFF 6

// Define FARM_LOG_LEVEL to set the compile-time log level
#ifndef FARM_LOG_LEVEL
#define FARM_LOG_LEVEL FARM_LEVEL_DEBUG
#endif

namespace farm_ng {
FARM_ENUM(LogLevel, int, (trace, debug, info, warning, error, critical, off));

std::string stringFromLogLevel(LogLevel level);

// A logger that writes to std::cerr
class StreamLogger {
 public:
  // The header format is a {fmt}-style format string that may include the
  //  named arguments {level}, {text}, {file}, {line}, {function}, {time},
  //  {time_ms}.
  void setHeaderFormat(std::string const& str);
  std::string getHeaderFormat() const;

  // Set the runtime log level
  void setLogLevel(LogLevel level);

  LogLevel getLogLevel();

  void log(
      LogLevel log_level,
      std::string const& header_text,
      std::string const& file,
      int line,
      std::string const& function);

  template <typename... T>
  inline void log(
      LogLevel log_level,
      std::string const& header_text,
      std::string const& file,
      int line,
      std::string const& function,
      std::string const& message,
      T&&... args) {
    if (log_level_ <= log_level) {
      writeHeader(log_level, header_text, file, line, function);
      write(FARM_FORMAT(message, std::forward<T>(args)...));
      flush();
    }
  }

 private:
  void writeHeader(
      LogLevel log_level,
      std::string const& header_text,
      std::string const& file,
      int line,
      std::string const& function);
  void write(std::string const& str);
  void flush();

  std::string header_format_ = "[FARM {text} in {file}:{line}]";
  LogLevel log_level_ = LogLevel(FARM_LOG_LEVEL);
};

inline StreamLogger& defaultLogger() {
  static StreamLogger logger;
  return logger;
}

}  // namespace farm_ng

#if defined FARM_LOG_LEVEL && FARM_LOG_LEVEL <= FARM_LEVEL_TRACE
#define FARM_TRACE(...)         \
  farm_ng::defaultLogger().log( \
      farm_ng::LogLevel::trace, \
      "TRACE",                  \
      __FILE__,                 \
      __LINE__,                 \
      __func__,                 \
      __VA_ARGS__)
#else
#define FARM_TRACE(...)
#endif

#if defined FARM_LOG_LEVEL && FARM_LOG_LEVEL <= FARM_LEVEL_DEBUG
#define FARM_DEBUG(...)         \
  farm_ng::defaultLogger().log( \
      farm_ng::LogLevel::debug, \
      "DEBUG",                  \
      __FILE__,                 \
      __LINE__,                 \
      __func__,                 \
      __VA_ARGS__)
#else
#define FARM_DEBUG(...)
#endif

#if defined FARM_LOG_LEVEL && FARM_LOG_LEVEL <= FARM_LEVEL_INFO
#define FARM_INFO(...)          \
  farm_ng::defaultLogger().log( \
      farm_ng::LogLevel::info,  \
      "INFO",                   \
      __FILE__,                 \
      __LINE__,                 \
      __func__,                 \
      __VA_ARGS__)
#else
#define FARM_INFO(...)
#endif

#if defined FARM_LOG_LEVEL && FARM_LOG_LEVEL <= FARM_LEVEL_WARN
#define FARM_WARN(...)            \
  farm_ng::defaultLogger().log(   \
      farm_ng::LogLevel::warning, \
      "WARN",                     \
      __FILE__,                   \
      __LINE__,                   \
      __func__,                   \
      __VA_ARGS__)
#else
#define FARM_WARN(...)
#endif

#if defined FARM_LOG_LEVEL && FARM_LOG_LEVEL <= FARM_LEVEL_ERROR
#define FARM_ERROR(...)         \
  farm_ng::defaultLogger().log( \
      farm_ng::LogLevel::error, \
      "ERROR",                  \
      __FILE__,                 \
      __LINE__,                 \
      __func__,                 \
      __VA_ARGS__)
#else
#define FARM_ERROR(...)
#endif

#if defined FARM_LOG_LEVEL && FARM_LOG_LEVEL <= FARM_LEVEL_CRITICAL
#define FARM_CRITICAL(...)         \
  farm_ng::defaultLogger().log(    \
      farm_ng::LogLevel::critical, \
      "CRITICAL",                  \
      __FILE__,                    \
      __LINE__,                    \
      __func__,                    \
      __VA_ARGS__)
#else
#define FARM_CRITICAL(...)
#endif

// Begin (Impl details)
#define FARM_VARGS(...) FARM_PP_COMMA() __VA_ARGS__

#define FARM_VARGS_NONE(...)

#define FARM_MAYBE_VARGS(...)                                        \
  FARM_PP_IF(                                                        \
      FARM_PP_EQUAL(FARM_PP_VARIADIC_SIZE(dummy, ##__VA_ARGS__), 1), \
      FARM_VARGS_NONE,                                               \
      FARM_VARGS)

#define FARM_IMPL_ASSERT_OP(symbol, name_str, lhs, rhs, ...)                 \
  do {                                                                       \
    if (!((lhs)symbol(rhs))) {                                               \
      farm_ng::defaultLogger().log(                                          \
          farm_ng::LogLevel::critical,                                       \
          FARM_FORMAT(                                                       \
              "PANIC: ASSERT_{} failed\n Not true: {} {} {}\n{}\nvs.\n{}\n", \
              name_str,                                                      \
              #lhs,                                                          \
              #symbol,                                                       \
              #rhs,                                                          \
              lhs,                                                           \
              rhs),                                                          \
          __FILE__,                                                          \
          __LINE__,                                                          \
          __func__ FARM_MAYBE_VARGS(__VA_ARGS__)(__VA_ARGS__));              \
      FARM_IMPL_ABORT();                                                     \
    }                                                                        \
  } while (false)

// Deprecate
#define FARM_IMPL_LOG_HEADER(msg) \
  FARM_IMPL_LOG_PRINTLN("[FARM {} in {}:{}]", msg, __FILE__, __LINE__)

// End (Impl details)

/// Print formatted error message and then panic.
#define FARM_PANIC(...)                                     \
  farm_ng::defaultLogger().log(                             \
      farm_ng::LogLevel::critical,                          \
      "PANIC!",                                             \
      __FILE__,                                             \
      __LINE__,                                             \
      __func__ FARM_MAYBE_VARGS(__VA_ARGS__)(__VA_ARGS__)); \
  FARM_IMPL_ABORT()

/// Print formatted error message and then panic.
#define FARM_UNIMPLEMENTED(...)                             \
  farm_ng::defaultLogger().log(                             \
      farm_ng::LogLevel::critical,                          \
      "PANIC: NOT IMPLEMENTED YET",                         \
      __FILE__,                                             \
      __LINE__,                                             \
      __func__ FARM_MAYBE_VARGS(__VA_ARGS__)(__VA_ARGS__)); \
  FARM_IMPL_ABORT()

// Deprecate
#define FARM_PRINTLN(cstr, ...) FARM_IMPL_LOG_PRINTLN(cstr, ##__VA_ARGS__)

/// LOG INFO: Only log every nth encounter.
#define FARM_INFO_EVERY_N(N, ...)                               \
  do {                                                          \
    static std::atomic<int> counter(0);                         \
    ++counter;                                                  \
    if (counter > (N)) {                                        \
      counter -= (N);                                           \
    }                                                           \
    if (counter == 1) {                                         \
      farm_ng::defaultLogger().log(                             \
          farm_ng::LogLevel::info,                              \
          FARM_FORMAT("LOG INFO EVERY N( = {} )", #N),          \
          __FILE__,                                             \
          __LINE__,                                             \
          __func__ FARM_MAYBE_VARGS(__VA_ARGS__)(__VA_ARGS__)); \
    }                                                           \
  } while (false)

/// If condition is false, Print formatted error message and then panic.
#define FARM_ASSERT(condition, ...)                                            \
  do {                                                                         \
    if (!(condition)) {                                                        \
      farm_ng::defaultLogger().log(                                            \
          farm_ng::LogLevel::critical,                                         \
          FARM_FORMAT("PANIC: ASSERT failed\nbool({}) not true.", #condition), \
          __FILE__,                                                            \
          __LINE__,                                                            \
          __func__ FARM_MAYBE_VARGS(__VA_ARGS__)(__VA_ARGS__));                \
      FARM_IMPL_ABORT();                                                       \
    }                                                                          \
  } while (false)

/// If it is false that `lhs` == `rhs`, print formatted error message and then
/// panic.
#define FARM_ASSERT_EQ(lhs, rhs, ...) \
  FARM_IMPL_ASSERT_OP(==, "EQ", lhs, rhs, __VA_ARGS__)

/// If it is false that `lhs` != `rhs`, print formatted error message and then
/// panic.
#define FARM_ASSERT_NE(lhs, rhs, ...) \
  FARM_IMPL_ASSERT_OP(!=, "NE", lhs, rhs, __VA_ARGS__)

/// If it is false that `lhs` <= `rhs`, print formatted error message and then
/// panic.
#define FARM_ASSERT_LE(lhs, rhs, ...) \
  FARM_IMPL_ASSERT_OP(<=, "LE", lhs, rhs, __VA_ARGS__)

/// If it is false that `lhs` < `rhs`, print formatted error message and then
/// panic.
#define FARM_ASSERT_LT(lhs, rhs, ...) \
  FARM_IMPL_ASSERT_OP(<, "LT", lhs, rhs, __VA_ARGS__)

/// If it is false that `lhs` >= `rhs`, print formatted error message and then
/// panic.
#define FARM_ASSERT_GE(lhs, rhs, ...) \
  FARM_IMPL_ASSERT_OP(>=, "GE", lhs, rhs, __VA_ARGS__)

/// If it is false that `lhs` > `rhs`, print formatted error message and then
/// panic.
#define FARM_ASSERT_GT(lhs, rhs, ...) \
  FARM_IMPL_ASSERT_OP(>, "GT", lhs, rhs, __VA_ARGS__)

namespace farm_ng {
namespace details {
// following:
// https://github.com/strasdat/Sophus/blob/master/sophus/types.hpp#L94
template <class TScalar>
class MaxMetric {
 public:
  static TScalar impl(TScalar s0, TScalar s1) {
    using std::abs;
    return abs(s0 - s1);
  }
};

}  // namespace details

/// Returns maximum metric between two points `p0` and `p1`, with `p0, p1`
/// being matrices or a scalars.
template <class TType>
auto maxMetric(TType const& p0, TType const& p1)
    -> decltype(details::MaxMetric<TType>::impl(p0, p1)) {
  return details::MaxMetric<TType>::impl(p0, p1);
}
}  // namespace farm_ng

/// If it is false that `lhs` is near `rhs` according to threshold `thr`, print
/// formatted error message and then panic.
///
/// `lhs` and `rhs` are near, if maxMetric(lhs, rhs) < thr.
#define FARM_ASSERT_NEAR(lhs, rhs, thr, ...)                                 \
  do {                                                                       \
    auto nrm = farm_ng::maxMetric((lhs), (rhs));                             \
    if (!(nrm < (thr))) {                                                    \
      farm_ng::defaultLogger().log(                                          \
          farm_ng::LogLevel::critical,                                       \
          FARM_FORMAT(                                                       \
              "PANIC: ASSERT_NEAR failed\n Not true: {} near {}; has error " \
              "of {} (thr: {})\n{}\n{}\nvs.\n{}\n{}",                        \
              #lhs,                                                          \
              #rhs,                                                          \
              nrm,                                                           \
              thr,                                                           \
              #lhs,                                                          \
              lhs,                                                           \
              #rhs,                                                          \
              rhs),                                                          \
          __FILE__,                                                          \
          __LINE__,                                                          \
          __func__ FARM_MAYBE_VARGS(__VA_ARGS__)(__VA_ARGS__));              \
      FARM_IMPL_ABORT();                                                     \
    }                                                                        \
  } while (false)
// End (ASSERT macros)

namespace farm_ng {
struct ErrorDetail {
  std::string file;
  int line;
  std::string msg;
};

namespace details {

template <class TContainer>
auto checkedAtContiguousContainer(
    TContainer& container,
    size_t index,
    char const* container_cstr,
    char const* index_cstr,
    std::string const& file,
    int line,
    std::string const& func,
    std::string const& str) -> decltype(container[index]) {
  if (index >= container.size()) {
    farm_ng::defaultLogger().log(
        farm_ng::LogLevel::critical,
        FARM_FORMAT(
            "FARM_AT index `{}` (={}) not in contiguous container `{}` of size "
            "`{}`",
            index_cstr,
            index,
            container_cstr,
            container.size()),
        file,
        line,
        func,
        str);
    FARM_IMPL_ABORT();
  }
  return container[index];
}

template <class TContainer, class TKey>
auto checkedGetFromAssociativeContainer(
    TContainer& container,
    TKey const& key,
    char const* container_cstr,
    char const* key_cstr,
    std::string const& file,
    int line,
    std::string const& func,
    std::string const& str) -> decltype(*(container.find(key))) {
  auto it = container.find(key);
  if (it == container.end()) {
    farm_ng::defaultLogger().log(
        farm_ng::LogLevel::critical,
        FARM_FORMAT(
            "FARM_GET key `{}` (={}) not in associative container `{}` of size "
            "`{}`",
            key_cstr,
            key,
            container_cstr,
            container.size()),
        file,
        line,
        func,
        str);
    FARM_IMPL_ABORT();
  }
  return *it;
}

template <class TMapLike, class TKey, class TValue>
auto insertKeyValueInMap(
    TMapLike& map,
    TKey const& key,
    TValue const& value,
    char const* container_cstr,
    char const* key_cstr,
    char const* value_cstr,
    std::string const& file,
    int line,
    std::string const& func,
    std::string const& str) {
  auto const [iterator, success] = map.insert({key, value});

  if (!success) {
    farm_ng::defaultLogger().log(
        farm_ng::LogLevel::critical,
        FARM_FORMAT(
            "FARM_INSERT key `{}` (={}) is already in map `{}` of size `{}`. "
            "\nWe cannot insert value `{}`.",
            key_cstr,
            key,
            container_cstr,
            map.size(),
            value_cstr),
        file,
        line,
        func,
        str);
    FARM_IMPL_ABORT();
  }
  return iterator;
}

template <class TWrapper>
struct UnwrapImpl {
  static auto impl(
      TWrapper& wrapper,
      char const* wrapper_cstr,
      ::farm_ng::ErrorDetail detail) -> decltype(*wrapper) {
    if (!bool(wrapper)) {
      farm_ng::defaultLogger().log(
          farm_ng::LogLevel::critical,
          FARM_FORMAT(
              "FARM_UNWRAP wrapper type `{}` does not contain a valid value",
              wrapper_cstr),
          detail.file,
          detail.line,
          "",
          detail.msg);
      FARM_IMPL_ABORT();
    }
    return *wrapper;
  }
};

template <class TWrapper>
auto unwrapImpl(
    TWrapper& wrapper, char const* wrapper_cstr, ::farm_ng::ErrorDetail detail)
    -> decltype(*wrapper) {
  return UnwrapImpl<TWrapper>::impl(wrapper, wrapper_cstr, std::move(detail));
}

}  // namespace details
}  // namespace farm_ng

/// Returns `contiguous_container[index]`, but panics if `index` is out of
/// bounds.
#define FARM_AT(contiguous_container, index, ...) \
  farm_ng::details::checkedAtContiguousContainer( \
      contiguous_container,                       \
      index,                                      \
      #contiguous_container,                      \
      #index,                                     \
      __FILE__,                                   \
      __LINE__,                                   \
      __func__,                                   \
      FARM_FORMAT(__VA_ARGS__))

/// Returns `associative_container[key]`, but panics if the container does not
/// contain `key` .
#define FARM_GET(associative_container, key, ...)       \
  farm_ng::details::checkedGetFromAssociativeContainer( \
      associative_container,                            \
      key,                                              \
      #associative_container,                           \
      #key,                                             \
      __FILE__,                                         \
      __LINE__,                                         \
      __func__,                                         \
      FARM_FORMAT(__VA_ARGS__))

/// Insert `val` to `map`, but panics if the container does
/// contain `key` already.
#define FARM_MAP_INSERT(map, key, value, ...) \
  farm_ng::details::insertKeyValueInMap(      \
      map,                                    \
      key,                                    \
      value,                                  \
      #map,                                   \
      #key,                                   \
      #value,                                 \
      __FILE__,                               \
      __LINE__,                               \
      __func__,                               \
      FARM_FORMAT(__VA_ARGS__))

/// Returns `*wrapper`, but panics if `wrapper` is `nullopt` or `null`.
#define FARM_UNWRAP(wrapper, ...) \
  farm_ng::details::unwrapImpl(   \
      wrapper,                    \
      #wrapper,                   \
      ::farm_ng::ErrorDetail{     \
          .file = __FILE__,       \
          .line = __LINE__,       \
          .msg = FARM_FORMAT(__VA_ARGS__)})

/// Asserts that the given path exits.
#define FARM_ASSERT_PATH_EXIST(path, ...)                                     \
  do {                                                                        \
    bool does_exist = std::filesystem::exists(path);                          \
    if (!does_exist) {                                                        \
      farm_ng::defaultLogger().log(                                           \
          farm_ng::LogLevel::critical,                                        \
          FARM_FORMAT(                                                        \
              "PANIC: ASSERT_PATH_EXIST failed\nThe following path does not " \
              "exist: {}",                                                    \
              path),                                                          \
          __FILE__,                                                           \
          __LINE__,                                                           \
          __func__ FARM_MAYBE_VARGS(__VA_ARGS__)(__VA_ARGS__));               \
      FARM_IMPL_ABORT();                                                      \
    }                                                                         \
  } while (false)

/// Asserts that the given path is a regular file.
/// Hence it asserts that
///  a) it does exit,
///  b) it is a regular file.
#define FARM_ASSERT_IS_FILE(path, ...)                                     \
  do {                                                                     \
    FARM_ASSERT_PATH_EXIST(path);                                          \
    bool is_regular = std::filesystem::is_regular_file(path);              \
    if (!is_regular) {                                                     \
      farm_ng::defaultLogger().log(                                        \
          farm_ng::LogLevel::critical,                                     \
          FARM_FORMAT(                                                     \
              "PANIC: ASSERT_IS_FILE failed\nThe following path is not a " \
              "regular file: {}",                                          \
              path),                                                       \
          __FILE__,                                                        \
          __LINE__,                                                        \
          __func__ FARM_MAYBE_VARGS(__VA_ARGS__)(__VA_ARGS__));            \
      FARM_IMPL_ABORT();                                                   \
    }                                                                      \
  } while (false)

/// Asserts that the given path is a folder.
/// Hence it asserts that
///  a) it does exit,
///  b) it is a folder (and not a file).
#define FARM_ASSERT_IS_FOLDER(path, ...)                                     \
  do {                                                                       \
    FARM_ASSERT_PATH_EXIST(path);                                            \
    bool is_folder = std::filesystem::is_directory(path);                    \
    if (!is_folder) {                                                        \
      farm_ng::defaultLogger().log(                                          \
          farm_ng::LogLevel::critical,                                       \
          FARM_FORMAT(                                                       \
              "PANIC: ASSERT_IS_FOLDER failed\nThe following path is not a " \
              "folder: {}",                                                  \
              path),                                                         \
          __FILE__,                                                          \
          __LINE__,                                                          \
          __func__ FARM_MAYBE_VARGS(__VA_ARGS__)(__VA_ARGS__));              \
      FARM_IMPL_ABORT();                                                     \
    }                                                                        \
  } while (false)

/// Asserts that the given path is an empty folder.
/// Hence it asserts that
///  a) it does exit,
///  b) it is a folder (and not a file),
///  c) it is empty
#define FARM_ASSERT_FOLDER_IS_EMPTY(path, ...)                                 \
  do {                                                                         \
    FARM_ASSERT_IS_FOLDER(path);                                               \
    bool is_empty = std::filesystem::is_empty(path);                           \
    if (!is_empty) {                                                           \
      farm_ng::defaultLogger().log(                                            \
          farm_ng::LogLevel::critical,                                         \
          FARM_FORMAT(                                                         \
              "PANIC: ASSERT_FOLDER_IS_EMPTY failed\nThe following folder is " \
              "not empty: {}",                                                 \
              path),                                                           \
          __FILE__,                                                            \
          __LINE__,                                                            \
          __func__ FARM_MAYBE_VARGS(__VA_ARGS__)(__VA_ARGS__));                \
      FARM_IMPL_ABORT();                                                       \
    }                                                                          \
  } while (false)
