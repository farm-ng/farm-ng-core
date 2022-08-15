// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng/core/utils/conversions.h"

#include <gtest/gtest.h>

#include <random>

using namespace farm_ng_core;

TEST(sperical, roundTrip) {
  double eps = 1e-8;
  for (int i = 0; i < 20; ++i) {
    Eigen::Vector3d point_world = Eigen::Vector3d::Random();
    double radius = point_world.norm();
    GeoSpherical spherical = sphericalFromCartesian(point_world);
    Eigen::Vector3d point2_world = cartesianFromSpherical(spherical, radius);
    for (int j = 0; j < 3; ++j) {
      EXPECT_NEAR(point_world[j], point2_world[j], eps);
    }
  }

  std::default_random_engine generator;
  std::uniform_real_distribution<double> longitude_dist(-180, 180);
  std::uniform_real_distribution<double> latitude_dist(-90, 90);

  for (int i = 0; i < 20; ++i) {
    GeoSpherical spherical;
    spherical.longitude_rad = radFromDeg(longitude_dist(generator));
    spherical.latitude_rad = radFromDeg(latitude_dist(generator));
    Eigen::Vector3d point_world = cartesianFromSpherical(spherical, 1.0);
    GeoSpherical spherical2 = sphericalFromCartesian(point_world);
    EXPECT_NEAR(
        degFromRad(spherical.longitude_rad),
        degFromRad(spherical2.longitude_rad),
        eps);
    EXPECT_NEAR(
        degFromRad(spherical.latitude_rad),
        degFromRad(spherical2.latitude_rad),
        eps);
  }
}

TEST(time, unit) {
  double eps = 1e-8;

  HmsTime time_ten_s;
  time_ten_s.setSeconds(10.0);
  double ten_s = secondsFromHmsTime(time_ten_s);
  EXPECT_NEAR(ten_s, 10.0, eps);

  HmsTime time_seventy_one_s;
  time_seventy_one_s.setMinutes(1);
  time_seventy_one_s.setSeconds(11);
  double seventy_one_s = secondsFromHmsTime(time_seventy_one_s);
  EXPECT_NEAR(seventy_one_s, 71.0, eps);

  HmsTime time_5k_s;
  time_5k_s.setHours(1);
  time_5k_s.setMinutes(23.0);
  time_5k_s.setSeconds(20.0);
  double fivek_s = secondsFromHmsTime(time_5k_s);
  EXPECT_NEAR(fivek_s, 5000, eps);

  // round trip
  std::default_random_engine generator;
  std::uniform_real_distribution<double> dist(0.0, 50000.0);
  for (int i = 0; i < 20; ++i) {
    double seconds = dist(generator);
    HmsTime time = hmsTimeFromSeconds(seconds);
    double seconds2 = secondsFromHmsTime(time);
    EXPECT_NEAR(seconds, seconds2, eps);
  }
}
