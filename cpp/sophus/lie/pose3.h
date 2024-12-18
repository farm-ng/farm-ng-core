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
      Tangent const& tangent_in_b = Tangent::Zero())
      : a_from_b_(a_from_b),
        frame_a_(frame_a),
        frame_b_(frame_b),
        tangent_in_b_(tangent_in_b) {}

  std::string const& frameA() const { return frame_a_; }
  std::string& frameA() { return frame_a_; }

  std::string const& frameB() const { return frame_b_; }
  std::string& frameB() { return frame_b_; }

  Isometry const& aFromB() const { return a_from_b_; }
  Isometry& aFromB() { return a_from_b_; }

  /// Rate of change of the pose ``a_from_b`` represented in frame ``b``.
  ///
  /// In other words tangent_in_b_ is the relative linear and angular velocity
  /// of the rigid body ``b`` (with respect to frame ``a``) and this
  /// velocity is expressed in frame ``b``.
  Tangent const& tangentInB() const { return tangent_in_b_; }

  /// Mutable version of tangentInB.
  Tangent& tangentInB() { return tangent_in_b_; }

  // deprecated naming convention
  Tangent const& tangentOfBInA() const { return tangentInB(); }

  // deprecated naming convention
  Tangent& tangentOfBInA() { return tangentInB(); }

  /// Pose of entity ``b`` in the ``a`` frame.
  Isometry bFromA() const { return a_from_b_.inverse(); }

  /// Changes origin frame of tangent vector from frame ``bar`` to frame
  /// ``foo``.
  static Tangent changeTangentOrigin(
      sophus::Isometry3F64 const& foo_from_bar, Tangent const& tangent_in_bar) {
    return foo_from_bar.adj() * tangent_in_bar;
  }

  Tangent log() const { return a_from_b_.log(); }

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

  /// Inverse of the pose (and its velocity).
  ///
  Pose3 inverse() const {
    return Pose3(
        bFromA(),
        frame_b_,
        frame_a_,
        -changeTangentOrigin(aFromB(), tangent_in_b_));
  }

  /// Evolves the pose by a small increment ``dt``.
  ///
  /// It is assumed that the egocentric velocity of the rigid body ``b`` stays
  /// constant.
  Pose3 evolve(double dt) const {
    return Pose3(
        aFromB() * Isometry::exp(tangent_in_b_ * dt),
        frame_a_,
        frame_b_,
        tangent_in_b_);
  }

  static Expected<Tangent> error(
      Pose3 const& lhs_a_from_b, Pose3 const& rhs_a_from_b) {
    FARM_TRY(Pose3, product, lhs_a_from_b.inverse() * rhs_a_from_b);

    FARM_INFO(
        "lhs:\n {}\n rhs: {}\n prod: {}",
        lhs_a_from_b.a_from_b_.matrix(),
        rhs_a_from_b.a_from_b_.matrix(),
        product.a_from_b_.matrix());
    Tangent log =  product.log();

    FARM_INFO("log: {}", log);
    return log;
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

    /// for notation simplicity, let's introduce the following three frames:
    ///
    /// a  := lhs.frameA()
    /// mid  := lhs.frameB() == rhs.frameA()
    /// b  := rhs.frameB()

    Isometry a_from_mid = lhs.aFromB();
    Isometry mid_from_b = rhs.aFromB();
    // Relative velocity of the rigid body ``mid`` (with respect to frame
    // a) in frame ``mid``.
    Tangent velocity_of_mid_wrt_a_in_mid = lhs.tangentInB();
    // Relative velocity of the rigid body ``b`` (with respect to frame
    // mid) in frame ``b``.
    Tangent velocity_of_b_wrt_mid_in_b = rhs.tangentInB();

    /// We aim to calculate the pose of ``a_from_b`` and its velocity
    /// ``velocity_of_b_wrt_a_in_b``.
    Isometry a_from_b = a_from_mid * mid_from_b;

    // Given relative velocity of the rigid body ``mid`` (with respect to frame
    // a) in frame ``mid``,
    // we calculate the relative velocity of the rigid body ``mid`` (with
    // respect to frame a) in frame ``b``.
    Eigen::Vector<double, 6> velocity_of_mid_wrt_a_in_b =
        changeTangentOrigin(mid_from_b.inverse(), velocity_of_mid_wrt_a_in_mid);

    // This is basically just vec(a,mid) + vec(mid,b) = vec(a,b). This is valid
    // because all velocities are expressed in the same frame b.
    Tangent velocity_of_b_wrt_a_in_b =
        velocity_of_mid_wrt_a_in_b + velocity_of_b_wrt_mid_in_b;

    return Pose3(
        a_from_b, lhs.frameA(), rhs.frameB(), velocity_of_b_wrt_a_in_b);
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
  Tangent tangent_in_b_;
};

using Pose3F32 = Pose3<float>;
using Pose3F64 = Pose3<double>;

}  // namespace sophus
