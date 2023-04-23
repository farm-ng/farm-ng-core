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

// Copyright (c) farm-ng, inc. All rights reserved.

#pragma once

#include "farm_ng/core/misc/uri.h"
#include "farm_ng/core/pipeline/context.h"
#include "farm_ng/core/pipeline/input.h"

#include <farm_ng/core/logging/logger.h>
#include <farm_ng/core/misc/void.h>

#include <typeinfo>

namespace farm_ng {

/// Transforms identifier to C++ source identifier
///
/// Used for uri on Component templated class
std::string demangleTypeid(std::type_info const &type);

/// Parent definition of a farm_ng::Component
class Component {
 public:
  /// Default constructor of a context, it gets its own `io_context`.
  template <class Derived>
  explicit Component(
      Context const &ctx,
      std::string scheme,
      Derived *,
      std::string const &path)
      : context_strand_(ctx),
        uri_(
            scheme,
            FARM_FORMAT("[{}]", demangleTypeid(typeid(Derived))),
            path) {
    static_assert(
        std::is_base_of_v<Component, Derived>,
        "must be derived from Component");
  }

  /// Virtual destructor for component derived classes
  virtual ~Component() = default;

  /// Returns the owned ``Context`` by the component.
  Context getContext() const;

  /// Returns the owned ``ContextStrand`` by the component.
  ContextStrand getContextStrand() const;

  /// Returns the unique uri of the component instance.
  Uri const &uri() const;

 protected:
  /// Placeholder for a component reset callback
  virtual void onReset(Void const &) {}

 private:
  ContextStrand context_strand_;
  Uri uri_;
};

/// Templated ``Input`` constructor
template <class T>
Input<T>::Input(
    Component const *component,
    std::string const &name,
    std::function<void(T)> const &f,
    InputConfig const &config)
    : context_strand_(FARM_UNWRAP(component).getContextStrand()),
      uri_(FARM_UNWRAP(component).uri()),
      config_(config),
      function_(f),
      count_(0) {
  uri_.query = FARM_FORMAT("in={}", name);
}

}  // namespace farm_ng
