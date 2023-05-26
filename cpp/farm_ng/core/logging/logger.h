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
#include <unordered_set>

namespace farm_ng {
FARM_ENUM(
    LogLevel,
    int,
    ((trace, 0),
     (debug, 1),
     (info, 2),
     (warning, 3),
     (error, 4),
     (critical, 5)));

std::string stringFromLogLevel(LogLevel level);

// A logger that writes to std::cerr
class StreamLogger {
 public:
  static LogLevel const kDefaultLogLevel = LogLevel(LogLevel::info);

  // The header format is a {fmt}-style format string that may include the
  //  named arguments {level}, {text}, {file}, {line}, {function}, {time},
  //  {time_ms}.
  void setHeaderFormat(std::string const& str);
  std::string getHeaderFormat() const;

  // Set the runtime log level
  void setLogLevel(LogLevel level);

  LogLevel getLogLevel();

  template <typename... TT>
  inline void log(
      LogLevel log_level,
      std::string const& header_text,
      std::string const& file,
      int line,
      std::string const& function,
      std::string const& message,
      TT&&... args) {
    if (log_level_ <= LogLevel::warning &&
        (log_level == LogLevel::debug || log_level == LogLevel::trace)) {
      // We are already in compile-time enabled noisy log-level territory. Hence
      // it is fine to do this hash map look-up from a performance standpoint.
      if (noisy_modules_.find(file) == noisy_modules_.end()) {
        // Only warn about noisy log level once per enabled compilation unit.
        noisy_modules_.insert(file);
        write("\n");
        writeHeader(LogLevel::warning, header_text, file, line, function);
        write(FARM_FORMAT(
            "Noisy logging (DEBUG or TRACE) enabled for module: {}  (runtime "
            "log "
            "level: {})",
            file,
            log_level_));
        flush();
      }
    }

    if (log_level_ <= log_level) {
      writeHeader(log_level, header_text, file, line, function);
      write(FARM_FORMAT(message, std::forward<TT>(args)...));
      flush();
    }
  }

  void log(
      LogLevel log_level,
      std::string const& header_text,
      std::string const& file,
      int line,
      std::string const& function) {
    this->log(log_level, header_text, file, line, function, std::string(""));
  }

 private:
  void writeHeader(
      LogLevel log_level,
      std::string const& header_text,
      std::string const& file,
      int line,
      std::string const& function);
  static void write(std::string const& str);
  static void flush();

  std::string header_format_ = "[FARM {text} in {file}:{line}]\n";
  LogLevel log_level_ = kDefaultLogLevel;
  std::unordered_set<std::string> noisy_modules_;
};

inline StreamLogger& defaultLogger() {
  static StreamLogger Static_Logger;
  return Static_Logger;
}

}  // namespace farm_ng

// Begin (Impl details)
#define FARM_VARGS(...) FARM_PP_COMMA() __VA_ARGS__

#define FARM_VARGS_NONE(...)

