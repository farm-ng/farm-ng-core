
// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng/core/utils/conversions.h"

#include "farm_ng/core/logging/logger.h"

#include <cmath>

namespace farm_ng_core {

std::ostream& operator<<(std::ostream& os, const GeoSpherical& lat_lon) {
  os << degFromRad(lat_lon.latitude_rad) << ", "
     << degFromRad(lat_lon.longitude_rad) << std::endl;
  return os;
}

GeoSpherical sphericalFromCartesian(const Eigen::Vector3d& point_world) {
  double radius = point_world.norm();
  FARM_NG_CHECK_GE(radius, 1e-5);
  double theta = std::acos(point_world.z() / radius);
  GeoSpherical spherical;
  spherical.longitude_rad = std::atan2(point_world.y(), point_world.x());
  spherical.latitude_rad = M_PI_2 - theta;
  return spherical;
}

Eigen::Vector3d cartesianFromSpherical(
    const GeoSpherical& spherical, double radius) {
  double theta = -spherical.latitude_rad + M_PI_2;
  double c_lat = std::cos(theta);
  double s_lat = std::sin(theta);
  double c_long = std::cos(spherical.longitude_rad);
  double s_long = std::sin(spherical.longitude_rad);
  return radius * Eigen::Vector3d(c_long * s_lat, s_long * s_lat, c_lat);
}

HmsTime hmsTimeFromSeconds(double seconds) {
  FARM_NG_CHECK_GE(seconds, 0.0);
  HmsTime time;
  time.setHours(seconds / 3600.0);
  double remaining_s = seconds - 3600.0 * time.hours();
  time.setMinutes(remaining_s / 60.0);
  time.setSeconds(std::fmod(remaining_s, 60.0));
  return time;
}

double secondsFromHmsTime(HmsTime time) {
  return 3600.0 * time.hours() + 60.0 * time.minutes() + time.seconds();
}

}  // namespace farm_ng_core
