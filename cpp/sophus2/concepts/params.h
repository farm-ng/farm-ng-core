// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sophus2/common/common.h"
#include "sophus2/concepts/point.h"
#include "sophus2/concepts/utils.h"

namespace sophus2 {
namespace concepts {

template <class TT>
concept ParamsImpl = std::is_same_v<
    typename TT::Params,
    Eigen::Vector<typename TT::Scalar, TT::kNumParams>> &&
    requires(typename TT::Params params) {
  // constructors and factories
  { TT::areParamsValid(params) } -> ConvertibleTo<sophus2::Expected<Success>>;

  { TT::paramsExamples() } -> ConvertibleTo<std::vector<typename TT::Params>>;

  {
    TT::invalidParamsExamples()
    } -> ConvertibleTo<std::vector<typename TT::Params>>;
};

template <class TT>
concept Tangent = std::is_same_v<
    typename TT::Tangent,
    Eigen::Vector<typename TT::Scalar, TT::kDof>> && requires() {
  { TT::tangentExamples() } -> ConvertibleTo<std::vector<typename TT::Tangent>>;
};

template <class TT>
concept Params = std::is_same_v<
    typename TT::Params,
    Eigen::Vector<typename TT::Scalar, TT::kNumParams>> &&
    requires(TT m, typename TT::Params params) {
  // constructors and factories
  { TT::fromParams(params) } -> ConvertibleTo<TT>;

  {m.setParams(params)};

  { m.params() } -> ConvertibleTo<typename TT::Params>;

  { m.ptr() } -> ConvertibleTo<typename TT::Scalar const *>;

  { m.unsafeMutPtr() } -> ConvertibleTo<typename TT::Scalar *>;
};

// Example scalar type to be used when specifying concepts interfaces.
//
// The main motivation are compatible scalar types such as
// ceres::Jet<double,...> which mix well with the scalar ``double``.
// However, we do not want include ceres::Jet<double,...> here.
template <class TT>
struct CompatScalarEx {
  CompatScalarEx(TT const &value) : value(value) {}

  TT value;
};

}  // namespace concepts
}  // namespace sophus2

namespace Eigen {

// This is mirrored from ceres::Jet.
template <class TT>
struct NumTraits<sophus2::concepts::CompatScalarEx<TT>> {
  using Real = sophus2::concepts::CompatScalarEx<TT>;
  using NonInteger = sophus2::concepts::CompatScalarEx<TT>;
  using Nested = sophus2::concepts::CompatScalarEx<TT>;
  using Literal = sophus2::concepts::CompatScalarEx<TT>;

  static bool constexpr IsComplex = false;
  static bool constexpr IsInteger = false;

  static typename sophus2::concepts::CompatScalarEx<TT> dummy_precision() {
    return sophus2::concepts::CompatScalarEx<TT>(1e-12);
  }

  inline static Real epsilon() {
    return Real(std::numeric_limits<TT>::epsilon());
  }

  inline static int digits10() { return NumTraits<TT>::digits10(); }

  inline static Real highest() {
    return Real((std::numeric_limits<TT>::max)());
  }
  inline static Real lowest() {
    return Real(-(std::numeric_limits<TT>::max)());
  }
};

template <class BinaryOp, class TT>
struct ScalarBinaryOpTraits<
    sophus2::concepts::CompatScalarEx<TT>,
    TT,
    BinaryOp> {
  using ReturnType = sophus2::concepts::CompatScalarEx<TT>;
};
template <class BinaryOp, class TT>
struct ScalarBinaryOpTraits<
    TT,
    sophus2::concepts::CompatScalarEx<TT>,
    BinaryOp> {
  using ReturnType = sophus2::concepts::CompatScalarEx<TT>;
};

}  // namespace Eigen
