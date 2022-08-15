// Copyright (c) farm-ng, inc. All rights reserved.

#pragma once

#include <boost/asio/io_context.hpp>

#include <thread>

namespace farm_ng_core {

// NOTE: should we consider using boost::asio::thread_pool ?
// https://www.boost.org/doc/libs/master/doc/html/boost_asio/reference/thread_pool.html

/// Class representing a pool of threads connnected to a boost:asio::io_context.
class ThreadPool {
 public:
  /// Constructor that creates a boost:asio::io_context.
  ThreadPool();
  /// Constructor for an existing boost:asio::io_context.
  ThreadPool(std::shared_ptr<boost::asio::io_context> ctx);

  /// Signals to the context to stop.
  void stop();

  /// Start the threadpool, given a thread count.
  void start(size_t n_threads);

  /// Joins the threads in the pool.
  void join();

  /// Returns a shared pointer to the owned `io_context`.
  boost::asio::io_context& getAsioIoContext();

 private:
  std::shared_ptr<boost::asio::io_context> ctx_;
  std::vector<std::thread> threads_;
};

}  // namespace farm_ng_core
