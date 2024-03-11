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

#include <farm_ng/core/logging/expected.h>
#include <sophus2/concepts/utils.h>

#include <deque>

namespace farm_ng {

namespace time_series {

/// Non-intrusive way to add getStamp function to a type needed for
/// StampedValue concept.
///
/// The default implementation assumes a member variable called stamp of type
/// double.
template <class TType>
auto getStamp(TType const& value) -> double {
  return value.stamp;
}

/// StampedValue is a type that has a timestamp.
template <class TType>
concept StampedValue = requires(TType m) {
  { getStamp<TType>(m) } -> sophus2::concepts::ConvertibleTo<double>;
};

/// Interpolate between two values.
template <class TType>
auto interpolate(
    TType const& foo_from_bar, TType const& foo_from_daz, double p = 0.5)
    -> TType;

/// Interpolative is a type which can be interpolated
template <class TType>
concept InterpolativeValue = StampedValue<TType> &&
    requires(TType m, double p) {
  { interpolate<TType>(m, m, p) } -> sophus2::concepts::ConvertibleTo<TType>;
};

/// New type to specify maximal allowed time gap for interpolation.
struct MaxGap {
  /// Construct from double.
  explicit MaxGap(double time_duration) : time_duration(time_duration) {}

  /// Maximal allowed time gap for interpolation.
  double time_duration;
};

/// New type to specify nearness to consider two values near in timeseries (e.g.
/// to skip interpolation calculation).
struct NearnessThreshold {
  /// Construct from double.
  explicit NearnessThreshold(double time_thr) : time_thr(time_thr) {}

  /// Nearness threshold.
  double time_thr;
};

}  // namespace time_series

/// TimeSeries is a container of StampedValue that are sorted by their
/// timestamp.
///
/// Invariant: time_ordered_series_ is sorted by time_series::getStamp.
///
/// Consequence: The invariant implies that we have a strict weak ordering
///              relation on the time stamps. Hence, it is not allowed to add
///              NAN timestamps to the time series.
///
/// TODO: Add std::ranges support to the API once we fully move to c++20.
template <time_series::StampedValue TValueWithStamp>
class TimeSeries {
 public:
  /// ValueWithStamp is a StampedValue.
  using ValueWithStamp = TValueWithStamp;
  /// Container type.
  using Container = std::deque<ValueWithStamp>;
  /// ConstIterator type.
  using ConstIterator = typename Container::const_iterator;

  /// Returns true if there are no Values in series.
  bool empty() const { return time_ordered_series_.empty(); }

  /// Adds ValueWithStamp to series.
  ///
  /// Precondition: stamp is not NAN.
  void insert(ValueWithStamp const& value) {
    double stamp = time_series::getStamp(value);
    if (std::isnan(stamp)) {
      FARM_WARN("Nan timestamp ignore");
      return;
    }
    auto it = findSmallestUpperBound(stamp);

    time_ordered_series_.insert(it, value);
  }

  /// Number of values in series.
  size_t size() const { return time_ordered_series_.size(); }

  /// access ValueWithStamp by index
  ///
  /// Precondition: idx < size()
  ValueWithStamp const& operator[](size_t idx) const {
    return time_ordered_series_[idx];
  }

  /// Find nearest value given ``stamp``.
  ///
  /// Precondition: stamp is not NAN.
  ///
  /// Returns error if times series is empty.
  Expected<ValueWithStamp> findNearest(double query_stamp) const {
    auto it_smallest_upper_bound = findSmallestUpperBound(query_stamp);
    if (it_smallest_upper_bound == cbegin() &&
        it_smallest_upper_bound == cend()) {
      return FARM_UNEXPECTED("Time series is empty.");
    }

    if (it_smallest_upper_bound == cbegin()) {
      // There is no element with stamp < query_stamp, return the smallest upper
      // bound.
      return *it_smallest_upper_bound;
    }
    auto it_greatest_lower_bound = std::prev(it_smallest_upper_bound);
    if (it_smallest_upper_bound == cend()) {
      // There is no element with stamp > query_stamp, return the greatest lower
      // bound.
      return *it_greatest_lower_bound;
    }

    double lb_abs_diff =
        std::abs(time_series::getStamp(*it_smallest_upper_bound) - query_stamp);
    double ub_abs_diff =
        std::abs(time_series::getStamp(*it_greatest_lower_bound) - query_stamp);

    return lb_abs_diff < ub_abs_diff ? *it_smallest_upper_bound
                                     : *it_greatest_lower_bound;
  }

