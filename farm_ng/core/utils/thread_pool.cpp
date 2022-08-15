// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng/core/utils/thread_pool.h"

#include "farm_ng/core/logging/logger.h"

namespace farm_ng_core {

ThreadPool::ThreadPool() : ctx_(std::make_shared<boost::asio::io_context>()) {}

ThreadPool::ThreadPool(std::shared_ptr<boost::asio::io_context> ctx)
    : ctx_(ctx) {}

void ThreadPool::stop() { ctx_->stop(); }

// NOTE: consider this solution
// https://stackoverflow.com/questions/33529212/boost-asio-multiple-threads-and-multiple-io-service/33529876#33529876
void ThreadPool::start(size_t n_threads) {
  FARM_NG_CHECK_GT(n_threads, 0);
  FARM_NG_CHECK(threads_.empty(), "ThreadPool already started. Call Join().");

  ctx_->reset();

  for (size_t i = 0; i < n_threads; ++i) {
    threads_.emplace_back([ctx = ctx_]() { ctx->run(); });
  }
}

void ThreadPool::join() {
  for (auto& thread : threads_) {
    thread.join();
  }
  threads_.clear();
}

boost::asio::io_context& ThreadPool::getAsioIoContext() { return *ctx_; }

}  // namespace farm_ng_core
