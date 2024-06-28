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
#include <deque>
#include <iostream>
#include <map>
#include <mutex>
#include <optional>

namespace farm_ng {

/// Stopwatch class for creating multiple, optionally concurrent, timers
class StopwatchSingleton {
 public:
  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<Clock>;

  static size_t constexpr kWindowSize = 30;

  /// Returns an instance of the stopwatch timer.
  static StopwatchSingleton& getInstance() {
    // https://stackoverflow.com/a/17712497
    static StopwatchSingleton Static_S;
    return Static_S;
  }

  /// Start a new, named timer.
  void start(std::string str) {
    std::scoped_lock lock(timers_mutex_);
    auto start_time = Clock::now();

    Bucket& b = timers_[str];

    std::thread::id this_id = std::this_thread::get_id();

    auto& maybe_start = b.maybe_start[this_id];
    if (maybe_start) {
      FARM_WARN("'{}' is already started (on thread {})", str, this_id);
    }
    maybe_start = start_time;
  }

  /// Stops the named timer and returns the duration.
  double stop(std::string str) {
    std::scoped_lock lock(timers_mutex_);

    auto stop_time = Clock::now();

    auto it = timers_.find(str);
    if (it == timers_.end()) {
      FARM_WARN(
          "{} was not started on any thread, but 'stop' was called!", str);
      return 0.0;
    }
    Bucket& b = it->second;

    auto& maybe_start = b.maybe_start[std::this_thread::get_id()];

    if (!maybe_start) {
      FARM_WARN(
          "Tried stopping '{}' (on thread {}), but 'stop' was called already.",
          std::this_thread::get_id(),
          str);
      return 0.0;
    }

    std::chrono::duration<double> diff = (stop_time - *maybe_start);
    double diff_seconds = diff.count();
    maybe_start.reset();

    b.data.sliding_window.push_back(diff_seconds);
    if (b.data.sliding_window.size() > kWindowSize) {
      b.data.sliding_window.pop_front();
    }
    ++b.data.total_num;

    return diff_seconds;
  }

  struct Stats {
    double last = -1.0;
    double second_last = -1.0;
    double min = -1.0;
    double max = -1.0;
    double median = -1.0;
    size_t total_num = 0;
  };

  /// Container for statistics on stopwatch timers.
  struct Measurements {
    Stats calcStats() const {
      Stats stats;
      stats.total_num = total_num;
      if (sliding_window.empty()) {
        return stats;
      }

      std::vector<double> sorted;
      for (auto const& t : sliding_window) {
        sorted.push_back(t);
      }
      std::sort(sorted.begin(), sorted.end());

      stats.last = sorted.back();
      stats.second_last = sorted.size() > 1 ? sorted[sorted.size() - 2] : -1.0;
      stats.min = sorted.front();
      stats.max = sorted.back();
      stats.median = sorted[sorted.size() / 2];
      return stats;
    }

    /// total number of timer starts.
    size_t total_num = 0;
    /// total time elapsed.
    std::deque<double> sliding_window;
  };

  /// Return container of stopwatch timer statistics.
  std::map<std::string, Measurements> getMeasurements() {
    std::scoped_lock lock(timers_mutex_);

    std::map<std::string, Measurements> stats_vec;
    for (auto const& bucket : timers_) {
      stats_vec.insert({bucket.first, bucket.second.data});
    }
    return stats_vec;
  }

  /// Return container of stopwatch timer statistics.
  std::map<std::string, Stats> getStats() {
    std::scoped_lock lock(timers_mutex_);

    std::map<std::string, Stats> stats_vec;
    for (auto const& bucket : timers_) {
      stats_vec.insert({bucket.first, bucket.second.data.calcStats()});
    }
    return stats_vec;
  }

  /// Prints statistics of stopwatch timers.
  std::string summaryString() {
    std::string str;
    for (auto const& [name, stats] : getStats()) {
      str += FARM_FORMAT(
          "dt `{}` - #{}, LAST: {:.3f} [2nd last {:.3f}], MEDIAN: {:3f} [{:3f} "
          "- "
          "{:3f}] \n",
          name,
          stats.total_num,
          stats.last,
          stats.second_last,
          stats.median,
          stats.min,
          stats.max);
    }
    return str;
  }

 private:
  StopwatchSingleton() {}

  struct Bucket {
    std::unordered_map<std::thread::id, std::optional<TimePoint>> maybe_start;
    Measurements data;
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
