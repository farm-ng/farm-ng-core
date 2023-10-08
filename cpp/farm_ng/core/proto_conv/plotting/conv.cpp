
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

#include "farm_ng/core/proto_conv/plotting/conv.h"

#include "farm_ng/core/proto_conv/calculus/conv.h"
#include "farm_ng/core/proto_conv/color/conv.h"
#include "farm_ng/core/proto_conv/linalg/conv.h"
#include "farm_ng/core/proto_conv/std/conv.h"
#include "farm_ng/core/proto_conv/std/conv_impl_macro.ipp"
#include "farm_ng/core/proto_conv/struct/conv_impl_macro.ipp"

namespace farm_ng {

template <>
auto fromProt<core::plotting::proto::CurveResetPredicate>(
    core::plotting::proto::CurveResetPredicate const& proto)
    -> Expected<plotting::CurveResetPredicate> {
  plotting::CurveResetPredicate predicate;
  predicate.clear_x_smaller_than =
      proto.shall_clear() ? std::optional<double>(proto.clear_x_smaller_than())
                          : std::optional<double>();
  return predicate;
}

template <>
auto toProt<plotting::CurveResetPredicate>(
    plotting::CurveResetPredicate const& v)
    -> core::plotting::proto::CurveResetPredicate {
  core::plotting::proto::CurveResetPredicate proto;
  proto.set_shall_clear(bool(v.clear_x_smaller_than));
  if (v.clear_x_smaller_than) {
    proto.set_clear_x_smaller_than(v.clear_x_smaller_than.value());
  }
  return proto;
}

template <>
auto fromProt<core::plotting::proto::LineType>(
    core::plotting::proto::LineType const& proto)
    -> Expected<plotting::LineType> {
  plotting::LineType line_type;
  FARM_TRY_ASSERT(trySetFromString(line_type, proto.variant_name()));
  return line_type;
}

template <>
auto toProt<plotting::LineType>(plotting::LineType const& v)
    -> core::plotting::proto::LineType {
  core::plotting::proto::LineType proto;
  proto.set_variant_name(toString(v));
  return proto;
}

template <>
auto fromProt<core::plotting::proto::XyPairsF64>(
    core::plotting::proto::XyPairsF64 const& proto)
    -> Expected<std::deque<Eigen::Vector2d>> {
  size_t len = proto.num_pairs();
  FARM_TRY_ASSERT(
      2 * len * sizeof(double) == proto.data().size(),
      "{} != {}",
      2 * len * sizeof(double),
      proto.data().size());

  return std::deque<Eigen::Vector2d>{proto.data().begin(), proto.data().end()};
}

template <>
auto toProt<std::deque<Eigen::Vector2d>>(std::deque<Eigen::Vector2d> const& v)
    -> core::plotting::proto::XyPairsF64 {
  core::plotting::proto::XyPairsF64 proto;
  std::vector<Eigen::Vector2d> x_y_pairs(v.begin(), v.end());
  proto.set_num_pairs(x_y_pairs.size());
  proto.set_data(std::string(
      reinterpret_cast<char const*>(x_y_pairs.data()),
      x_y_pairs.size() * sizeof(Eigen::Vector2d)));
  return proto;
}

template <>
auto fromProt<core::plotting::proto::XVecTupleF64>(
    core::plotting::proto::XVecTupleF64 const& proto)
    -> Expected<std::deque<Eigen::Vector4d>> {
  size_t len = proto.num_tuples();
  FARM_TRY_ASSERT(
      4 * len * sizeof(double) == proto.data().size(),
      "{} != {}",
      4 * len * sizeof(double),
      proto.data().size());

  return std::deque<Eigen::Vector4d>{proto.data().begin(), proto.data().end()};
}

template <>
auto toProt<std::deque<Eigen::Vector4d>>(std::deque<Eigen::Vector4d> const& v)
    -> core::plotting::proto::XVecTupleF64 {
  core::plotting::proto::XVecTupleF64 proto;
  std::vector<Eigen::Vector4d> x_vec_pairs(v.begin(), v.end());
  proto.set_num_tuples(x_vec_pairs.size());
  proto.set_data(std::string(
      reinterpret_cast<char const*>(x_vec_pairs.data()),
      x_vec_pairs.size() * sizeof(Eigen::Vector4d)));
  return proto;
}

template <>
auto fromProt<core::plotting::proto::XVecConvTupleF64>(
    core::plotting::proto::XVecConvTupleF64 const& proto)
    -> Expected<std::deque<plotting::Vec7d>> {
  size_t len = proto.num_tuples();
  FARM_TRY_ASSERT(
      7 * len * sizeof(double) == proto.data().size(),
      "{} != {}",
      7 * len * sizeof(double),
      proto.data().size());

  return std::deque<plotting::Vec7d>{proto.data().begin(), proto.data().end()};
}

template <>
auto toProt<std::deque<plotting::Vec7d>>(std::deque<plotting::Vec7d> const& v)
    -> core::plotting::proto::XVecConvTupleF64 {
  core::plotting::proto::XVecConvTupleF64 proto;
  std::vector<plotting::Vec7d> x_vec_pairs(v.begin(), v.end());
  proto.set_num_tuples(x_vec_pairs.size());
  proto.set_data(std::string(
      reinterpret_cast<char const*>(x_vec_pairs.data()),
      x_vec_pairs.size() * sizeof(plotting::Vec7d)));
  return proto;
}

FARM_PROTO_CONV_IMPL(
    plotting::ColoredRect, core::plotting::proto::ColoredRect, (color, region));

FARM_CONV_IMPL_REPEATED(
    core::plotting::proto::RepeatedG0ColoredRect, plotting::ColoredRect);

FARM_PROTO_CONV_IMPL(
    plotting::RectPlot,
    core::plotting::proto::RectPlot,
    (bounds, colored_rects, path, reset));

FARM_PROTO_CONV_IMPL(
    plotting::Curve,
    core::plotting::proto::Curve,
    (bounds, color, line_type, path, reset, x_y_pairs));

FARM_PROTO_CONV_IMPL_FN(
    plotting::Vec3Curve,
    core::plotting::proto::Vec3Curve,
    (bounds, (color, SKIP), line_type, path, reset, x_vec_pairs),
    [](plotting::Vec3Curve&& s, core::plotting::proto::Vec3Curve const& proto)
        -> Expected<plotting::Vec3Curve> {
      if (proto.color_size() != 3) {
        return FARM_UNEXPECTED("color_size {} != 3", proto.color_size());
      }
      for (int i = 0; i < proto.color_size(); i++) {
        FARM_TRY(auto, c, fromProt(proto.color(i)));
        s.color[i] = c;
      }
      return std::move(s);
    },
    [](core::plotting::proto::Vec3Curve&& proto, plotting::Vec3Curve const& s) {
      for (auto const& c : s.color) {
        proto.add_color()->CopyFrom(toProt(c));
      }
      return std::move(proto);
    });

FARM_PROTO_CONV_IMPL_FN(
    plotting::Vec3CurveWithConfInterval,
    core::plotting::proto::Vec3CurveWithConfInterval,
    (bounds, (color, SKIP), (conf_color, SKIP), path, reset, x_vec_conf_tuples),
    [](plotting::Vec3CurveWithConfInterval&& s,
       core::plotting::proto::Vec3CurveWithConfInterval const& proto)
        -> Expected<plotting::Vec3CurveWithConfInterval> {
      if (proto.color_size() != 3) {
        return FARM_UNEXPECTED("color_size {} != 3", proto.color_size());
      }
      if (proto.conf_color_size() != 3) {
        return FARM_UNEXPECTED("color_size {} != 3", proto.color_size());
      }
      for (int i = 0; i < 3; i++) {
        FARM_TRY(auto, c, fromProt(proto.color(i)));
        FARM_TRY(auto, cc, fromProt(proto.conf_color(i)));

        s.color[i] = c;
        s.conf_color[i] = cc;
      }
      return std::move(s);
    },
    [](core::plotting::proto::Vec3CurveWithConfInterval&& proto,
       plotting::Vec3CurveWithConfInterval const& s) {
      for (auto const& c : s.color) {
        proto.add_color()->CopyFrom(toProt(c));
      }
      for (auto const& c : s.conf_color) {
        proto.add_conf_color()->CopyFrom(toProt(c));
      }
      return std::move(proto);
    });

FARM_PROTO_CONV_IMPL(
    plotting::XCoordinateBounds,
    core::plotting::proto::XCoordinateBounds,
    (max_x, len));

FARM_PROTO_CONV_IMPL(
    plotting::YCoordinateBounds,
    core::plotting::proto::YCoordinateBounds,
    (height, offset));

FARM_PROTO_CONV_IMPL(
    plotting::Bounds, core::plotting::proto::Bounds, (x_bounds, y_bounds));

template <>
auto fromProt<core::plotting::proto::Message>(
    core::plotting::proto::Message const& proto)
    -> Expected<plotting::Message> {
  plotting::Message msg;
  if (proto.has_curve()) {
    FARM_TRY(auto, m, fromProt(proto.curve()));
    msg = m;
    return msg;
  }
  if (proto.has_rects()) {
    FARM_TRY(auto, m, fromProt(proto.rects()));
    msg = m;
    return msg;
  }
  if (proto.has_vec3_curve()) {
    FARM_TRY(auto, m, fromProt(proto.vec3_curve()));
    msg = m;
    return msg;
  }
  if (proto.has_vec3_conf_curve()) {
    FARM_TRY(auto, m, fromProt(proto.vec3_conf_curve()));
    msg = m;
    return msg;
  }
  return msg;
}

template <>
auto toProt<plotting::Message>(plotting::Message const& v)
    -> core::plotting::proto::Message {
  core::plotting::proto::Message proto;
  visit_variant(
      v,
      [&](plotting::RectPlot const& v) { *proto.mutable_rects() = toProt(v); },
      [&](plotting::Curve const& v) { *proto.mutable_curve() = toProt(v); },
      [&](plotting::Vec3Curve const& v) {
        *proto.mutable_vec3_curve() = toProt(v);
      },
      [&](plotting::Vec3CurveWithConfInterval const& v) {
        *proto.mutable_vec3_conf_curve() = toProt(v);
      });
  return proto;
}

template <>
auto fromProt<core::plotting::proto::Messages>(
    core::plotting::proto::Messages const& proto)
    -> Expected<std::vector<plotting::Message>> {
  std::vector<plotting::Message> v;
  for (auto const& msg : proto.messages()) {
    FARM_TRY(auto, m, fromProt(msg));
    v.push_back(m);
  }
  return v;
}

template <>
auto toProt<std::vector<plotting::Message>>(
    std::vector<plotting::Message> const& v)
    -> core::plotting::proto::Messages {
  core::plotting::proto::Messages proto;
  for (auto const& msg : v) {
    proto.add_messages()->CopyFrom(toProt(msg));
  }
  return proto;
}

}  // namespace farm_ng
