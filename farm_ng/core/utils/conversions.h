
// Copyright (c) farm-ng, inc. All rights reserved.

#pragma once

#include "farm_ng/core/logging/logger.h"

#include <Eigen/Dense>
#include <sophus/se3.hpp>

namespace farm_ng_core {

/// Converts meters [m] to inches [in]
template <class T>
T inchesFromMeters(T m) {
  static_assert(!std::is_integral_v<T>, "Use a floating point type.");
  return m / 0.0254f;
}

/// Converts inches [in] to meters [m]
template <class T>
T metersFromInches(T inches) {
  static_assert(!std::is_integral_v<T>, "Use a floating point type.");
  return 0.0254 * inches;
}

/// Converts dots to meters [m]. Assumes 72 DPI.
template <class T>
T metersFromDots(T dot) {
  return metersFromInches(dot / 72.f);
}

/// Converts meters [m] to dots. Assumes 72 DPI.
template <class T>
T dotsFromMeters(T meters) {
  return inchesFromMeters(meters) * 72.f;
}

/// Converts 2d vector from meters [m] to dots. Assumes 72 DPI.
template <class T>
Sophus::Vector2<T> dotsFromMeters(const Sophus::Vector2<T>& meters) {
  return Sophus::Vector2<T>(
      dotsFromMeters(meters.x()), dotsFromMeters(meters.y()));
}

/// Converts radians to degrees
template <class T>
T degFromRad(T rad) {
  static_assert(!std::is_integral_v<T>, "Use a floating point type.");
  return T(180.0) / M_PI * rad;
}

/// Converts degrees to radians
template <class T>
T radFromDeg(T deg) {
  static_assert(!std::is_integral_v<T>, "Use a floating point type.");
  return M_PI / T(180.0) * deg;
}

/// GeoSpherical coordinates (latitude & longitude)
struct GeoSpherical {
  double longitude_rad;  // phi
  double latitude_rad;   // theta
  friend std::ostream& operator<<(
      std::ostream& os, const GeoSpherical& lat_lon);
};

constexpr double kKilometersFromMeters = 0.001;
constexpr double kMetersFromKilometers = 1000.0;

/// Converts 3d pose from kilometers [km] to meters [m]
template <class T>
Sophus::SE3<T> metersFromKilometers(const Sophus::SE3<T>& foo_pose_bar_km) {
  Sophus::SE3<T> foo_pose_bar_m = foo_pose_bar_km;
  foo_pose_bar_m.translation() *= kMetersFromKilometers;
  return foo_pose_bar_m;
}

/// Converts 3d pose from meters [m] to kilometers [km]
template <class T>
Sophus::SE3<T> kilometersFromMeters(const Sophus::SE3<T>& foo_pose_bar_m) {
  Sophus::SE3<T> foo_pose_bar_km = foo_pose_bar_m;
  foo_pose_bar_km.translation() *= kKilometersFromMeters;
  return foo_pose_bar_km;
}

/// Converts 3d point from cartesian [xyz] to spherical coords
///
/// Radius is calculated innately
GeoSpherical sphericalFromCartesian(const Eigen::Vector3d& point_world);

/// Converts 3d point from spherical coords to cartesian [xyz]
///
/// Requires the spherical radius
Eigen::Vector3d cartesianFromSpherical(
    const GeoSpherical& spherical, double radius);

/// Time in hours, minutes, and seconds
class HmsTime {
 public:
  /// Default constructor (empty)
  HmsTime() {}

  /// Set number of hours
  void setHours(int hours) {
    FARM_NG_CHECK_GE(hours, 0.0);
    this->hours_ = hours;
  }

  /// Set number of minutes
  void setMinutes(int minutes) {
    FARM_NG_CHECK_GE(minutes, 0);
    FARM_NG_CHECK_LE(minutes, 59);
    this->minutes_ = minutes;
  }

  /// Set number of seconds
  void setSeconds(double seconds) {
    FARM_NG_CHECK_GE(seconds, 0);
    FARM_NG_CHECK_LE(seconds, 59);
    this->seconds_ = seconds;
  }

  /// Returns hours
  int hours() const { return hours_; }

  /// Returns minutes
  int minutes() const { return minutes_; }

  /// Returns seconds
  double seconds() const { return seconds_; }

 private:
  int hours_ = 0.0;       // [0, infinity)
  int minutes_ = 0.0;     // [0, 59]
  double seconds_ = 0.0;  // [0, 59]
};

/// Converts seconds [s] to HMS time
HmsTime hmsTimeFromSeconds(double seconds);

/// Converts HMS time to seconds [s]
double secondsFromHmsTime(HmsTime time);

}  // namespace farm_ng_core