  /// Bounds used in findBounds.
  struct Bounds {
    /// Largest lower bound.
    ValueWithStamp largest_lower_bound;
    /// Smallest upper bound.
    ValueWithStamp smallest_upper_bound;
  };

  /// Find bounds for query_stamp.
  Expected<Bounds> findBounds(double query_stamp) const {
    auto it_smallest_upper_bound = findSmallestUpperBound(query_stamp);
    if (it_smallest_upper_bound == cbegin() &&
        it_smallest_upper_bound == cend()) {
      return FARM_UNEXPECTED("Time series is empty.");
    }

    if (it_smallest_upper_bound == cbegin()) {
      return FARM_UNEXPECTED(
          "No element with stamp < query_stamp ({}). Min/Max {}/{}",
          query_stamp,
          time_series::getStamp(front()),
          time_series::getStamp(back()));
    }
    auto it_greatest_lower_bound = std::prev(it_smallest_upper_bound);
    if (it_smallest_upper_bound == cend()) {
      return FARM_UNEXPECTED(
          "No element with stamp > query_stamp ({}). Min/Max {}/{}",
          query_stamp,
          time_series::getStamp(front()),
          time_series::getStamp(back()));
    }

    return Bounds{
        .largest_lower_bound = *it_greatest_lower_bound,
        .smallest_upper_bound = *it_smallest_upper_bound};
  }

  /// Find nearest value given ``stamp`` within search radius ``threshold``.
  ///
  /// Precondition: stamp is not NAN.
  ///
  /// Returns error if |value.stamp - stamp| > threshold.
  Expected<ValueWithStamp> findNearestWithin(
      double stamp, double threshold) const {
    FARM_TRY(auto, nearest_value, findNearest(stamp));
    double abs_diff = std::abs(time_series::getStamp(nearest_value) - stamp);
    if (abs_diff > threshold) {
      return FARM_UNEXPECTED(
          "|value.stamp ({}) - stamp ({})| ({})  >  threshold ({})",
          time_series::getStamp(nearest_value),
          stamp,
          abs_diff,
          threshold);
    }
    return nearest_value;
  }

  /// Just for unit testing.
  ///
  /// Must return always true.
  bool testIsSortedInvariant() const {
    return std::is_sorted(
        time_ordered_series_.begin(),
        time_ordered_series_.end(),
        [](ValueWithStamp left, ValueWithStamp right) {
          return time_series::getStamp(left) < time_series::getStamp(right);
        });
  }

  /// Returns interpolated value from time series at query_stamp.
  ///
  /// Note: Only available if TValueWithStamp meets
  ///       time_series::InterpolativeValue concept.
  ///
  /// The following strategy is used:
  ///
  ///  1. If query_stamp is close to an existing value in the series, using the
  ///    ``nearness_threshold``, that exiting value is return directly.
  ///  2. Otherwise, an upper bound value and a lower bound value is found for
  ///     the query_stamp, and linearly interpolated result between the two
  ///     values is return, but only if ``|upper.t - lower.t| <=
  ///     maximal_time_gap``.
  ///  3. If there is no upper or lower bound, or if the ``|upper.t - lower.t| >
  ///     maximal_time_gap`` and error is return.
  std::enable_if_t<
      time_series::InterpolativeValue<TValueWithStamp>,
      Expected<TValueWithStamp>>
  interpolatedValue(
      double query_stamp,
      time_series::MaxGap maximal_time_gap = time_series::MaxGap(1.0),
      time_series::NearnessThreshold nearness_threshold =
          time_series::NearnessThreshold(1e-5)) const {
    auto expect_nearest =
        this->findNearestWithin(query_stamp, nearness_threshold.time_thr);
    if (expect_nearest) {
      return FARM_UNWRAP(expect_nearest);
    }

    return this->interpolatedValueImpl(
        query_stamp, maximal_time_gap.time_duration);
  }
  /// Returns first value in series.
  ///
  /// Precondition: series is not empty.
  ValueWithStamp const& front() const { return time_ordered_series_.front(); }