#define FARM_MAYBE_VARGS(...)                                        \
  FARM_PP_IF(                                                        \
      FARM_PP_EQUAL(FARM_PP_VARIADIC_SIZE(dummy, ##__VA_ARGS__), 1), \
      FARM_VARGS_NONE,                                               \
      FARM_VARGS)

// Global toggle to enable logging, undefined to disable logging globally
#define FARM_LOG_ENABLED 1

#ifdef FARM_LOG_ENABLED
/// Log level to be used for more noisy FYI type of status. Don't use FARM_INFO
/// if the pure compile-time invocation of the macro would hurt performance.
/// E.g. if this code might be executed within an inner loop such as for every
/// pixel of an image. Use FARM_DEBUG or FARM_TRACE instead which can be toggled
/// at compile-time (and are turned off by default).
#define FARM_INFO(...)          \
  farm_ng::defaultLogger().log( \
      farm_ng::LogLevel::info,  \
      "INFO",                   \
      __FILE__,                 \
      __LINE__,                 \
      __func__,                 \
      __VA_ARGS__)
/// More significant information with high signal to noise. Typically to
/// communicate an usual but valid state of the program.
#define FARM_WARN(...)            \
  farm_ng::defaultLogger().log(   \
      farm_ng::LogLevel::warning, \
      "WARN",                     \
      __FILE__,                   \
      __LINE__,                   \
      __func__,                   \
      __VA_ARGS__)
/// Error state (but not logical error/precondition violation nor panic).
/// Currently not used.
#define FARM_ERROR(...)         \
  farm_ng::defaultLogger().log( \
      farm_ng::LogLevel::error, \
      "ERROR",                  \
      __FILE__,                 \
      __LINE__,                 \
      __func__,                 \
      __VA_ARGS__)
/// Critical error state to communicated. Often followed by a panic (i.e.
/// std::abort, if enabled).
#define FARM_CRITICAL(...)         \
  farm_ng::defaultLogger().log(    \
      farm_ng::LogLevel::critical, \
      "CRITICAL",                  \
      __FILE__,                    \
      __LINE__,                    \
      __func__,                    \
      __VA_ARGS__)
#else
#define FARM_INFO(...)
#define FARM_WARN(...)
#define FARM_ERROR(...)
#define FARM_CRITICAL(...)
#endif

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

template <class TPoint, class TEnabler = void>
struct CheckNear;

template <class TT>
struct CheckNear<TT, std::enable_if_t<std::is_arithmetic_v<TT>, void>> {
  static void impl(
      double const& lhs,
      double const& rhs,
      double const& thr,
      char const* lhs_cstr,
      char const* rhs_cstr,
      char const* thr_cstr,
      std::string const& file,
      int line,
      std::string const& func,
      std::string const& str) {
    using std::abs;
    using std::min;
    auto lhs_abs = abs(lhs);
    auto rhs_abs = abs(rhs);
    if (min(lhs_abs, rhs_abs) < thr) {
      /* close to zero, we are doing an absolute comparison*/
      auto err = abs(lhs - rhs);
      if (!(err < (thr))) {
        farm_ng::defaultLogger().log(
            farm_ng::LogLevel::critical,
            FARM_FORMAT(
                "ASSERT_NEAR absolute]\n"
                "Not true: {} (={}) near {} (={}); has error of {} (thr: "
                "{} (={}))",
                lhs,
                lhs_cstr,
                rhs,
                rhs_cstr,
                err,
                thr,
                thr_cstr),
            file,
            line,
            func,
            str);
        FARM_IMPL_ABORT();
      }
    } else {
      /*  relative comparison*/
      double err = abs(lhs / rhs - 1.0);
      if (!(err < (thr))) {
        farm_ng::defaultLogger().log(
            farm_ng::LogLevel::critical,
            FARM_FORMAT(
                "ASSERT_NEAR relative]\n"
                "Not true: {} (={}) near {} (={}); has error of {} (thr: "
                "{} (={}))",
                lhs,
                lhs_cstr,
                rhs,
                rhs_cstr,
                err,
                thr,
                thr_cstr),
            file,
            line,
            func,
            str);
        FARM_IMPL_ABORT();
      }
    }
  }
};

}  // namespace details
}  // namespace farm_ng

/// If it is false that `lhs` is near `rhs` according to threshold `thr`, print
/// formatted error message and then panic.
///
/// `lhs` and `rhs` are near, if maxMetric(lhs, rhs) < thr.
#define FARM_ASSERT_NEAR(lhs, rhs, thr, ...)                        \
  ::farm_ng::details::CheckNear<std::decay_t<decltype(lhs)>>::impl( \
      lhs,                                                          \
      rhs,                                                          \
      thr,                                                          \
      #lhs,                                                         \
      #rhs,                                                         \
      #thr,                                                         \
      __FILE__,                                                     \
      __LINE__,                                                     \
      __func__,                                                     \
      FARM_FORMAT(__VA_ARGS__))

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
