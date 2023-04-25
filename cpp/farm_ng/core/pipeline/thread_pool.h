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

//#include <boost/asio/io_context.hpp>
#include <farm_ng/core/pipeline/context.h>

#include <thread>

namespace farm_ng {

// NOTE: should we consider using boost::asio::thread_pool ?
// https://www.boost.org/doc/libs/master/doc/html/boost_asio/reference/thread_pool.html

/// Class representing a pool of threads connected to a boost:asio::io_context.
class ThreadPool {
 public:
  /// Constructor that creates a boost:asio::io_context.
  ThreadPool();
  /// Constructor for an existing boost:asio::io_context.
  ThreadPool(std::shared_ptr<Context::IoContext> ctx);

  /// Signals to the context to stop.
  void stop();

  /// Start the threadpool, given a thread count.
  void start(size_t n_threads);

  /// Joins the threads in the pool.
  void join();

  /// Returns a shared pointer to the owned `io_context`.
  Context::IoContext& getAsioIoContext();

 private:
  std::shared_ptr<Context::IoContext> ctx_;
  std::vector<std::thread> threads_;
};

}  // namespace farm_ng
