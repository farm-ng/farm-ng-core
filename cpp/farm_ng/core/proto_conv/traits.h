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

#include "farm_ng/core/logging/expected.h"

namespace farm_ng {

/// Trait to associate one concrete Proto type with given C++ type `TCpp`.
///
/// Helper trait for the `toProt` function template.
template <class TCpp>
struct ToProtoTrait;

/// Trait to associate one concrete Cpp type with given proto type `TProto`.
///
/// Helper trait for the `fromProt` function template.
template <class TProto>
struct FromProtoTrait;

/// Function template to convert a C++ object to a proto object.
///
/// Example:
///
///     Eigen::Vector3d v(1, 2, 3);
///     proto::Vec3F64 proto = toProt(v);
///
/// Note: The return type is deduced from the argument type using the
/// `FromProtoTrait` trait.
///
/// TODO: Redesign to allow {1:many} mappings.
///
/// * We always do know the c++ type and the proto type. We can use that to
/// * generate a function that converts from the proto type to the c++ type.
/// * We want a syntax like this:
/// *
/// *    auto v = fromProt<Eigen::Vector3d>(proto);
/// *
/// * where fromProt would have the following signature:
/// *
/// *    template <class TCpp, class TProto>
/// *    auto fromProt(TProto const& proto) -> Expected<TCpp>;
/// *
/// * Note: This shall also eliminate the indirection FromProtImpl in
/// struct/conv_impl_macro.h.
template <class TProto>
auto fromProt(TProto const& proto)
    -> Expected<typename FromProtoTrait<TProto>::CppType>;

/// Function template to convert a proto object to a C++ object.
///
/// Example:
///
///     proto::Vec3F64 proto;
///     proto.set_x(1);
///     proto.set_y(2);
///     proto.set_z(3);
///     Eigen::Vector3d v = fromProt(proto);
///
/// Note: The return type is deduced from the argument type using the
/// `ToProtoTrait` trait.
///
/// TODO:
/// * Slight redesign to allow partial specialization e.g. to have a
/// * single function definition to map std::vector<float> and std::deque<float>
/// * to a common proto type.
/// *
/// * Note: We will kepp a {many:1} mapping. In particular, any given C++ type
/// * can be mapped to at most one proto type.
template <class TCpp>
auto toProt(TCpp const& cpp) -> typename ToProtoTrait<TCpp>::ProtoType;

/// Convenience macro to define the `ToProtoTrait` and `FromProtoTrait` as well
/// as the `toProt` and `fromProt` function templates instantiation declarations
/// for a given C++ type `TCpp` and a given proto type `TProto`. Hence, this
/// macro is typically used in a header file.
///
/// Example:
///
///     FARM_PROTO_CONV_TRAIT(Eigen::Vector3d, proto::Vec3F64);
///
/// produces the following code:
///
///     template <>
///     struct ToProtoTrait<Eigen::Vector3d> {
///       using ProtoType = proto::Vec3F64;
///     };
///
///     template <>
///     struct FromProtoTrait<proto::Vec3F64> {
///       using CppType = Eigen::Vector3d;
///     };
///
///     template <>
///     auto fromProt<proto::Vec3F64>(proto::Vec3F64 const& proto)
///         -> Expected<Eigen::Vector3d>;
///
///     template <>
///     auto toProt<Eigen::Vector3d>(Eigen::Vector3d const& cpp)
///         -> proto::Vec3F64;
///
#define FARM_PROTO_CONV_TRAIT(TCpp, TProto)                   \
  template <>                                                 \
  struct ToProtoTrait<TCpp> {                                 \
    using ProtoType = TProto;                                 \
  };                                                          \
  template <>                                                 \
  struct FromProtoTrait<TProto> {                             \
    using CppType = TCpp;                                     \
  };                                                          \
  template <>                                                 \
  auto fromProt<TProto>(TProto const& proto)->Expected<TCpp>; \
                                                              \
  template <>                                                 \
  auto toProt<TCpp>(TCpp const& cpp)->TProto

}  // namespace farm_ng
