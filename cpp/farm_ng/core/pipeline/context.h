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

#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>
// #include <boost/asio/executor_work_guard.hpp>
// #include <boost/asio/io_context.hpp>
// #include <boost/asio/io_context_strand.hpp>

#include <iostream>
#include <set>

namespace farm_ng {

class Component;

template <class TArg>
class Input;

template <class TArg>
class Output;

/// Contains the execution state context.
class Context {
 public:
  // using WorkGuard =
  // boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
  using WorkGuard = boost::asio::io_service::work;
  // boost::asio::io_contex
  using IoContext = boost::asio::io_service;
  using Strand = Context::IoContext::strand;

  /// Default constructor of a context.
  Context();

  /// Returns a reference to the owned `io_context`.
  IoContext& getAsioIoContext();

  /// Returns a shared pointer to the owned `io_context`.
  std::shared_ptr<IoContext> getAsioIoContextPtr();

  /// Run the `io_context`.
  void run();

  // Restart the context and run, to be used when context has stopped
  // i.e. when all posted jobs are complete and no workguard.
  void restartAndRun();

  /// clear the work for advanced use cases where you want to execute a finite
  /// number of tasks, e.g. in tests, or to run the pipeline to completion for a
  /// given input.
  void clearWorkGuard();

  /// Create a new pointer for the worker guard.
  void setWorkGuard();

 private:
  // A shared pointer to the owned `io_context`.
  std::shared_ptr<IoContext> ctx_;
  // A shared pointer to the owned worker guard.
  std::shared_ptr<std::unique_ptr<WorkGuard>> work_;
};

/// Contains a stranded context to post and dispatch handlers with
/// the guarantee that none of the handlers will execute concurrently.
///
/// More info:
/// https://www.boost.org/doc/libs/1_78_0/doc/html/boost_asio/reference/io_context__strand.html
class ContextStrand {
 public:
  /// Default constructor of a stranded context based on an allocated
  /// ``Context``.
  explicit ContextStrand(Context const& ctx);

  /// Return the `io_context` owned by the ``Context`` member.
  Context::IoContext& getAsioIoContext();

  /// Return the `io_context::strand`` owned by the  object instance.
  Context::Strand& getAsioStrand();

  /// Returns the owned ``Context`` by the component.
  [[nodiscard]] Context getContext() const;

 private:
  // The ``Context`` object owned by the class.
  Context ctx_;
  // A shared pointer to the ``io_context::strand`` owned by the class.
  std::shared_ptr<Context::Strand> strand_;
};

}  // namespace farm_ng
