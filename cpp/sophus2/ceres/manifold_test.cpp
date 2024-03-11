// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "sophus2/ceres/manifold.h"

#include "sophus2/common/common.h"
#include "sophus2/lie/isometry2.h"
#include "sophus2/lie/isometry3.h"
#include "sophus2/lie/scaling.h"
#include "sophus2/lie/scaling_translation.h"
#include "sophus2/lie/similarity2.h"
#include "sophus2/lie/similarity3.h"
#include "sophus2/lie/translation.h"
#include "sophus2/linalg/vector_space.h"

#include <ceres/ceres.h>
#include <gtest/gtest.h>

#include <iostream>

namespace sophus2::test {

template <class LieGroup>
struct RotationalPart;

template <>
struct RotationalPart<sophus2::Rotation2F64> {
  static double norm(sophus2::Rotation2F64::Tangent const& t) {
    return t.template tail<1>().norm();
  }
};

template <>
struct RotationalPart<sophus2::Rotation3F64> {
  static double norm(sophus2::Rotation3F64::Tangent const& t) {
    return t.template tail<3>().norm();
  }
};

template <>
struct RotationalPart<sophus2::Isometry2F64> {
  static double norm(sophus2::Isometry2F64::Tangent const& t) {
    return t.template tail<1>().norm();
  }
};

template <>
struct RotationalPart<sophus2::Isometry3F64> {
  static double norm(sophus2::Isometry3F64::Tangent const& t) {
    return t.template tail<3>().norm();
  }
};

template <>
struct RotationalPart<sophus2::SpiralSimilarity2F64> {
  static double norm(sophus2::SpiralSimilarity2F64::Tangent const& t) {
    return t.template segment<1>(0).norm();
  }
};

template <>
struct RotationalPart<sophus2::SpiralSimilarity3F64> {
  static double norm(sophus2::SpiralSimilarity3F64::Tangent const& t) {
    return t.template segment<3>(0).norm();
  }
};

template <>
struct RotationalPart<sophus2::Similarity2F64> {
  static double norm(sophus2::Similarity2F64::Tangent const& t) {
    return t.template segment<1>(2).norm();
  }
};

template <>
struct RotationalPart<sophus2::Similarity3F64> {
  static double norm(sophus2::Similarity3F64::Tangent const& t) {
    return t.template segment<3>(3).norm();
  }
};

template <>
struct RotationalPart<sophus2::Scaling<double, 2>> {
  static double norm(typename sophus2::Scaling<double, 2>::Tangent const& t) {
    return 0.0;
  }
};

template <>
struct RotationalPart<sophus2::Scaling<double, 3>> {
  static double norm(typename sophus2::Scaling<double, 3>::Tangent const& t) {
    return 0.0;
  }
};

template <>
struct RotationalPart<sophus2::ScalingTranslation<double, 2>> {
  static double norm(
      typename sophus2::ScalingTranslation<double, 2>::Tangent const& t) {
    return 0.0;
  }
};

template <>
struct RotationalPart<sophus2::ScalingTranslation<double, 3>> {
  static double norm(
      typename sophus2::ScalingTranslation<double, 3>::Tangent const& t) {
    return 0.0;
  }
};

template <>
struct RotationalPart<sophus2::Translation<double, 2>> {
  static double norm(
      typename sophus2::Translation<double, 2>::Tangent const& t) {
    return 0.0;
  }
};

template <>
struct RotationalPart<sophus2::Translation<double, 3>> {
  static double norm(
      typename sophus2::Translation<double, 3>::Tangent const& t) {
    return 0.0;
  }
};

template <template <class> class TLieGroup>
struct CeresManifoldTests {
  template <class TScalar>
  using LieGroup = TLieGroup<TScalar>;
  using LieGroupF64 = LieGroup<double>;

  static int constexpr kDof = LieGroupF64::kDof;
  static int constexpr kNumParams = LieGroupF64::kNumParams;
  static int constexpr kPointDim = LieGroupF64::kPointDim;

  using PointF64 = Eigen::Vector<double, kPointDim>;
  using TangentF64 = Eigen::Vector<double, kDof>;

  CeresManifoldTests(
      std::vector<LieGroupF64> const& group_vec,
      std::vector<PointF64> const& point_vec,
      std::string group_name)
      : group_vec(group_vec), point_vec(point_vec), group_name(group_name) {}

