// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sophus/lie/isometry3.h"

namespace sophus {

template <class TScalar>
class Pose3 {
 public:
  using Scalar = TScalar;
  using Isometry = Isometry3<Scalar>;
  using Tangent = typename Isometry::Tangent;
  using Rotation = typename Isometry::Rotation;
  using Params = typename Isometry::Params;
  Pose3(
      Isometry const& a_from_b,
      std::string const& frame_a,
      std::string const& frame_b,
      Tangent const& tangent_of_b_in_a = Tangent::Zero())
      : a_from_b_(a_from_b),
        frame_a_(frame_a),
        frame_b_(frame_b),
        tangent_of_b_in_a_(tangent_of_b_in_a) {}

  std::string const& frameA() const { return frame_a_; }
  std::string& frameA() { return frame_a_; }

  std::string const& frameB() const { return frame_b_; }
  std::string& frameB() { return frame_b_; }

  Isometry const& aFromB() const { return a_from_b_; }
  Isometry& aFromB() { return a_from_b_; }

  Tangent const& tangentOfBInA() const { return tangent_of_b_in_a_; }
  Tangent& tangentOfBInA() { return tangent_of_b_in_a_; }

  Tangent log() const { return a_from_b_.log(); }

  static Pose3 exp(
      Tangent const& tangent_of_b_in_a,
      double dt,
      std::string const& frame_a,
      std::string const& frame_b) {
    return Pose3(
        Isometry::exp(tangent_of_b_in_a * dt),
        frame_a,
        frame_b,
        tangent_of_b_in_a);
  }

  Eigen::Vector<Scalar, 3> translation() const {
    return aFromB().translation();
  }

  Eigen::VectorBlock<Params, 3> translation() {
    return a_from_b_.translation();
  }

  Rotation rotation() const { return aFromB().rotation(); }
  void setRotation(Rotation const& rotation) {
    return aFromB().setRotation(rotation);
  }

  Pose3 inverse() const {
    return Pose3(a_from_b_.inverse(), frame_b_, frame_a_, -tangent_of_b_in_a_);
  }

  friend Expected<Tangent> error(
      Pose3 const& lhs_a_from_b, Pose3 const& rhs_a_from_b) {
    return (lhs_a_from_b.inverse() * rhs_a_from_b)
        .and_then([](Expected<Pose3> const& pose) -> Expected<Tangent> {
          return pose->log();
        });
  }

  friend Expected<Pose3> operator*(Pose3 const& lhs, Pose3 const& rhs) {
    if (lhs.frameB() != rhs.frameA()) {
      return FARM_UNEXPECTED(
          "Pose frame error: lhs a={} b={} rhs a={} b={}",
          lhs.frameA(),
          lhs.frameB(),
          rhs.frameA(),
          rhs.frameB());
    }

    Tangent tangent_of_rhs_b_in_lhs_a;

    tangent_of_rhs_b_in_lhs_a.template head<3>() =
        lhs.tangentOfBInA().template head<3>() +
        lhs.aFromB().rotation() * rhs.tangentOfBInA().template head<3>();

    tangent_of_rhs_b_in_lhs_a.template tail<3>() =
        lhs.tangentOfBInA().template tail<3>() +
        lhs.aFromB().rotation() * rhs.tangentOfBInA().template tail<3>();

    return Pose3(
        lhs.aFromB() * rhs.aFromB(),
        lhs.frameA(),
        rhs.frameB(),
        tangent_of_rhs_b_in_lhs_a);
  }

  friend Expected<Pose3> operator*(
      Pose3 const& lhs, Expected<Pose3> const& rhs) {
    if (!rhs) {
      return rhs;
    }
    return lhs * (*rhs);
  }

  friend Expected<Pose3> operator*(
      Expected<Pose3> const& lhs, Expected<Pose3> const& rhs) {
    if (!lhs) {
      return lhs;
    }
    if (!rhs) {
      return rhs;
    }
    return (*lhs) * (*rhs);
  }

  friend Expected<Pose3> operator*(
      Expected<Pose3> const& lhs, Pose3 const& rhs) {
    if (!lhs) {
      return lhs;
    }
    return (*lhs) * rhs;
  }

 private:
  Isometry a_from_b_;
  std::string frame_a_;
  std::string frame_b_;
  Tangent tangent_of_b_in_a_;
};

using Pose3F32 = Pose3<float>;
using Pose3F64 = Pose3<double>;

}  // namespace sophus
