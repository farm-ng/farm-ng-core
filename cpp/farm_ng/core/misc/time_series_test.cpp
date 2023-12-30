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

#include "farm_ng/core/misc/time_series.h"

#include "farm_ng/core/misc/conversions.h"
#include "farm_ng/core/logging/fmt_eigen.h"
#include "sophus/interp/interpolate.h"

#include <gtest/gtest.h>
#include <sophus/lie/rotation3.h>

#include <random>

namespace farm_ng {
struct StampedFoo {
  double t;
};

struct StampedBar {
  double stamp;
};

namespace time_series {

template <>
auto getStamp<StampedFoo>(StampedFoo const& foo) -> double {
  return foo.t;
}
}  // namespace time_series

TEST(time_series, empty) {
  {
    TimeSeries<StampedFoo> time_series;
    FARM_ASSERT(time_series.empty());

    auto maybe_value = time_series.findNearest(1.0);
    EXPECT_FALSE(maybe_value);
  }
  {
    TimeSeries<StampedBar> time_series;
    FARM_ASSERT(time_series.empty());

    auto maybe_value = time_series.findNearest(1.0);
    EXPECT_FALSE(maybe_value);
  }
}

TEST(time_series, sorted_invariant) {
  std::default_random_engine generator;
  std::uniform_real_distribution<double> value_distribution(-10, 10.0);
  std::uniform_int_distribution<size_t> size_distribution(1, 100);

  for (int i = 0; i < 10; i++) {
    TimeSeries<StampedFoo> time_series;

    size_t num = size_distribution(generator);
    for (size_t n = 0; n < size_distribution(generator); n++) {
      time_series.insert(StampedFoo{value_distribution(generator)});
    }
    FARM_ASSERT(!time_series.empty());

    FARM_ASSERT(time_series.testIsSortedInvariant());
  }
}

TEST(time_series, find_nearest) {
  TimeSeries<StampedFoo> time_series;

  time_series.insert(StampedFoo{3.0});
  time_series.insert(StampedFoo{1.0});
  time_series.insert(StampedFoo{2.0});
  FARM_ASSERT(time_series.testIsSortedInvariant());

  FARM_ASSERT(!time_series.empty());

  {
    auto maybe_value = time_series.findNearest(1.4);
    auto value = FARM_UNWRAP(maybe_value);
    EXPECT_EQ(time_series::getStamp(value), 1.0);
  }

  {
    auto maybe_value = time_series.findNearest(0.9);
    auto value = FARM_UNWRAP(maybe_value);
    EXPECT_EQ(time_series::getStamp(value), 1.0);
  }

  {
    auto maybe_value = time_series.findNearest(1.6);
    auto value = FARM_UNWRAP(maybe_value);
    EXPECT_EQ(time_series::getStamp(value), 2.0);
  }

  {
    auto maybe_value = time_series.findNearest(3.3);
    auto value = FARM_UNWRAP(maybe_value);
    EXPECT_EQ(time_series::getStamp(value), 3.0);
  }
}

TEST(time_series, find_near_using_threshold) {
  TimeSeries<StampedFoo> time_series;

  time_series.insert(StampedFoo{3.0});
  time_series.insert(StampedFoo{1.0});
  time_series.insert(StampedFoo{2.0});
  FARM_ASSERT(time_series.testIsSortedInvariant());

  FARM_ASSERT(!time_series.empty());

  {
    auto maybe_value = time_series.findNearestWithin(1.4, 0.5);
    auto value = FARM_UNWRAP(maybe_value);
    EXPECT_EQ(time_series::getStamp(value), 1.0);
  }

  {
    auto maybe_value = time_series.findNearestWithin(0.9, 0.5);
    auto value = FARM_UNWRAP(maybe_value);
    EXPECT_EQ(time_series::getStamp(value), 1.0);
  }

  {
    auto maybe_value = time_series.findNearestWithin(1.6, 0.5);
    auto value = FARM_UNWRAP(maybe_value);
    EXPECT_EQ(time_series::getStamp(value), 2.0);
  }

  {
    auto maybe_value = time_series.findNearestWithin(3.3, 0.5);
    auto value = FARM_UNWRAP(maybe_value);
    EXPECT_EQ(time_series::getStamp(value), 3.0);
  }

  {
    auto maybe_value = time_series.findNearestWithin(1.4, 0.1);
    EXPECT_FALSE(maybe_value.has_value());
  }

  {
    auto maybe_value = time_series.findNearestWithin(1.6, 0.1);
    EXPECT_FALSE(maybe_value.has_value());
  }
}

TEST(time_series, size) {
  TimeSeries<StampedFoo> time_series;

  time_series.insert(StampedFoo{3.0});
  time_series.insert(StampedFoo{1.0});
  time_series.insert(StampedFoo{2.0});

  FARM_ASSERT(!time_series.empty());
  EXPECT_EQ(time_series.size(), 3);
}

TEST(time_series, index) {
  TimeSeries<StampedFoo> time_series;

  time_series.insert(StampedFoo{1.0});
  time_series.insert(StampedFoo{2.0});
  time_series.insert(StampedFoo{3.0});

  FARM_ASSERT(!time_series.empty());
  EXPECT_EQ(time_series[0].t, 1.0);
  EXPECT_EQ(time_series[1].t, 2.0);
  EXPECT_EQ(time_series[2].t, 3.0);
}

struct StampedInterpolative {
  double t;
  sophus::Rotation3F64 world_from_foo;
  Eigen::Vector3d angular_velocity;
};

namespace time_series {

template <>
auto getStamp<StampedInterpolative>(StampedInterpolative const& foo) -> double {
  return foo.t;
}

template <>
auto interpolate<StampedInterpolative>(
    StampedInterpolative const& lhs, StampedInterpolative const& rhs, double p)
    -> StampedInterpolative {
  StampedInterpolative result;
  result.t = (1.0 - p) * lhs.t + p * rhs.t;
  result.world_from_foo =
      sophus::interpolate(lhs.world_from_foo, rhs.world_from_foo, p);
  result.angular_velocity =
      (1.0 - p) * lhs.angular_velocity + p * rhs.angular_velocity;
  return result;
}

TEST(interpolative_time_series, unit) {
  TimeSeries<StampedInterpolative> series;

  FARM_ASSERT(!series.interpolatedValue(0.1));
  StampedInterpolative null;
  null.t = 0;
  null.angular_velocity.setZero();
  StampedInterpolative one = null;
  one.t = 1.0;
  one.angular_velocity.x() = 1.0;

  series.insert(null);
  series.insert(one);

  // Test for error if gap is larger MaxGap.
  FARM_ASSERT(!series.interpolatedValue(0.1, time_series::MaxGap(0.9)));

  // test what that we get a result if gap between stamps is smaller MaxGap.
  FARM_ASSERT(series.interpolatedValue(0.1, time_series::MaxGap(1.1)));

  // Expect error, since -0.01 is outside of bound, and also not near to null.t
  // according to nearness threshold.
  FARM_ASSERT(!series.interpolatedValue(
      -0.01, time_series::MaxGap(0.5), time_series::NearnessThreshold(0.001)));

  // Expect success, since -0.01 is outside of bound, but near to null.t
  // according to nearness threshold.
  FARM_ASSERT(series.interpolatedValue(
      -0.01, time_series::MaxGap(0.5), time_series::NearnessThreshold(0.02)));
}

TEST(interpolative_time_series, regression) {
  std::vector<StampedInterpolative> values;
  double t = 0.0;
  double delta_t = 0.1;

  double acceleration_x = 0.1;
  double acceleration_y = 0.033;

  Eigen::Vector3d angular_velocity(0.0, 0.0, 0.0);
  sophus::Rotation3F64 world_from_foo;

  TimeSeries<StampedInterpolative> series;

  while (t < 2.0) {
    StampedInterpolative value;
    value.t = t;
    value.world_from_foo =
        world_from_foo * sophus::Rotation3F64::exp(delta_t * angular_velocity);
    value.angular_velocity = angular_velocity;
    angular_velocity.x() += delta_t * acceleration_x;
    angular_velocity.y() += delta_t * acceleration_y;

    t += delta_t;

    if (values.size() % 3 == 0) {
      series.insert(value);
    }
    values.push_back(value);

    acceleration_x += 0.0001;
  }
  series.insert(values.back());
  FARM_INFO("{}, {}", values.size(), series.size());

  for (StampedInterpolative const& value : values) {
    FARM_INFO("t: {}", value.t);
    auto expect_value = series.interpolatedValue(value.t);
    auto interpolated = FARM_UNWRAP(expect_value);

    FARM_INFO(
        "true.angular_velocity:\n {}", value.angular_velocity.transpose());
    FARM_INFO(
        "interp.angular_velocity:\n {}",
        interpolated.angular_velocity.transpose());

    FARM_INFO(
        "true.world_from_foo:\n {}", value.world_from_foo.compactMatrix());
    FARM_INFO(
        "interp.world_from_foo:\n {}",
        interpolated.world_from_foo.compactMatrix());

    static double const kEps = sophus::kEpsilonF32;
    FARM_ASSERT_WITHIN_REL(interpolated.t, value.t, kEps);
    FARM_ASSERT_WITHIN_REL(
        interpolated.angular_velocity, value.angular_velocity, 0.01);
    static double const kAngularThrDeg = 0.01;
    FARM_ASSERT_WITHIN_REL(
        degFromRad(
            (interpolated.world_from_foo * value.world_from_foo.inverse())
                .log()[0]),
        0.0,
        kAngularThrDeg);
  }
}

}  // namespace time_series

}  // namespace farm_ng
