// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <fng_core/logging/format.h>

// Begin (Impl details)

#define FNG_IMPL_CHECK_OP(symbol, name_str, lhs, rhs, ...)                     \
  do {                                                                         \
    if (!((lhs)symbol(rhs))) {                                                 \
      FNG_IMPL_LOG_HEADER("CHECK_" name_str " failed");                        \
      FNG_IMPL_LOG_PRINTLN(                                                    \
          "Not true: {} " #symbol " {}\n{}\nvs.\n{}\n", #lhs, #rhs, lhs, rhs); \
      FNG_IMPL_LOG_PRINTLN(__VA_ARGS__);                                       \
      FNG_IMPL_ABORT();                                                        \
    }                                                                          \
  } while (false)

#define FNG_IMPL_LOG_HEADER(msg) \
  FNG_IMPL_LOG_PRINTLN("[FARM_NG {} in {}:{}]", msg, __FILE__, __LINE__)
// End (Impl details)

/// Print formatted error message and then panic.
#define FNG_FATAL(cstr, ...)                 \
  FNG_IMPL_LOG_HEADER("FATAL error");        \
  FNG_IMPL_LOG_PRINTLN(cstr, ##__VA_ARGS__); \
  FNG_IMPL_ABORT()

/// Print formatted error message and then panic.
#define FNG_UNIMPLEMENTED(cstr, ...)          \
  FNG_IMPL_LOG_HEADER("NOT IMPLEMENTED YET"); \
  FNG_IMPL_LOG_PRINTLN(cstr, ##__VA_ARGS__);  \
  FNG_IMPL_ABORT()

#define FNG_PRINTLN(cstr, ...) FNG_IMPL_LOG_PRINTLN(cstr, ##__VA_ARGS__)

/// LOG INFO: Log cstr formatted (using variadic arguments) to cerr.
#define FNG_LOG_INFO(cstr, ...)    \
  FNG_IMPL_LOG_HEADER("LOG INFO"); \
  FNG_IMPL_LOG_PRINTLN(cstr, ##__VA_ARGS__)

/// LOG INFO: Only log every nth encounter.
#define FNG_LOG_INFO_EVERY_N(N, cstr, ...)                \
  do {                                                    \
    static std::atomic<int> counter(0);                   \
    ++counter;                                            \
    if (counter > (N)) {                                  \
      counter -= (N);                                     \
    }                                                     \
    if (counter == 1) {                                   \
      FNG_IMPL_LOG_HEADER("LOG INFO EVERY N( =" #N " )"); \
      FNG_IMPL_LOG_PRINTLN(cstr, ##__VA_ARGS__);          \
    }                                                     \
  } while (false)

/// LOG WARNING: Log cstr formatted (using variadic arguments) to cerr.
#define FNG_LOG_WARNING(cstr, ...)    \
  FNG_IMPL_LOG_HEADER("LOG WARNING"); \
  FNG_IMPL_LOG_PRINTLN(cstr, ##__VA_ARGS__)

/// If condition is false, Print formatted error message and then panic.
#define FNG_CHECK(condition, ...)                             \
  do {                                                        \
    if (!(condition)) {                                       \
      FNG_IMPL_LOG_HEADER("FNG_CHECK failed");                \
      FNG_IMPL_LOG_PRINTLN("bool({}) not true.", #condition); \
      FNG_IMPL_LOG_PRINTLN(__VA_ARGS__);                      \
      FNG_IMPL_ABORT();                                       \
    }                                                         \
  } while (false)

/// If it is false that `lhs` == `rhs`, print formatted error message and then
/// panic.
#define FNG_CHECK_EQ(lhs, rhs, ...) \
  FNG_IMPL_CHECK_OP(==, "EQ", lhs, rhs, __VA_ARGS__)

/// If it is false that `lhs` != `rhs`, print formatted error message and then
/// panic.
#define FNG_CHECK_NE(lhs, rhs, ...) \
  FNG_IMPL_CHECK_OP(!=, "NE", lhs, rhs, __VA_ARGS__)

/// If it is false that `lhs` <= `rhs`, print formatted error message and then
/// panic.
#define FNG_CHECK_LE(lhs, rhs, ...) \
  FNG_IMPL_CHECK_OP(<=, "LE", lhs, rhs, __VA_ARGS__)

/// If it is false that `lhs` < `rhs`, print formatted error message and then
/// panic.
#define FNG_CHECK_LT(lhs, rhs, ...) \
  FNG_IMPL_CHECK_OP(<, "LT", lhs, rhs, __VA_ARGS__)

/// If it is false that `lhs` >= `rhs`, print formatted error message and then
/// panic.
#define FNG_CHECK_GE(lhs, rhs, ...) \
  FNG_IMPL_CHECK_OP(>=, "GE", lhs, rhs, __VA_ARGS__)

/// If it is false that `lhs` > `rhs`, print formatted error message and then
/// panic.
#define FNG_CHECK_GT(lhs, rhs, ...) \
  FNG_IMPL_CHECK_OP(>, "GT", lhs, rhs, __VA_ARGS__)

namespace fng_core {
namespace details {
// following:
// https://github.com/strasdat/Sophus/blob/master/sophus/types.hpp#L94
template <class Scalar>
class MaxMetric {
 public:
  static Scalar impl(Scalar s0, Scalar s1) {
    using std::abs;
    return abs(s0 - s1);
  }
};

}  // namespace details

/// Returns maximum metric between two points `p0` and `p1`, with `p0, p1`
/// being matrices or a scalars.
template <class T>
auto maxMetric(T const& p0, T const& p1)
    -> decltype(details::MaxMetric<T>::impl(p0, p1)) {
  return details::MaxMetric<T>::impl(p0, p1);
}
}  // namespace fng_core

/// If it is false that `lhs` is near `rhs` according to threshold `thr`, print
/// formatted error message and then panic.
///
/// `lhs` and `rhs` are near, if maxMetric(lhs, rhs) < thr.
#define FNG_CHECK_NEAR(lhs, rhs, thr, ...)               \
  do {                                                   \
    auto nrm = fng_core::maxMetric((lhs), (rhs));        \
    if (!(nrm < (thr))) {                                \
      FNG_IMPL_LOG_HEADER("CHECK_NEAR failed");          \
      FNG_IMPL_LOG_PRINTLN(                              \
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
      FNG_IMPL_LOG_PRINTLN(__VA_ARGS__);                 \
      FNG_IMPL_ABORT();                                  \
    }                                                    \
  } while (false)
// End (ASSERT macros)

namespace fng_core {
struct ErrorDetail {
  std::string file;
  int line;
  std::string msg;
};

namespace details {

template <class Container>
auto checkedAtContiguousContainer(
    Container& container,
    size_t index,
    const char* container_cstr,
    const char* index_cstr,
    const std::string& file,
    int line,
    const std::string& str) -> decltype(container[index]) {
  if (index >= container.size()) {
    FNG_IMPL_LOG_PRINTLN("[FNG_AT in {}:{}]", file, line);
    FNG_IMPL_LOG_PRINTLN(
        "index `{}` (={}) not in contiguous container `{}` of size `{}`",
        index_cstr,
        index,
        container_cstr,
        container.size());
    if (!str.empty()) {
      ::fmt::print(stderr, str);
    }
    FNG_IMPL_ABORT();
  }
  return container[index];
}

template <class Container, class Key>
auto checkedGetFromAssociativeContainer(
    Container& container,
    const Key& key,
    const char* container_cstr,
    const char* key_cstr,
    const std::string& file,
    int line,
    const std::string& str) -> decltype(*(container.find(key))) {
  auto it = container.find(key);
  if (it == container.end()) {
    FNG_IMPL_LOG_PRINTLN("[FNG_GET in {}:{}]", file, line);
    FNG_IMPL_LOG_PRINTLN(
        "key `{}` (={}) not in associative container `{}` of size `{}`",
        key_cstr,
        key,
        container_cstr,
        container.size());
    if (!str.empty()) {
      ::fmt::print(stderr, str);
    }
    FNG_IMPL_ABORT();
  }
  return *it;
}

template <class WrapperT>
struct UnwrapImpl {
  static auto impl(
      WrapperT& wrapper,
      const char* wrapper_cstr,
      ::fng_core::ErrorDetail detail) -> decltype(*wrapper) {
    if (!bool(wrapper)) {
      FNG_IMPL_LOG_PRINTLN(
          "[FNG_UNWRAP failed in {}:{}]", detail.file, detail.line);
      FNG_IMPL_LOG_PRINTLN(
          "wrapper type `{}` does not contain a valid value", wrapper_cstr);
      if (!detail.msg.empty()) {
        ::fmt::print(stderr, detail.msg);
      }
      FNG_IMPL_ABORT();
    }
    return *wrapper;
  }
};

template <class WrapperT>
auto unwrapImpl(
    WrapperT& wrapper, const char* wrapper_cstr, ::fng_core::ErrorDetail detail)
    -> decltype(*wrapper) {
  return UnwrapImpl<WrapperT>::impl(wrapper, wrapper_cstr, std::move(detail));
}

}  // namespace details
}  // namespace fng_core

/// Returns `contiguous_container[index]`, but panics if `index` is out of
/// bounds.
#define FNG_AT(contiguous_container, index, ...)   \
  fng_core::details::checkedAtContiguousContainer( \
      contiguous_container,                        \
      index,                                       \
      #contiguous_container,                       \
      #index,                                      \
      __FILE__,                                    \
      __LINE__,                                    \
      FNG_FORMAT(__VA_ARGS__))

/// Returns `associative_container[key]`, but panics if the container does not
/// contain `key` .
#define FNG_GET(associative_container, key, ...)         \
  fng_core::details::checkedGetFromAssociativeContainer( \
      associative_container,                             \
      key,                                               \
      #associative_container,                            \
      #key,                                              \
      __FILE__,                                          \
      __LINE__,                                          \
      FNG_FORMAT(__VA_ARGS__))

/// Returns `*wrapper`, but panics if `wrapper` is `nullopt` or `null`.
#define FNG_UNWRAP(wrapper, ...) \
  fng_core::details::unwrapImpl( \
      wrapper,                   \
      #wrapper,                  \
      ::fng_core::ErrorDetail{   \
          .file = __FILE__, .line = __LINE__, .msg = FNG_FORMAT(__VA_ARGS__)})

/// Checks whether the given path exits.
#define FNG_CHECK_PATH_EXIST(path)                                         \
  do {                                                                     \
    bool does_exist = std::filesystem::exists(path);                       \
    if (!does_exist) {                                                     \
      FNG_IMPL_LOG_HEADER("FNG_CHECK_PATH_EXIST failed");                  \
      FNG_IMPL_LOG_PRINTLN("The following path does not exist: {}", path); \
      FNG_IMPL_ABORT();                                                    \
    }                                                                      \
  } while (false)

/// Checks whether the given path is a regular file.
/// Hence it checks whether
///  a) it does exit,
///  b) it is a regular file.
#define FNG_CHECK_IS_FILE(path)                                  \
  do {                                                           \
    FNG_CHECK_PATH_EXIST(path);                                  \
    bool is_regular = std::filesystem::is_regular_file(path);    \
    if (!is_regular) {                                           \
      FNG_IMPL_LOG_HEADER("FNG_CHECK_IS_FILE failed");           \
      FNG_IMPL_LOG_PRINTLN(                                      \
          "The following path is not a regular file: {}", path); \
      FNG_IMPL_ABORT();                                          \
    }                                                            \
  } while (false)

/// Checks whether the given path is a folder.
/// Hence it checks whether
///  a) it does exit,
///  b) it is a folder (and not a file).
#define FNG_CHECK_IS_FOLDER(path)                                           \
  do {                                                                      \
    FNG_CHECK_PATH_EXIST(path);                                             \
    bool is_folder = std::filesystem::is_directory(path);                   \
    if (!is_folder) {                                                       \
      FNG_IMPL_LOG_HEADER("FNG_CHECK_IS_FOLDER failed");                    \
      FNG_IMPL_LOG_PRINTLN("The following path is not a folder: {}", path); \
      FNG_IMPL_ABORT();                                                     \
    }                                                                       \
  } while (false)

/// Checks whether the given path is an empty folder.
/// Hence it checks whether
///  a) it does exit,
///  b) it is a folder (and not a file),
///  c) it is empty
#define FNG_CHECK_FOLDER_IS_EMPTY(path)                                    \
  do {                                                                     \
    FNG_CHECK_IS_FOLDER(path);                                             \
    bool is_empty = std::filesystem::is_empty(path);                       \
    if (!is_empty) {                                                       \
      FNG_IMPL_LOG_HEADER("FNG_CHECK_FOLDER_IS_EMPTY failed");             \
      FNG_IMPL_LOG_PRINTLN("The following folder is not empty: {}", path); \
      FNG_IMPL_ABORT();                                                    \
    }                                                                      \
  } while (false)
