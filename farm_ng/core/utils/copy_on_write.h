// Copyright (c) farm-ng, inc. All rights reserved.
#pragma once

#include "farm_ng/core/logging/logger.h"

#include <functional>
#include <memory>

namespace farm_ng_core {

/// Copy on write class template.
template <class T>
class CopyOnWrite {
 public:
  /// Creates copy on write instance.
  CopyOnWrite(T&& t) : data_(std::make_shared<T>(t)) {
    FARM_NG_CHECK_EQ(data_.use_count(), 1);
  }

  const T& get() const { return *data_; }

  /// Mutation through a lambda.
  ///
  /// This will create a copy, if this is not unique.
  void mutate(std::function<void(T&)> mutator) {
    if (!isUnique()) {
      // create a copy
      data_ = std::make_shared<T>(*data_);
    }
    FARM_NG_CHECK(isUnique());
    mutator(*data_);
  }

  /// Is there only one instance?
  bool isUnique() const { return data_.use_count() == 1; }

 private:
  // class invariant: data_ != nullptr (hence data_.count()>= 1).
  std::shared_ptr<T> data_;
};

}  // namespace farm_ng_core