  void testAll() {
    for (size_t i = 0; i < group_vec.size(); ++i) {
      for (size_t j = 0; j < group_vec.size(); ++j) {
        testManifold(group_vec[i], group_vec[j]);
      }
    }
  }

  bool testManifold(LieGroupF64 const& x, LieGroupF64 const& y) {
    // ceres/manifold_test_utils.h is google-test based; here we check all the
    // same invariants
    const TangentF64 delta = (x.inverse() * y).log();
    TangentF64 o;
    o.setZero();
    ::sophus2::ceres::Manifold<TLieGroup> manifold;

    LieGroupF64 test_group;

    bool passed = true;
    auto coeffs =
        Eigen::Map<const Eigen::Matrix<double, kNumParams, 1>>(x.ptr());
    auto coeffs_y =
        Eigen::Map<const Eigen::Matrix<double, kNumParams, 1>>(y.ptr());
    std::cerr << "XPlusZeroIsXAt " << coeffs.transpose() << std::endl;
    xPlusZeroIsXAt(x);
    std::cerr << "XMinusXIsZeroAt " << coeffs.transpose() << std::endl;
    xMinusXIsZeroAt(x);
    std::cerr << "MinusPlusIsIdentityAt " << coeffs.transpose() << std::endl;
    minusPlusIsIdentityAt(x, delta);
    std::cerr << "MinusPlusIsIdentityAt " << coeffs.transpose() << std::endl;
    minusPlusIsIdentityAt(x, o);
    std::cerr << "PlusMinusIsIdentityAt " << coeffs.transpose() << std::endl;
    plusMinusIsIdentityAt(x, x);
    std::cerr << "PlusMinusIsIdentityAt " << coeffs.transpose() << " "
              << coeffs_y.transpose() << std::endl;
    plusMinusIsIdentityAt(x, y);
    std::cerr << "MinusPlusJacobianIsIdentityAt " << coeffs.transpose()
              << std::endl;
    minusPlusJacobianIsIdentityAt(x);
    return passed;
  }

  void xPlusZeroIsXAt(LieGroupF64 const& x) {
    TangentF64 o;
    o.setZero();
    sophus2::ceres::Manifold<TLieGroup> manifold;
    LieGroupF64 test_group;

    SOPHUS_ASSERT(manifold.Plus(x.ptr(), o.data(), test_group.unsafeMutPtr()));
    double const error = ((x.inverse() * test_group).log()).squaredNorm();
    SOPHUS_ASSERT_LE(
        error,
        sophus2::kEpsilonF64,
        "{}\nx:\n {}\ntg:\n {}",
        group_name,
        x.matrix(),
        test_group.matrix());
  }

  void xMinusXIsZeroAt(LieGroupF64 const& x) {
    sophus2::ceres::Manifold<TLieGroup> manifold;
    LieGroupF64 test_group;
    TangentF64 test_tangent;

    SOPHUS_ASSERT(manifold.Minus(x.ptr(), x.ptr(), test_tangent.data()));
    double const error = test_tangent.squaredNorm();
    SOPHUS_ASSERT(error < sophus2::kEpsilonF64);
  }

  void minusPlusIsIdentityAt(LieGroupF64 const& x, TangentF64 const& delta) {
    if (RotationalPart<LieGroupF64>::norm(delta) >
        sophus2::kPi<double> * (1. - sophus2::kEpsilonF64)) {
      return;
    }
    sophus2::ceres::Manifold<TLieGroup> manifold;
    LieGroupF64 test_group;
    TangentF64 test_tangent;

    bool passed = true;

    SOPHUS_ASSERT(
        manifold.Plus(x.ptr(), delta.data(), test_group.unsafeMutPtr()));

    SOPHUS_ASSERT(
        manifold.Minus(test_group.ptr(), x.ptr(), test_tangent.data()));

    const TangentF64 diff = test_tangent - delta;
    double const error = diff.squaredNorm();
    SOPHUS_ASSERT_LE(error, 10 * sophus2::kEpsilonF64, "{}", group_name);
  }

