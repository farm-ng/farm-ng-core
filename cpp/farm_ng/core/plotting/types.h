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

#include "farm_ng/core/struct/struct.h"
#include "sophus/calculus/region.h"
#include "sophus/color/color.h"

#include <deque>

#pragma once

namespace farm_ng {

namespace plotting {

/// Type of curves:
//
/// points:     plot dots along the curve
/// line_strip: plot line segments along the curve
FARM_ENUM(LineType, (points, line_strip));

/// Predicate for resetting a curve. If `clear_x_smaller_than` is set, then
/// points with x coordinate smaller than that value will be cleared from the
/// curve.
///
/// TODO: Redesign this to be more general and more usable.
struct CurveResetPredicate {
  /// Returns a predicate that does reset the curve.
  static CurveResetPredicate replace() {
    return CurveResetPredicate{
        .clear_x_smaller_than = std::numeric_limits<double>::max()};
  }

  /// Condition for clearing part of the curve.
  std::optional<double> clear_x_smaller_than = std::nullopt;
};

// Append to an existing curve in a given plot (or add new one if it does not
// exist already), with path being "plot_name/curve_name".
FARM_STRUCT(
    Curve,
    ((sophus::Color, color, {}),
     (LineType, line_type, {LineType::line_strip}),
     (std::filesystem::path, path, {}),
     (CurveResetPredicate, reset, {}),
     (std::deque<Eigen::Vector2d>, x_y_pairs, {})));

// No commas allowed in macro arguments, so we need to define this here.
using ColorArray3 = std::array<sophus::Color, 3>;
ColorArray3 constexpr kDefaultColorArray3 = {
    sophus::Color::red(), sophus::Color::green(), sophus::Color::blue()};
ColorArray3 constexpr kDefaultConfColorArray3 = {
    sophus::Color::orange(),
    sophus::Color::turquoise(),
    sophus::Color::violet()};

// Add a "3-vec curve" to the plot or append to exiting one with path being
// "plot_name/curve_name". If a curve with that name already exists, it will be
// overwritten.
//
// A "3-vec curve" are 3 curves described a list of (3+1)-dimensional tuples
// `x_vec_pairs`, where the first component of the tuples is the x coordinate
// for each curve, and the remaining components are the y coordinates.
FARM_STRUCT(
    Vec3Curve,
    ((ColorArray3, color, {kDefaultColorArray3}),
     (LineType, line_type, {LineType::line_strip}),
     (std::filesystem::path, path, {}),
     (CurveResetPredicate, reset, {}),
     (std::deque<Eigen::Vector4d>, x_vec_pairs, {})));

using Vec7d = Eigen::Matrix<double, 7, 1>;

// A vector of 3 curves, each with a confidence interval. The confidence
// interval is represented by an additive offset from the curve (e.g. 3x
// standard deviation).
FARM_STRUCT(
    Vec3CurveWithConfInterval,
    ((ColorArray3, color, {kDefaultColorArray3}),
     (ColorArray3, conf_color, {kDefaultConfColorArray3}),
     (LineType, line_type, {LineType::line_strip}),
     (std::filesystem::path, path, {}),
     (CurveResetPredicate, reset, {}),
     (std::deque<Vec7d>, x_vec_conf_tuples, {})));

/// A colored rectangle.
struct ColoredRect {
  /// Number of fields (for FARM_PROTO_CONV_IMPL to work)
  static int constexpr kNumFields = 2;

  /// Tuple of field names (for FARM_PROTO_CONV_IMPL to work)
  static std::array<std::string_view, kNumFields> constexpr kFieldNames = {
      "color", "region"};

  /// Tuple of field types (for FARM_PROTO_CONV_IMPL to work)
  using FieldTypes = std::tuple<sophus::Region2F64, sophus::Color>;
  static_assert(
      std::tuple_size_v<FieldTypes> == kNumFields,
      "Tuple size mismatch. Make sure to update kNumFields and kFieldNames when"
      " adding/modifying fields.");

  /// Vertical Line
  static ColoredRect xLine(
      sophus::Color const& color,
      double x,
      sophus::RegionF64 const& y_range,
      double thickness = 0.01);

  /// Horizontal line
  static ColoredRect yLine(
      sophus::Color const& color,
      double y,
      sophus::RegionF64 const& x_range,
      double thickness = 0.01);

  /// Color of the rectangle.
  sophus::Color color{};

  /// Region the rectangle describes.
  sophus::Region2F64 region = sophus::Region2F64::empty();
};

FARM_STRUCT(
    RectPlot,
    ((std::filesystem::path, path, {}),
     (std::deque<ColoredRect>, colored_rects, {}),
     (CurveResetPredicate, reset, {})));

FARM_STRUCT(
    XRange,
    ((sophus::RegionF64, range, {sophus::RegionF64::empty()}),
     (std::filesystem::path, path, {})));
FARM_STRUCT(
    YRange,
    ((sophus::RegionF64, range, {sophus::RegionF64::empty()}),
     (std::filesystem::path, path, {})));

using Message = std::variant<
    RectPlot,
    Curve,
    Vec3Curve,
    Vec3CurveWithConfInterval,
    XRange,
    YRange>;

}  // namespace plotting

}  // namespace farm_ng
