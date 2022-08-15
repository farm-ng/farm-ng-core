// Copyright (c) farm-ng, inc. All rights reserved.

#pragma once

#include <core/logging/logger.h>
#include <my_enum/EnumO.h>

#include <chrono>
#include <map>

namespace farm_ng_core {

/// The steady timepoint type used in farm-ng codebase.
using SteadyTimepoint = std::chrono::
    time_point<std::chrono::steady_clock, std::chrono::steady_clock::duration>;

/// seconds from any std::chrono duration.
template <class Rep, class Period>
double secondsFromDuration(const std::chrono::duration<Rep, Period>& duration) {
  return 1e-6 * std::chrono::duration_cast<std::chrono::microseconds>(duration)
                    .count();
}

/// seconds from any std::chrono timepoint.
template <class Clock, class Duration>
double secondsFromTimepoint(
    const std::chrono::time_point<Clock, Duration>& time_point) {
  return secondsFromDuration(time_point.time_since_epoch());
}

/// SteadyTimepoint from seconds.
inline SteadyTimepoint timepointFromSeconds(double seconds) {
  return SteadyTimepoint(std::chrono::duration<int64_t, std::micro>(
      std::chrono::microseconds(int64_t(1000000 * seconds))));
}

/// std::chrono nanoseconds from seconds.
inline std::chrono::nanoseconds nanosFromSecondss(double seconds) {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::duration<double>(seconds));
}

/// Seconds from std::chrono nanoseconds.
inline double secondsFromNanos(std::chrono::nanoseconds nanos) {
  return std::chrono::duration_cast<std::chrono::duration<double>>(nanos)
      .count();
}

/// Type of clock.
MY_ENUM_O(ClockType, (monotonic));

/// Timestamp of a clock
struct Timestamp {
  /// Creates uninitialized timestamp. With great powers comes ...
  static Timestamp uninitialized() { return Timestamp{}; }

  /// Creates timestamp.
  Timestamp(std::string clock_name, double seconds, ClockType clock_type)
      : clock_name(std::move(clock_name)),
        seconds(seconds),
        clock_type(clock_type) {}

  /// Creates monotonic timestamp.
  static Timestamp makeMonotonic(std::string clock_name, double seconds) {
    return Timestamp(std::move(clock_name), seconds, ClockType::monotonic);
  }

  /// Creates a now stamp on the steady host clock.
  static Timestamp steadyClockNow(std::string clock_name) {
    return Timestamp::makeMonotonic(
        std::move(clock_name),
        secondsFromTimepoint(SteadyTimepoint::clock::now()));
  }

  /// name of the clock
  std::string clock_name;
  /// timepoint in seconds
  double seconds;
  /// the type of clock
  ClockType clock_type;

  bool operator<(const Timestamp& rhs) const {
    FARM_NG_CHECK_EQ(clock_name, rhs.clock_name);
    FARM_NG_CHECK_EQ(clock_type, rhs.clock_type);
    return seconds < rhs.seconds;
  }

 private:
  Timestamp() {}
};

struct TimeOffset {
  TimeOffset(
      Timestamp clock_a_epoch,
      Timestamp clock_b_epoch,
      double a_offset_b = 0.0,
      double a_skew_b = 1.0);
  Timestamp clock_a_epoch;
  Timestamp clock_b_epoch;
  double a_offset_b;
  double a_skew_b;

  const std::string& clockA() const;
  const std::string& clockB() const;
  TimeOffset inverse() const;

  Timestamp mapAtoB(const Timestamp& a) const;
};

class TimeOffsetMap {
 public:
  typedef std::map<std::pair<std::string, std::string>, TimeOffset> ContainerT;

  bool empty() const;
  void clear();

  void update(
      const std::string& clock_a,
      const std::string& clock_b,
      const TimeOffset& offset);

  void update(TimeOffset offset);

  TimeOffset timeOffsetAtoB(std::string clock_a, std::string clock_b) const;

  std::optional<TimeOffset> maybeTimeOffsetAtoB(
      std::string clock_a, std::string clock_b) const;

 private:
  ContainerT container_;
};

}  // namespace farm_ng_core
