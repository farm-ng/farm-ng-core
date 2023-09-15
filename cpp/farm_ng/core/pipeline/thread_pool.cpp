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

#include "farm_ng/core/pipeline/thread_pool.h"

#include <farm_ng/core/logging/logger.h>

namespace farm_ng {

ThreadPool::ThreadPool() : ctx_(std::make_shared<Context::IoContext>()) {}

ThreadPool::ThreadPool(std::shared_ptr<Context::IoContext> ctx) : ctx_(ctx) {}

void ThreadPool::stop() { ctx_->stop(); }

// NOTE: consider this solution
// https://stackoverflow.com/questions/33529212/boost-asio-multiple-threads-and-multiple-io-service/33529876#33529876
void ThreadPool::start(size_t n_threads) {
  FARM_ASSERT_GT(n_threads, 0);
  FARM_ASSERT(threads_.empty(), "ThreadPool already started. Call Join().");

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

Context::IoContext& ThreadPool::getAsioIoContext() { return *ctx_; }

}  // namespace farm_ng
