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

  struct StartToken {
    std::string event_name;
    TimePoint start_time;
  };

  /// Start a new, named timer.
  StartToken start(std::string event_name) {
    auto start_time = Clock::now();

    return StartToken{
        .event_name = event_name,
        .start_time = start_time,
    };
  }

  /// Stops the named timer and returns the duration.
  double stop(StartToken start_token) {
    std::scoped_lock lock(timers_mutex_);

    auto stop_time = Clock::now();

    // Find the bucket for the event name, create if it doesn't exist.
    Measurements& b = buckets_[start_token.event_name];

    std::chrono::duration<double> diff = (stop_time - start_token.start_time);
    double diff_seconds = diff.count();

    b.sliding_window.push_back(diff_seconds);
    if (b.sliding_window.size() > kWindowSize) {
      b.sliding_window.pop_front();
    }
    ++b.total_num;

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
      // intentionally done before sorting
      stats.last = sorted.back();
      stats.second_last = sorted.size() > 1 ? sorted[sorted.size() - 2] : -1.0;

      // now sort
      std::sort(sorted.begin(), sorted.end());

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

  /// Return container of stopwatch measurements.
  std::map<std::string, Measurements> getMeasurements() {
    std::scoped_lock lock(timers_mutex_);

    return buckets_;
  }

  /// Return measurements for a specific event.
  std::optional<Measurements> getMeasurementForEvent(std::string event) {
    std::scoped_lock lock(timers_mutex_);

    auto it = buckets_.find(event);
    if (it == buckets_.end()) {
      return std::nullopt;
    }
    return it->second;
  }

  /// Return container of stopwatch timer statistics.
  std::map<std::string, Stats> getStats() {
    std::scoped_lock lock(timers_mutex_);

    std::map<std::string, Stats> stats_vec;
    for (auto const& bucket : buckets_) {
      stats_vec.insert({bucket.first, bucket.second.calcStats()});
    }
    return stats_vec;
  }

  /// Prints statistics of stopwatch timers.
  std::string summaryString() {
    std::string event_name;
    for (auto const& [name, stats] : getStats()) {
      event_name += FARM_FORMAT(
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
    return event_name;
  }

 private:
  StopwatchSingleton() {}

  std::mutex timers_mutex_;
  std::map<std::string, Measurements> buckets_;
};

/// Stopwatch for a single, named timer.
struct ScopedTimer {
  /// Default constructor, requires timer name.
  ScopedTimer(std::string event_name) {
    token_ = StopwatchSingleton::getInstance().start(event_name);
  }

  /// Destructor that returns the timer duration.
  ~ScopedTimer() { StopwatchSingleton::getInstance().stop(this->token_); }

 private:
  StopwatchSingleton::StartToken token_;
};

}  // namespace farm_ng