  /// Returns last value in series.
  ///
  /// Precondition: series is not empty.
  ValueWithStamp const& back() const { return time_ordered_series_.back(); }

  /// Removes all values from series.
  void clear() { return time_ordered_series_.clear(); }

  /// Removes first value from series.
  ///
  /// Precondition: series is not empty.
  void pop_front() { return time_ordered_series_.pop_front(); }

  /// Removes last value from series.
  ///
  /// Precondition: series is not empty.
  void pop_back() { return time_ordered_series_.pop_back(); }

  /// begin of series.
  ConstIterator begin() const { return time_ordered_series_.begin(); }

  /// begin of series.
  ConstIterator cbegin() const { return time_ordered_series_.cbegin(); }

  /// end of series.
  ConstIterator end() const { return time_ordered_series_.end(); }

  /// end of series.
  ConstIterator cend() const { return time_ordered_series_.cend(); }

 private:
  ConstIterator findSmallestUpperBound(double stamp) const {
    FARM_ASSERT(!std::isnan(stamp));

    // "Returns an iterator pointing to the first element in the range [first,
    // last) that does not satisfy element < value"
    return std::lower_bound(
        time_ordered_series_.begin(),
        time_ordered_series_.end(),
        stamp,
        [](ValueWithStamp const& lhs, double rhs) {
          return time_series::getStamp<ValueWithStamp>(lhs) < rhs;
        });
  }

  std::enable_if_t<
      time_series::InterpolativeValue<ValueWithStamp>,
      Expected<ValueWithStamp>>
  interpolatedValueImpl(double timestamp, double max_delta_t = 1.0) const {
    FARM_TRY(auto, bounds, this->findBounds(timestamp));

    auto lower = bounds.largest_lower_bound;
    auto upper = bounds.smallest_upper_bound;
    double lower_stamp = time_series::getStamp(lower);
    double upper_stamp = time_series::getStamp(upper);

    FARM_ASSERT_LE(lower_stamp, upper_stamp);
    double delta_t = std::abs(lower_stamp - upper_stamp);
    // negated comparisons to also catch NANSs
    if (!(delta_t < max_delta_t)) {
      return FARM_UNEXPECTED(
          "Delta of [{}, {}] = {}, which is too large (> {})",
          lower_stamp,
          upper_stamp,
          delta_t,
          max_delta_t);
    }

    // interpolation factor where p=0 means 100% of lower, and p=1.0 means
    // 100% of upper.
    double p = 1.0 - (upper_stamp - timestamp) / delta_t;
    FARM_ASSERT(p >= 0.0 && p <= 1.0, "p ({}) must in [0, 1].", p);

    // "proof" by example:
    //
    // case 1:
    // lower_stamp = timestamp
    // upper_stamp = timestamp + 0.5
    // hence, delta_t = 0.5
    //
    // p = 1.0 - (upper-timestamp)/0.5 = 1.0 - 0.5/0.5 = 0.0
    //
    // case 2:
    // lower_stamp = timestamp - 2.0
    // upper_stamp = timestamp
    // hence, delta_t = 2.0
    //
    // p = 1.0 - (upper-timestamp)/2.0 = 1.0 - 0.0/2.0 = 1.0

    return time_series::interpolate(lower, upper, p);
  }

  // Invariant: sorted, see details above.
  Container time_ordered_series_;
};

}  // namespace farm_ng
