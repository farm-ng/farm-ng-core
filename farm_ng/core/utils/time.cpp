// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng/core/utils/time.h"

#include <core/logging/logger.h>

namespace farm_ng_core {

TimeOffset::TimeOffset(
    Timestamp clock_a_epoch,
    Timestamp clock_b_epoch,
    double a_offset_b,
    double a_skew_b)
    : clock_a_epoch(clock_a_epoch),
      clock_b_epoch(clock_b_epoch),
      a_offset_b(a_offset_b),
      a_skew_b(a_skew_b) {}

const std::string& TimeOffset::clockA() const {
  return clock_a_epoch.clock_name;
}
const std::string& TimeOffset::clockB() const {
  return clock_b_epoch.clock_name;
}

TimeOffset TimeOffset::inverse() const {
  return TimeOffset(clock_b_epoch, clock_a_epoch, -a_offset_b, 1.0 / a_skew_b);
}

Timestamp TimeOffset::mapAtoB(const Timestamp& a) const {
  return Timestamp::makeMonotonic(
      clock_b_epoch.clock_name,
      (a.seconds - clock_a_epoch.seconds + a_offset_b) * a_skew_b +
          clock_b_epoch.seconds);
}

bool TimeOffsetMap::empty() const { return container_.empty(); }
void TimeOffsetMap::clear() { container_.clear(); }

void TimeOffsetMap::update(
    const std::string& clock_a,
    const std::string& clock_b,
    const TimeOffset& offset) {
  FARM_NG_CHECK_EQ(clock_a, offset.clockA());
  FARM_NG_CHECK_EQ(clock_b, offset.clockB());
  auto key = std::make_pair(clock_a, clock_b);
  auto [it, success] = container_.emplace(std::make_pair(key, offset));
  if (!success) {
    it->second = offset;
  }
}
void TimeOffsetMap::update(TimeOffset offset) {
  update(offset.clockA(), offset.clockB(), offset);
  update(offset.clockB(), offset.clockA(), offset.inverse());
}

TimeOffset TimeOffsetMap::timeOffsetAtoB(
    std::string clock_a, std::string clock_b) const {
  auto it = container_.find(std::make_pair(clock_a, clock_b));
  FARM_NG_CHECK(
      it != container_.end(),
      "timeoffset {} -> {} not found.",
      clock_a,
      clock_b);
  return it->second;
}

std::optional<TimeOffset> TimeOffsetMap::maybeTimeOffsetAtoB(
    std::string clock_a, std::string clock_b) const {
  auto it = container_.find(std::make_pair(clock_a, clock_b));
  if (it != container_.end()) {
    return it->second;
  }
  return std::nullopt;
}

}  // namespace farm_ng_core
