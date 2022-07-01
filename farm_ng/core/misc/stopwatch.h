// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "farm_ng/core/logging/logger.h"

#include <chrono>
#include <iostream>
#include <map>
#include <optional>
#include <stdexcept>

namespace farm_ng {

/// Stopwatch class for creating multiple, optionally concurrent, timers
class StopwatchSingleton {
 public:
  /// Returns an instance of the stopwatch timer.
  static StopwatchSingleton& getInstance() {
    // https://stackoverflow.com/a/17712497
    static StopwatchSingleton Static_S;
    return Static_S;
  }

  /// Start a new, named timer.
  void start(std::string str) {
    auto start_time = std::chrono::high_resolution_clock::now();

    Bucket& b = timers_[str];
    FARM_CHECK(!b.maybe_start, "{} is already started", str);
    b.maybe_start = start_time;
  }

  /// Stops the named timer and returns the duration.
  double stop(std::string str) {
    auto stop_time = std::chrono::high_resolution_clock::now();

    auto it = timers_.find(str);
    FARM_CHECK(it != timers_.end());
    Bucket& b = it->second;
    FARM_CHECK(b.maybe_start);
    std::chrono::duration<double> diff = stop_time - *b.maybe_start;
    b.maybe_start.reset();
    double d = diff.count();
    if (d < b.min) {
      b.min = d;
    }
    if (d > b.max) {
      b.max = d;
    }
    b.sum += d;
    ++b.num;
    return d;
  }

  /// Container for statistics on stopwatch timers.
  struct StopwatchStats {
    double mean;
    double min;
    double max;
    int num;
  };

  /// Return container of stopwatch timer statistics.
  std::map<std::string, StopwatchStats> getStats() {
    std::map<std::string, StopwatchStats> stats_vec;
    for (const auto& bucket : timers_) {
      StopwatchStats stats;
      stats.mean = bucket.second.sum / bucket.second.num;
      stats.num = bucket.second.num;
      stats.min = bucket.second.min;
      stats.max = bucket.second.max;
      stats_vec.insert({bucket.first, stats});
    }
    return stats_vec;
  }

  /// Prints statistics of stopwatch timers.
  void print() {
    for (const auto& stats : getStats()) {
      std::cout << stats.first << " mean time: " << stats.second.mean
                << " min time: " << stats.second.min
                << " max time: " << stats.second.max
                << " num: " << stats.second.num << std::endl;
    }
  }

 private:
  StopwatchSingleton() {}
  struct Bucket {
    std::optional<std::chrono::time_point<std::chrono::high_resolution_clock>>
        maybe_start;

    size_t num = 0;
    double sum = 0.0;
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::lowest();
  };

  std::map<std::string, Bucket> timers_;
};

/// Stopwatch for a single, named timer.
struct ScopedTimer {
  /// Default constructor, requires timer name.
  ScopedTimer(std::string str) : str_(str) {
    StopwatchSingleton::getInstance().start(str);
  }

  /// Destructor that returns the timer duration.
  ~ScopedTimer() { StopwatchSingleton::getInstance().stop(str_); }

 private:
  std::string str_;
};

}  // namespace farm_ng
