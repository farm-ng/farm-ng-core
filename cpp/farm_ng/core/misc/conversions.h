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

#include <Eigen/Dense>
#include <farm_ng/core/logging/logger.h>
#include <sophus/concepts/point.h>
#include <sophus/lie/se3.h>

namespace farm_ng {

/// Converts meters [m] to inches [in]
template <sophus::concepts::RealScalarType T>
T inchesFromMeters(T m) {
  return m / 0.0254f;
}

/// Converts inches [in] to meters [m]
template <sophus::concepts::RealScalarType T>
T metersFromInches(T inches) {
  return 0.0254 * inches;
}

/// Converts feet [ft] to meters [m]
template <sophus::concepts::RealScalarType T>
T metersFromFeet(T feet) {
  return metersFromInches(12.0 * feet);
}

/// Converts meters [m] to feet [ft]
template <sophus::concepts::RealScalarType T>
T feetFromMeters(T meters) {
  return inchesFromMeters(meters) / 12.0;
}

/// Converts radians to degrees
template <sophus::concepts::RealScalarType T>
T degFromRad(T rad) {
  return T(180.0) / M_PI * rad;
}

/// Converts degrees to radians
template <sophus::concepts::RealScalarType T>
T radFromDeg(T deg) {
  return M_PI / T(180.0) * deg;
}

template <sophus::concepts::RealEigenDenseType TPoint>
auto radFromDeg(TPoint const& v_deg) {
  return radFromDeg(typename TPoint::Scalar(1.0)) * v_deg;
}

/// Converts radians to arc minutes (60th of a degree)
template <sophus::concepts::RealScalarType T>
T arcMinuteFromRad(T rad) {
  return T(60.) * degFromRad(rad);
}

/// Converts arc minutes (60th of a degree) to radians
template <sophus::concepts::RealScalarType T>
T radFromArcMinute(T arc_minute) {
  return radFromDeg(arc_minute / T(60.));
}

}  // namespace farm_ng
