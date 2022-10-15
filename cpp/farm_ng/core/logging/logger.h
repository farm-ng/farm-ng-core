// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "farm_ng/core/logging/format.h"

// Begin (Impl details)

#define FARM_IMPL_CHECK_OP(symbol, name_str, lhs, rhs, ...)                    \
  do {                                                                         \
    if (!((lhs)symbol(rhs))) {                                                 \
      FARM_IMPL_LOG_HEADER("CHECK_" name_str " failed");                       \
      FARM_IMPL_LOG_PRINTLN(                                                   \
          "Not true: {} " #symbol " {}\n{}\nvs.\n{}\n", #lhs, #rhs, lhs, rhs); \
      FARM_IMPL_LOG_PRINTLN(__VA_ARGS__);                                      \
      FARM_IMPL_ABORT();                                                       \
    }                                                                          \
  } while (false)

#define FARM_IMPL_LOG_HEADER(msg) \
  FARM_IMPL_LOG_PRINTLN("[FARM_NG {} in {}:{}]", msg, __FILE__, __LINE__)
// End (Impl details)

/// Print formatted error message and then panic.
#define FARM_FATAL(cstr, ...)                 \
  FARM_IMPL_LOG_HEADER("FATAL error");        \
  FARM_IMPL_LOG_PRINTLN(cstr, ##__VA_ARGS__); \
  FARM_IMPL_ABORT()

/// Print formatted error message and then panic.
#define FARM_UNIMPLEMENTED(cstr, ...)          \
  FARM_IMPL_LOG_HEADER("NOT IMPLEMENTED YET"); \
  FARM_IMPL_LOG_PRINTLN(cstr, ##__VA_ARGS__);  \
  FARM_IMPL_ABORT()

#define FARM_PRINTLN(cstr, ...) FARM_IMPL_LOG_PRINTLN(cstr, ##__VA_ARGS__)

/// LOG INFO: Log cstr formatted (using variadic arguments) to cerr.
#define FARM_LOG_INFO(cstr, ...)    \
  FARM_IMPL_LOG_HEADER("LOG INFO"); \
  FARM_IMPL_LOG_PRINTLN(cstr, ##__VA_ARGS__)

/// LOG INFO: Only log every nth encounter.
#define FARM_LOG_INFO_EVERY_N(N, cstr, ...)                \
  do {                                                     \
    static std::atomic<int> counter(0);                    \
    ++counter;                                             \
    if (counter > (N)) {                                   \
      counter -= (N);                                      \
    }                                                      \
    if (counter == 1) {                                    \
      FARM_IMPL_LOG_HEADER("LOG INFO EVERY N( =" #N " )"); \
      FARM_IMPL_LOG_PRINTLN(cstr, ##__VA_ARGS__);          \
    }                                                      \
  } while (false)

/// LOG WARNING: Log cstr formatted (using variadic arguments) to cerr.
#define FARM_LOG_WARNING(cstr, ...)    \
  FARM_IMPL_LOG_HEADER("LOG WARNING"); \
  FARM_IMPL_LOG_PRINTLN(cstr, ##__VA_ARGS__)

/// If condition is false, Print formatted error message and then panic.
#define FARM_CHECK(condition, ...)                             \
  do {                                                         \
    if (!(condition)) {                                        \
      FARM_IMPL_LOG_HEADER("FARM_CHECK failed");               \
      FARM_IMPL_LOG_PRINTLN("bool({}) not true.", #condition); \
      FARM_IMPL_LOG_PRINTLN(__VA_ARGS__);                      \
      FARM_IMPL_ABORT();                                       \
    }                                                          \
  } while (false)

/// If it is false that `lhs` == `rhs`, print formatted error message and then
/// panic.
#define FARM_CHECK_EQ(lhs, rhs, ...) \
  FARM_IMPL_CHECK_OP(==, "EQ", lhs, rhs, __VA_ARGS__)

/// If it is false that `lhs` != `rhs`, print formatted error message and then
/// panic.
#define FARM_CHECK_NE(lhs, rhs, ...) \
  FARM_IMPL_CHECK_OP(!=, "NE", lhs, rhs, __VA_ARGS__)

/// If it is false that `lhs` <= `rhs`, print formatted error message and then
/// panic.
#define FARM_CHECK_LE(lhs, rhs, ...) \
  FARM_IMPL_CHECK_OP(<=, "LE", lhs, rhs, __VA_ARGS__)

/// If it is false that `lhs` < `rhs`, print formatted error message and then
/// panic.
#define FARM_CHECK_LT(lhs, rhs, ...) \
  FARM_IMPL_CHECK_OP(<, "LT", lhs, rhs, __VA_ARGS__)

/// If it is false that `lhs` >= `rhs`, print formatted error message and then
/// panic.
#define FARM_CHECK_GE(lhs, rhs, ...) \
  FARM_IMPL_CHECK_OP(>=, "GE", lhs, rhs, __VA_ARGS__)

/// If it is false that `lhs` > `rhs`, print formatted error message and then
/// panic.
#define FARM_CHECK_GT(lhs, rhs, ...) \
  FARM_IMPL_CHECK_OP(>, "GT", lhs, rhs, __VA_ARGS__)

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
#define FARM_CHECK_NEAR(lhs, rhs, thr, ...)              \
  do {                                                   \
    auto nrm = farm_ng::maxMetric((lhs), (rhs));         \
    if (!(nrm < (thr))) {                                \
      FARM_IMPL_LOG_HEADER("CHECK_NEAR failed");         \
      FARM_IMPL_LOG_PRINTLN(                             \
          "Not true: {} near {}; has error of {} (thr: " \
          "{})\n{}\n{}\nvs.\n{}\n{}",                    \
          #lhs,                                          \
          #rhs,                                          \
          nrm,                                           \
          thr,                                           \
          #lhs,                                          \
          lhs,                                           \
          #rhs,                                          \
          rhs);                                          \
      FARM_IMPL_LOG_PRINTLN(__VA_ARGS__);                \
      FARM_IMPL_ABORT();                                 \
    }                                                    \
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
    std::string const& str) -> decltype(container[index]) {
  if (index >= container.size()) {
    FARM_IMPL_LOG_PRINTLN("[FARM_AT in {}:{}]", file, line);
    FARM_IMPL_LOG_PRINTLN(
        "index `{}` (={}) not in contiguous container `{}` of size `{}`",
        index_cstr,
        index,
        container_cstr,
        container.size());
    if (!str.empty()) {
      ::fmt::print(stderr, "{}", str);
    }
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
    std::string const& str) -> decltype(*(container.find(key))) {
  auto it = container.find(key);
  if (it == container.end()) {
    FARM_IMPL_LOG_PRINTLN("[FARM_GET in {}:{}]", file, line);
    FARM_IMPL_LOG_PRINTLN(
        "key `{}` (={}) not in associative container `{}` of size `{}`",
        key_cstr,
        key,
        container_cstr,
        container.size());
    if (!str.empty()) {
      ::fmt::print(stderr, "{}", str);
    }
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
    std::string const& str) {
  auto const [iterator, success] = map.insert({key, value});

  if (!success) {
    FARM_IMPL_LOG_PRINTLN("[FARM_INSERT in {}:{}]", file, line);
    FARM_IMPL_LOG_PRINTLN(
        "key `{}` (={}) is already in map `{}` of size `{}`. \n"
        "We cannot insert value `{}`.",
        key_cstr,
        key,
        container_cstr,
        map.size(),
        value_cstr);
    if (!str.empty()) {
      ::fmt::print(stderr, "{}", str);
    }
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
      FARM_IMPL_LOG_PRINTLN(
          "[FARM_UNWRAP failed in {}:{}]", detail.file, detail.line);
      FARM_IMPL_LOG_PRINTLN(
          "wrapper type `{}` does not contain a valid value", wrapper_cstr);
      if (!detail.msg.empty()) {
        ::fmt::print(stderr, "{}", detail.msg);
      }
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

/// Checks whether the given path exits.
#define FARM_CHECK_PATH_EXIST(path)                                         \
  do {                                                                      \
    bool does_exist = std::filesystem::exists(path);                        \
    if (!does_exist) {                                                      \
      FARM_IMPL_LOG_HEADER("FARM_CHECK_PATH_EXIST failed");                 \
      FARM_IMPL_LOG_PRINTLN("The following path does not exist: {}", path); \
      FARM_IMPL_ABORT();                                                    \
    }                                                                       \
  } while (false)

/// Checks whether the given path is a regular file.
/// Hence it checks whether
///  a) it does exit,
///  b) it is a regular file.
#define FARM_CHECK_IS_FILE(path)                                 \
  do {                                                           \
    FARM_CHECK_PATH_EXIST(path);                                 \
    bool is_regular = std::filesystem::is_regular_file(path);    \
    if (!is_regular) {                                           \
      FARM_IMPL_LOG_HEADER("FARM_CHECK_IS_FILE failed");         \
      FARM_IMPL_LOG_PRINTLN(                                     \
          "The following path is not a regular file: {}", path); \
      FARM_IMPL_ABORT();                                         \
    }                                                            \
  } while (false)

/// Checks whether the given path is a folder.
/// Hence it checks whether
///  a) it does exit,
///  b) it is a folder (and not a file).
#define FARM_CHECK_IS_FOLDER(path)                                           \
  do {                                                                       \
    FARM_CHECK_PATH_EXIST(path);                                             \
    bool is_folder = std::filesystem::is_directory(path);                    \
    if (!is_folder) {                                                        \
      FARM_IMPL_LOG_HEADER("FARM_CHECK_IS_FOLDER failed");                   \
      FARM_IMPL_LOG_PRINTLN("The following path is not a folder: {}", path); \
      FARM_IMPL_ABORT();                                                     \
    }                                                                        \
  } while (false)

/// Checks whether the given path is an empty folder.
/// Hence it checks whether
///  a) it does exit,
///  b) it is a folder (and not a file),
///  c) it is empty
#define FARM_CHECK_FOLDER_IS_EMPTY(path)                                    \
  do {                                                                      \
    FARM_CHECK_IS_FOLDER(path);                                             \
    bool is_empty = std::filesystem::is_empty(path);                        \
    if (!is_empty) {                                                        \
      FARM_IMPL_LOG_HEADER("FARM_CHECK_FOLDER_IS_EMPTY failed");            \
      FARM_IMPL_LOG_PRINTLN("The following folder is not empty: {}", path); \
      FARM_IMPL_ABORT();                                                    \
    }                                                                       \
  } while (false)
