// Copyright (c) farm-ng, inc. All rights reserved.

#pragma once

#include "farm_ng_core/base/pipeline/input.h"
#include "farm_ng_core/base/pipeline/output.h"

namespace farm_ng_core {

// To keep track of calls that leave the management of
// the context: e.g. calls to the pipeline on the OAK. This will keep the
// Context alive until those calls return. Word of caution: there is no timeout
// so if DepthAI pipeline does not return a value to call decrement, the
// WorkGuard will exist forever.
class BookKeeper {
 public:
  typedef boost::asio::executor_work_guard<
      boost::asio::io_context::executor_type>
      WorkGuard;
  void increment(std::shared_ptr<boost::asio::io_context> ctx);

  void decrement();

 private:
  int count = 0;
  std::unique_ptr<WorkGuard> work_;
  std::mutex mtx_;
};

}  // namespace farm_ng_core
