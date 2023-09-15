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

#include "farm_ng/core/misc/uri.h"
#include "farm_ng/core/pipeline/component.h"
#include "farm_ng/core/pipeline/context.h"

#include <boost/signals2.hpp>
#include <boost/signals2/connection.hpp>

#include <functional>

namespace farm_ng {

template <class TArg>
class Input;

/// A class that represents an ``Output`` type to be used in a ``Component``.
template <class TArg>
class Output {
 public:
  /// Default constructor, takes its `component`, and its `name`.
  ///
  /// Convention: If the variable name of the output is `out_foo_`, then
  ///             `name` shall be `foo`.
  Output(Component const* component, std::string const& name)
      : context_strand_(FARM_UNWRAP(component).getContextStrand()),
        uri_(FARM_UNWRAP(component).uri()) {
    uri_.query = FARM_FORMAT("out={}", name);
  }

  /// Default destructor
  ~Output() {}

  /// Call the actual function and send the result through the signal.
  void send(TArg const& out) { signal_(out); }

  /// Return the signal to the output function signature.
  boost::signals2::signal<void(TArg)>& signal() { return signal_; }

  /// Create a connection between the current signal and the receiving input.
  void connect(Input<TArg>& input) { input.connect(signal()); }

  /// Returns the unique uri of the output.
  [[nodiscard]] Uri const& uri() const { return uri_; }

 private:
  /// The stranded context of the function.
  ContextStrand context_strand_;
  /// The configuration structure for the class instance.
  Uri uri_;
  /// The signal to the function.
  boost::signals2::signal<void(TArg)> signal_;
};

}  // namespace farm_ng
