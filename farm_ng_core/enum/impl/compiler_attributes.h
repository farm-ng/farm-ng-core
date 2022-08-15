// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#ifdef _MSC_VER

#if _MSVC_LANG >= 201703L
#define FNG_ENUM_HAS_CPP_17() 1
#else
#define FNG_ENUM_HAS_CPP_17() 0
#endif

#else  // !_MSC_VER

#if __cplusplus <= 201402L
#define FNG_ENUM_HAS_CPP_17() 0
#else
#define FNG_ENUM_HAS_CPP_17() 1
#endif

#endif  // !_MSC_VER

#if FNG_ENUM_HAS_CPP_17()

#define FNG_ENUM_NODISCARD [[nodiscard]]
#define FNG_ENUM_MAYBE_UNUSED [[maybe_unused]]

#else  // !HAS_CPP_17()

#ifdef __has_cpp_attribute

// from https://infektor.net/posts/2017-01-19-using-cpp17-attributes-today.html
#if __has_cpp_attribute(nodiscard)
#define FNG_ENUM_NODISCARD [[nodiscard]]
#elif __has_cpp_attribute(gnu::warn_unused_result)
#define FNG_ENUM_NODISCARD [[gnu::warn_unused_result]]
#endif

#if __has_cpp_attribute(maybe_unused)
#define FNG_ENUM_MAYBE_UNUSED [[maybe_unused]]
#elif __has_cpp_attribute(gnu::unused)
#define FNG_ENUM_MAYBE_UNUSED [[gnu::unused]]
#endif

#else  // !__has_cpp_attribute

#ifdef _MSC_VER
#define NODISCARD _Check_return_
#endif

#endif  // !__has_cpp_attribute

//
// Fallbacks in case above logic doesn't trigger: empty defines.
//

#ifndef FNG_ENUM_NODISCARD
#define FNG_ENUM_NODISCARD
#endif

#ifndef FNG_ENUM_MAYBE_UNUSED
#define FNG_ENUM_MAYBE_UNUSED
#endif

#endif  // !FNG_ENUM_HAS_CPP_17()