  void plusMinusIsIdentityAt(LieGroupF64 const& x, LieGroupF64 const& y) {
    sophus2::ceres::Manifold<TLieGroup> manifold;
    LieGroupF64 test_group;
    TangentF64 test_tangent;

    SOPHUS_ASSERT(manifold.Minus(y.ptr(), x.ptr(), test_tangent.data()));

    SOPHUS_ASSERT(
        manifold.Plus(x.ptr(), test_tangent.data(), test_group.unsafeMutPtr()));

    double const error = ((y.inverse() * test_group).log()).squaredNorm();
    SOPHUS_ASSERT_LE(error, 10.0 * sophus2::kEpsilonF64);
  }

  void minusPlusJacobianIsIdentityAt(LieGroupF64 const& x) {
    sophus2::ceres::Manifold<TLieGroup> manifold;
    LieGroupF64 test_group;

    Eigen::Matrix<
        double,
        kNumParams,
        kDof,
        kDof == 1 ? Eigen::ColMajor : Eigen::RowMajor>
        jplus;
    Eigen::Matrix<double, kDof, kNumParams, Eigen::RowMajor> jminus;

    SOPHUS_ASSERT(manifold.PlusJacobian(x.ptr(), jplus.data()));

    SOPHUS_ASSERT(manifold.MinusJacobian(x.ptr(), jminus.data()));

    const Eigen::Matrix<double, kDof, kDof> diff =
        jminus * jplus - Eigen::Matrix<double, kDof, kDof>::Identity();

    std::cerr << diff << std::endl;
    double const error = diff.squaredNorm();
    SOPHUS_ASSERT(error < sophus2::kEpsilonF64);
  }

  std::vector<LieGroupF64> group_vec;
  std::vector<PointF64> point_vec;

  std::string group_name;
};

TEST(ceres_manifold, prop_test2) {
  auto points2 = sophus2::pointExamples<double, 2>();

  CeresManifoldTests<sophus2::Rotation2>(
      sophus2::Rotation2F64::elementExamples(), points2, "Rotation2")
      .testAll();

  CeresManifoldTests<sophus2::Isometry2>(
      sophus2::Isometry2F64::elementExamples(), points2, "Isometry2")
      .testAll();

  CeresManifoldTests<sophus2::SpiralSimilarity2>(
      sophus2::SpiralSimilarity2F64::elementExamples(),
      points2,
      "SpiralSimilarity2")
      .testAll();

  CeresManifoldTests<sophus2::Similarity2>(
      sophus2::Similarity2F64::elementExamples(), points2, "Similarity2")
      .testAll();

  CeresManifoldTests<sophus2::Scaling2>(
      sophus2::Scaling2<double>::elementExamples(), points2, "Scaling2")
      .testAll();

  CeresManifoldTests<sophus2::Translation2>(
      sophus2::Translation2<double>::elementExamples(), points2, "Translation2")
      .testAll();

  CeresManifoldTests<sophus2::ScalingTranslation2>(
      sophus2::ScalingTranslation2<double>::elementExamples(),
      points2,
      "ScalingTranslation2")
      .testAll();
}

TEST(ceres_manifold, prop_test3) {
  auto points3 = sophus2::pointExamples<double, 3>();

  CeresManifoldTests<sophus2::Rotation3>(
      sophus2::Rotation3F64::elementExamples(), points3, "Rotation3")
      .testAll();

  CeresManifoldTests<sophus2::Isometry3>(
      sophus2::Isometry3F64::elementExamples(), points3, "Isometry3")
      .testAll();

  CeresManifoldTests<sophus2::SpiralSimilarity3>(
      sophus2::SpiralSimilarity3F64::elementExamples(),
      points3,
      "SpiralSimilarity3")
      .testAll();

  CeresManifoldTests<sophus2::Similarity3>(
      sophus2::Similarity3F64::elementExamples(), points3, "Similarity3")
      .testAll();

  CeresManifoldTests<sophus2::Scaling3>(
      sophus2::Scaling3<double>::elementExamples(), points3, "Scaling3")
      .testAll();

  CeresManifoldTests<sophus2::Translation3>(
      sophus2::Translation3<double>::elementExamples(), points3, "Translation3")
      .testAll();

  CeresManifoldTests<sophus2::ScalingTranslation3>(
      sophus2::ScalingTranslation3<double>::elementExamples(),
      points3,
      "ScalingTranslation3")
      .testAll();
}

}  // namespace sophus2::test
