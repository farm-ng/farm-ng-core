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

#include "farm_ng/core/plotting/types.h"

namespace farm_ng {

namespace plotting {

ColoredRect ColoredRect::xLine(
    sophus2::Color const& color,
    double x,
    sophus2::RegionF64 const& y_range,
    double thickness) {
  return ColoredRect{
      .color = color,
      .region = sophus2::Region2F64::fromMinMax(
          {x - 0.5 * thickness, y_range.min()},
          {x + 0.5 * thickness, y_range.max()}),
  };
}
ColoredRect ColoredRect::yLine(
    sophus2::Color const& color,
    double y,
    sophus2::RegionF64 const& x_range,
    double thickness) {
  return ColoredRect{
      .color = color,
      .region = sophus2::Region2F64::fromMinMax(
          {x_range.min(), y - 0.5 * thickness},
          {x_range.max(), y + 0.5 * thickness}),
  };
}
}  // namespace plotting
}  // namespace farm_ng
