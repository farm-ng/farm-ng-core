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

#include "farm_ng/core/pipeline/context.h"

#include "farm_ng/core/pipeline/component.h"

#include <memory>
namespace farm_ng {

Context::Context()
    : ctx_(std::make_shared<IoContext>()),
      work_(std::make_shared<std::unique_ptr<WorkGuard>>(
          std::make_unique<WorkGuard>(*ctx_)))
// new WorkGuard(boost::asio::make_work_guard(*ctx_))))
{}

Context::IoContext& Context::getAsioIoContext() { return *ctx_; }

std::shared_ptr<Context::IoContext> Context::getAsioIoContextPtr() {
  return ctx_;
}

void Context::run() { ctx_->run(); }

void Context::restartAndRun() {
  ctx_->reset();
  ctx_->run();
}

void Context::clearWorkGuard() { work_->reset(); }

void Context::setWorkGuard() { *work_ = std::make_unique<WorkGuard>(*ctx_); }

ContextStrand::ContextStrand(Context const& ctx)
    : ctx_(ctx),
      strand_(std::make_shared<Context::Strand>(ctx_.getAsioIoContext())) {}

Context::IoContext& ContextStrand::getAsioIoContext() {
  return ctx_.getAsioIoContext();
}

Context::Strand& ContextStrand::getAsioStrand() { return *strand_; }

/// Returns the owned ``Context`` by the component.
Context ContextStrand::getContext() const { return ctx_; }

}  // namespace farm_ng
