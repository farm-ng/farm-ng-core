
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

#include "farm_ng/core/plotting/plotting.pb.h"
#include "farm_ng/core/plotting/types.h"
#include "farm_ng/core/proto_conv/traits.h"

namespace farm_ng {

FARM_PROTO_CONV_TRAIT(
    plotting::CurveResetPredicate, core::plotting::proto::CurveResetPredicate);
FARM_PROTO_CONV_TRAIT(plotting::LineType, core::plotting::proto::LineType);
FARM_PROTO_CONV_TRAIT(
    plotting::ColoredRect, core::plotting::proto::ColoredRect);
FARM_PROTO_CONV_TRAIT(
    std::deque<plotting::ColoredRect>,
    core::plotting::proto::RepeatedG0ColoredRect);

FARM_PROTO_CONV_TRAIT(
    std::deque<Eigen::Vector2d>, core::plotting::proto::XyPairsF64);

FARM_PROTO_CONV_TRAIT(
    std::deque<Eigen::Vector4d>, core::plotting::proto::XVecTupleF64);

FARM_PROTO_CONV_TRAIT(
    std::deque<plotting::Vec7d>, core::plotting::proto::XVecConvTupleF64);

FARM_PROTO_CONV_TRAIT(plotting::RectPlot, core::plotting::proto::RectPlot);
FARM_PROTO_CONV_TRAIT(plotting::Curve, core::plotting::proto::Curve);
FARM_PROTO_CONV_TRAIT(plotting::Vec3Curve, core::plotting::proto::Vec3Curve);
FARM_PROTO_CONV_TRAIT(
    plotting::Vec3CurveWithConfInterval,
    core::plotting::proto::Vec3CurveWithConfInterval);

FARM_PROTO_CONV_TRAIT(
    plotting::XCoordinateBounds, core::plotting::proto::XCoordinateBounds);
FARM_PROTO_CONV_TRAIT(
    plotting::YCoordinateBounds, core::plotting::proto::YCoordinateBounds);
FARM_PROTO_CONV_TRAIT(plotting::Bounds, core::plotting::proto::Bounds);
FARM_PROTO_CONV_TRAIT(plotting::Message, core::plotting::proto::Message);
FARM_PROTO_CONV_TRAIT(
    std::vector<plotting::Message>, core::plotting::proto::Messages);

}  // namespace farm_ng
