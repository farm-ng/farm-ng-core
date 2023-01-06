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

#include "farm_ng/core/logging/expected.h"
#include "farm_ng/core/misc/concept_utils.h"

#include <memory>

namespace farm_ng {

// Represents a Non-nullable pointer with shared ownership
// Is essentially an adapter between std::shared_ptr and farm_ng::Expected
template <class T>
class Shared {
 public:
  using ExpectedT = farm_ng::Expected<Shared<T>>;
  using BadExpectedAccess = tl::bad_expected_access<farm_ng::Error>;

  /// Construct from a possibly null shared_ptr
  /// The return value is an object containing either a non-null Shared object
  /// pointer,
  //  or an farm_ng::Error object
  static ExpectedT tryFrom(std::shared_ptr<T> const& maybe_null) noexcept {
    if (!maybe_null) {
      return FARM_UNEXPECTED("is null");
    }
    // For some reason, Expected seems to have trouble accepting an r-value
    // Shared<T> with its deleted copy constructor.
    Shared<T> temp(maybe_null);
    return temp;
  }

  /// Construct and also makes interior object T
  /// The return value is an object containing either a non-null Shared object
  /// pointer, for the new object, or an farm_ng::Error object if memory
  /// allocation failst or the constructor for the object throws
  template <class... Args>
  static ExpectedT tryMake(Args&&... args) noexcept {
    try {
      // can throw on bad memory allocation and exceptions in constructor of T.
      return tryFrom(std::make_shared<T>(std::forward<Args>(args)...));
    } catch (std::exception& e) {
      return FARM_UNEXPECTED(e.what());
    }
    return FARM_UNEXPECTED();
  }

  /// Construct from a possibly null shared_ptr
  /// Panics if shared is null. See `tryFrom()` for alternate.
  static Shared from(std::shared_ptr<T> const& shared) {
    auto maybe = tryFrom(shared);
    return FARM_UNWRAP(maybe);
  }

  /// Construct and also makes interior object T
  /// Panics if the constructor throws. See `tryFrom()` for alternate.
  template <class... Args>
  static Shared<T> make(Args&&... args) {
    auto maybe = tryMake(std::forward<Args>(args)...);
    return FARM_UNWRAP(maybe);
  }

  /// Returns the interior object which is guarenteed to be available
  T& operator*() { return *non_null_shared_; }

  /// Returns the interior object which is guarenteed to be available
  T const& operator*() const { return *non_null_shared_; }

  /// Returns the interior object which is guarenteed to be available
  T* operator->() { return non_null_shared_.get(); }

  /// Returns the interior object which is guarenteed to be available
  T const* operator->() const { return non_null_shared_.get(); }

  // Implicit conversion to a nullable std::shared_ptr okay
  operator std::shared_ptr<T>() const { return sharedPtr(); }

  // Return a nullable shared_ptr<T> from this Shared<T> object
  std::shared_ptr<T> sharedPtr() const { return non_null_shared_; }

  // Return the raw point from this Shared<T> object
  T const* ptr() const { return sharedPtr().get(); }

  // Return the raw point from this Shared<T> object
  T* ptr() { return sharedPtr().get(); }

  // Default constructable only if T is
  Shared() requires(std::is_default_constructible<T>::value)
      : non_null_shared_(std::make_shared<T>()) {
    checkMaybeThrow();
  }

  // Copy constructor from derived bases
  template <DerivedFrom<T> Derived>
  Shared(Shared<Derived> const& other) : non_null_shared_(other.sharedPtr()) {
    checkMaybeThrow();
  }

  // Construct from shared_ptr
  template <DerivedFrom<T> Derived>
  Shared(std::shared_ptr<Derived> const& panic_if_null)
      : non_null_shared_(panic_if_null) {
    checkMaybeThrow();
  }

  // Take ownership from unique_ptr
  template <DerivedFrom<T> Derived>
  Shared(std::unique_ptr<Derived>&& panic_if_null)
      : non_null_shared_(std::move(panic_if_null)) {
    checkMaybeThrow();
  }

  // Not sure why this is needed when the generic one
  // is defined above
  Shared(std::shared_ptr<T> const& panic_if_null)
      : non_null_shared_(panic_if_null) {
    checkMaybeThrow();
  }

  Shared(Shared<T>&& o) : non_null_shared_(o.non_null_shared_) {
    // We mustn't move the internal shared_ptr
    // because that would break the invariant.
  }

  Shared<T>& operator=(Shared<T>&& o) {
    // We maintain the invariant since
    // o.non_null_shared_ must also be valid
    non_null_shared_ = o.non_null_shared_;
    return *this;
  }

  Shared(Shared<T> const&) = default;
  Shared<T>& operator=(Shared<T> const&) = default;

  bool operator==(Shared<T> const& rhs) const noexcept {
    return this->non_null_shared_ == rhs.non_null_shared_;
  }

 private:
  void checkMaybeThrow() const {
    if (!non_null_shared_) {
      throw BadExpectedAccess(FARM_ERROR_REPORT("Shared is null."));
    }
  }

  // Class invariant:non_null_shared_ is guaranteed not to be null.
  std::shared_ptr<T> non_null_shared_;
};

template <typename T>
using ExpectShared = farm_ng::Expected<Shared<T>>;

template <class T, class... Args>
static ExpectShared<T> tryMakeShared(Args&&... args) noexcept {
  return Shared<T>::tryMake(std::forward<Args>(args)...);
}

template <typename T>
using SharedVector = std::vector<Shared<T>>;

}  // namespace farm_ng
