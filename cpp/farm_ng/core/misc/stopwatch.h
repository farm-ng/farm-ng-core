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

#pragma once

#include "farm_ng/core/logging/logger.h"

#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
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
    std::scoped_lock lock(timers_mutex_);
    auto start_time = std::chrono::high_resolution_clock::now();

    Bucket& b = timers_[str];
    if (b.maybe_start) {
      FARM_WARN("'{}' is already started", str);
    }
    b.maybe_start = start_time;
  }

  /// Stops the named timer and returns the duration.
  double stop(std::string str) {
    std::scoped_lock lock(timers_mutex_);

    auto stop_time = std::chrono::high_resolution_clock::now();

    auto it = timers_.find(str);
    if (it == timers_.end()) {
      FARM_WARN("{} is not started, but 'stop' was called!", str);
      return 0.0;
    }
    Bucket& b = it->second;

    if (!b.maybe_start) {
      FARM_WARN("Tried stopping '{}', but 'stop' was called already.", str);
      return 0.0;
    }
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
    /// Mean time of timer.
    double mean;
    /// Minimum time of timer.
    double min;
    /// Maximum time of timer.
    double max;
    /// Number of times timer was called.
    int num;
  };

  /// Return container of stopwatch timer statistics.
  std::map<std::string, StopwatchStats> getStats() {
    std::scoped_lock lock(timers_mutex_);

    std::map<std::string, StopwatchStats> stats_vec;
    for (auto const& bucket : timers_) {
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
    for (auto const& stats : getStats()) {
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
  std::mutex timers_mutex_;
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
