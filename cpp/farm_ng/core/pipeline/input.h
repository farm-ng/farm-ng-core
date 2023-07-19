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

#include "farm_ng/core/pipeline/context.h"

#include <boost/signals2/connection.hpp>
#include <boost/signals2/signal.hpp>
#include <farm_ng/core/logging/logger.h>

#include <functional>

namespace farm_ng {

/// The configuration of an ``Input`` class.
class InputConfig {
 public:
  explicit InputConfig(size_t max_queue_length = 0);

  size_t max_queue_length;
};

/// A class that represents in ``Input`` type to be used in a ``Component``.
template <class TArg>
class Input {
 public:
  /// Default constructor, takes its `component`, its `name`, a callback
  /// function and its configuration.
  ///
  /// Convention: If the variable name of the input is `in_foo_`, then
  ///             `name` shall be `foo`.
  explicit Input(
      Component const* component,
      std::string const& name,
      std::function<void(TArg)> const& f,
      InputConfig const& config = InputConfig());

  /// Default destructor
  ~Input() {}

  /// Add a connection between the input signal with the internal signal_slot
  /// function
  Input<TArg>& connect(boost::signals2::signal<void(TArg)>& signal) {
    connections_.push_back(
        std::make_shared<boost::signals2::scoped_connection>(signal.connect(
            std::bind(&Input<TArg>::signalSlot, this, std::placeholders::_1))));
    return *this;
  }

  /// Call the actual function and send the result through the signal.
  void send(TArg const& value) { signalSlot(value); }

  /// Returns the unique uri of the input.
  [[nodiscard]] std::string const& uri() const { return uri_; }

 private:
  // signalSlot is called by the signal, on the sending thread.
  bool signalSlot(TArg value) {
    int count = count_.load();
    if (config_.max_queue_length != 0 && count > 0 &&
        size_t(count) >= config_.max_queue_length) {
      return false;
    }
    count_++;
    // request the strand to invoke the function and send the message value
    // NOTE: seems that `io_context::strand::post` it's deprecated and need
    // to be use `boost::asio::post` instead. See:
    // https://www.boost.org/doc/libs/1_78_0/doc/html/boost_asio/reference/io_context__strand/post.html
    context_strand_.getAsioStrand().post([this, value = std::move(value)] {
      this->strandedSlot(std::move(value));
    });
    return true;
  }

  // strandedSlot is called on the strand associated with ContextStrand
  // context_strand_, and it can be a generic place to track things like
  // function execution time, and manage the queue length.
  void strandedSlot(TArg value) {
    // begin timing
    function_(std::move(value));
    // end timing
    // e.g. context_strand_.report_timing(input_name_, timing)
    count_--;
  }

  /// The stranded context of the function.
  ContextStrand context_strand_;
  /// The uri of the input.
  std::string uri_;
  /// The configuration structure for the class instance.
  InputConfig config_;
  /// The callback function definition.
  std::function<void(TArg)> function_;
  /// The list contained connections between other functions.
  std::vector<std::shared_ptr<boost::signals2::scoped_connection>> connections_;
  /// A counter tracking the number of calls made to the function.
  std::atomic<int> count_;
};

}  // namespace farm_ng
