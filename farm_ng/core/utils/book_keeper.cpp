// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng/core/utils/book_keeper.h"

#include "farm_ng_core/base/pipeline/input.h"
#include "farm_ng_core/base/pipeline/output.h"

namespace farm_ng_core {

// To keep track of calls that leave the management of
// the context: e.g. calls to the pipeline on the OAK. This will keep the
// Context alive until those calls return. Word of caution: there is no timeout
// so if DepthAI pipeline does not return a value to call decrement, the
// WorkGuard will exist forever.
void BookKeeper::increment(std::shared_ptr<boost::asio::io_context> ctx) {
  std::lock_guard<std::mutex> lock(mtx_);
  count++;
  if (!work_) {
    work_ = std::make_unique<WorkGuard>(boost::asio::make_work_guard(*ctx));
  }
}

void BookKeeper::decrement() {
  std::lock_guard<std::mutex> lock(mtx_);
  count--;
  FARM_NG_CHECK_GE(count, 0);
  if (count == 0) {
    work_.reset();
  }
}

}  // namespace farm_ng_core
