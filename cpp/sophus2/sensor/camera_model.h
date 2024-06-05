// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sophus2/common/common.h"
#include "sophus2/common/enum.h"
#include "sophus2/geometry/point_transform.h"
#include "sophus2/image/image.h"
#include "sophus2/image/image_size.h"
#include "sophus2/lie/se3.h"
#include "sophus2/linalg/homogeneous.h"
#include "sophus2/sensor/camera_distortion/affine.h"
#include "sophus2/sensor/camera_distortion/brown_conrady.h"
#include "sophus2/sensor/camera_distortion/kannala_brandt.h"
#include "sophus2/sensor/camera_projection/projection_ortho.h"
#include "sophus2/sensor/camera_projection/projection_z1.h"

#include <Eigen/Dense>

#include <numeric>
#include <variant>

namespace sophus2 {

/// Subsamples pixel down, factor of 0.5.
///
/// See for details:
/// https://docs.google.com/document/d/1xmhCMWklP2UoQMGaMqFnsoPWoeMvBfXN7S8-ie9k0UA/edit#heading=h.97r8rr8owwpc
template <class TScalar>
auto subsampleDown(Eigen::Matrix<TScalar, 2, 1> const& in)
    -> Eigen::Matrix<TScalar, 2, 1> {
  return TScalar(0.5) * in;
}

/// Subsamples pixel up, factor of 2.0.
///
/// See for details:
/// https://docs.google.com/document/d/1xmhCMWklP2UoQMGaMqFnsoPWoeMvBfXN7S8-ie9k0UA/edit#heading=h.97r8rr8owwpc
template <class TScalar>
auto subsampleUp(Eigen::Matrix<TScalar, 2, 1> const& in)
    -> Eigen::Matrix<TScalar, 2, 1> {
  return TScalar(2.0) * in;
}

/// Bins pixel down, factor of 0.5.
///
/// See for details:
/// https://docs.google.com/document/d/1xmhCMWklP2UoQMGaMqFnsoPWoeMvBfXN7S8-ie9k0UA/edit#heading=h.elfm6123mecj
template <class TScalar>
auto binDown(Eigen::Matrix<TScalar, 2, 1> const& in)
    -> Eigen::Matrix<TScalar, 2, 1> {
  Eigen::Matrix<TScalar, 2, 1> out;
  // Map left image border from -0.5 to 0.0, then scale down, then
  // map the left image border from 0.0 back to -0.5
  out[0] = TScalar(0.5) * (in[0] + 0.5) - 0.5;  // cx
  out[1] = TScalar(0.5) * (in[1] + 0.5) - 0.5;  // cy
  return out;
}

/// Bins pixel up, factor of 2.0.
///
/// See for details:
/// https://docs.google.com/document/d/1xmhCMWklP2UoQMGaMqFnsoPWoeMvBfXN7S8-ie9k0UA/edit#heading=h.elfm6123mecj
template <class TScalar>
auto binUp(Eigen::Matrix<TScalar, 2, 1> const& in)
    -> Eigen::Matrix<TScalar, 2, 1> {
  Eigen::Matrix<TScalar, 2, 1> out;
  // Map left image border from -0.5 to 0.0, then scale up, then
  // map the left image border from 0.0 back to -0.5
  out[0] = TScalar(2.0) * (in[0] + 0.5) - 0.5;  // cx
  out[1] = TScalar(2.0) * (in[1] + 0.5) - 0.5;  // cy
  return out;
}

/// Camera model class template for pinhole-like camera projections.
template <class TBatch, class TDistortion, class TProj = ProjectionZ1>
class CameraModelT {
 public:
  /// The underlying scalar type.
  using Scalar = typename BatchTrait<TBatch>::ScalarBatch;
  static int constexpr kBatchSize = BatchTrait<TBatch>::kBatchSize;

  using Distortion = TDistortion;
  using Proj = TProj;
  static int constexpr kNumDistortionParams = Distortion::kNumDistortionParams;
  static int constexpr kNumParams = Distortion::kNumParams;
  static std::string_view constexpr const kProjectionModel =
      Distortion::kProjectionModel;

  using PointCamera = Eigen::Matrix<Scalar, 3, 1>;
  using PixelImage = Eigen::Matrix<Scalar, 2, 1>;
  using ProjInCameraLifted = Eigen::Matrix<Scalar, 2, 1>;
  using Params = Eigen::Matrix<Scalar, kNumParams, 1>;
  using DistorationParams = Eigen::Matrix<Scalar, kNumDistortionParams, 1>;

  /// Constructs camera model from image size and set up parameters.
  CameraModelT(ImageSize const& image_size, Params const& params)
      : image_size_(image_size), params_(params) {}

  CameraModelT() : image_size_({0, 0}) {
    params_.setZero();
    params_.template head<2>().setOnes();
  }

  /// Returns camera model from raw data pointer. To be used within ceres
  /// optimization only.
  static auto fromData(Scalar const* const ptr) -> CameraModelT {
    CameraModelT out;
    Eigen::Map<Eigen::Matrix<Scalar, kNumParams, 1> const> map(
        ptr, kNumParams, 1);
    out.params_ = map;
    return out;
  }

  /// Focal length.
  [[nodiscard]] auto focalLength() const -> PixelImage {
    return params_.template head<2>();
  }

  /// Focal length.
  void setFocalLength(PixelImage const& focal_length) {
    params_.template head<2>() = focal_length;
  }

  [[nodiscard]] auto principalPoint() const -> PixelImage {
    return params_.template segment<2>(2);
  }

  /// Focal length.
  void setPrincipalPoint(PixelImage const& principal_point) {
    params_.template segment<2>(2) = principal_point;
  }

  /// Returns distortion parameters by value.
  [[nodiscard]] auto distortionParams() const -> DistorationParams {
    return params_.template tail<kNumDistortionParams>();
  }

  /// Parameters mutator
  auto params() -> Eigen::Matrix<Scalar, kNumParams, 1>& { return params_; }

  /// Parameters accessor
  [[nodiscard]] auto params() const
      -> Eigen::Matrix<Scalar, kNumParams, 1> const& {
    return params_;
  }

  /// Subsamples pixel down, factor of 0.5.
  ///
  /// See for details:
  /// https://docs.google.com/document/d/1xmhCMWklP2UoQMGaMqFnsoPWoeMvBfXN7S8-ie9k0UA/edit#heading=h.97r8rr8owwpc
  ///
  /// If the original width [height] is odd, the new width [height] will be:
  /// (width+1)/2 [height+1)/2].
  [[nodiscard]] auto subsampleDown() const -> CameraModelT {
    Params params = this->params_;
    params[0] = Scalar(0.5) * params[0];  // fx
    params[1] = Scalar(0.5) * params[1];  // fy
    params.template segment<2>(2) = ::sophus2::subsampleDown(
        params.template segment<2>(2).eval());  // cx, cy
    return CameraModelT(half(image_size_), params);
  }

  /// Subsamples pixel up, factor of 2.0.
  ///
  /// See for details:
  /// https://docs.google.com/document/d/1xmhCMWklP2UoQMGaMqFnsoPWoeMvBfXN7S8-ie9k0UA/edit#heading=h.97r8rr8owwpc
  [[nodiscard]] auto subsampleUp() const -> CameraModelT {
    Params params = this->params_;
    params[0] = Scalar(2.0) * params[0];  // fx
    params[1] = Scalar(2.0) * params[1];  // fy
    params.template segment<2>(2) =
        ::sophus2::subsampleUp(params.template segment<2>(2).eval());  // cx, cy
    return CameraModelT(
        {image_size_.width * 2, image_size_.height * 2}, params);
  }

  /// Bins pixel down, factor of 0.5.
  ///
  /// See for details:
  /// https://docs.google.com/document/d/1xmhCMWklP2UoQMGaMqFnsoPWoeMvBfXN7S8-ie9k0UA/edit#heading=h.elfm6123mecj
  ///
  /// If the original width [height] is odd, the new width [height] will be:
  /// (width+1)/2 [height+1)/2].
  [[nodiscard]] auto binDown() const -> CameraModelT {
    Params params = this->params_;
    params[0] = Scalar(0.5) * params[0];  // fx
    params[1] = Scalar(0.5) * params[1];  // fy
    params.template segment<2>(2) =
        ::sophus2::binDown(params.template segment<2>(2).eval());  // cx, cy
    return CameraModelT(half(image_size_), params);
  }

  /// Bins pixel up, factor of 2.0.
  ///
  /// See for details:
  /// https://docs.google.com/document/d/1xmhCMWklP2UoQMGaMqFnsoPWoeMvBfXN7S8-ie9k0UA/edit#heading=h.elfm6123mecj
  [[nodiscard]] auto binUp() const -> CameraModelT {
    Params params = this->params_;
    params[0] = Scalar(2.0) * params[0];  // fx
    params[1] = Scalar(2.0) * params[1];  // fy
    params.template segment<2>(2) =
        ::sophus2::binUp(params.template segment<2>(2).eval());  // cx, cy
    return CameraModelT(
        {image_size_.width * 2, image_size_.height * 2}, params);
  }

  [[nodiscard]] auto scale(ImageSize const& image_size) const -> CameraModelT {
    Params params = this->params_;
    params[0] =
        Scalar(image_size.width) / Scalar(image_size_.width) * params[0];  // fx
    params[1] = Scalar(image_size.height) / Scalar(image_size_.height) *
                params[1];  // fy
    params[2] =
        Scalar(image_size.width) / Scalar(image_size_.width) * params[2];  // cx
    params[3] = Scalar(image_size.height) / Scalar(image_size_.height) *
                params[3];  // cy
    return CameraModelT({image_size.width, image_size.height}, params);
  }

  /// Region of interest given `top_left` and ``roi_size`.
  [[nodiscard]] auto roi(Eigen::Vector2i const& top_left, ImageSize roi_size)
      const -> CameraModelT {
    Params params = this->params_;
    params[2] = params[2] - top_left.x();  // cx
    params[3] = params[3] - top_left.y();  // cy
    return CameraModelT(roi_size, params);
  }

  /// Maps a 2-point in the z=1 plane of the camera to a pixel in the image.
  [[nodiscard]] auto distort(
      ProjInCameraLifted const& point2_in_camera_lifted) const -> PixelImage {
    return Distortion::template distort(params_, point2_in_camera_lifted);
  }

  [[nodiscard]] auto dxDistort(PixelImage const& pixel_in_image) const
      -> Eigen::Matrix<Scalar, 2, 2> {
    return Distortion::template dxDistort(params_, pixel_in_image);
  }

  /// Maps a pixel in the image to a 2-point in the z=1 plane of the camera.
  [[nodiscard]] auto undistort(PixelImage const& pixel_in_image) const
      -> ProjInCameraLifted {
    return Distortion::template undistort(params_, pixel_in_image);
  }

  [[nodiscard]] auto undistortTable() const -> MutImage<Eigen::Vector2f> {
    MutImage<Eigen::Vector2f> table(image_size_);
    for (int v = 0; v < table.height(); ++v) {
      Eigen::Vector2f* row_ptr = table.rowPtrMut(v);
      for (int u = 0; u < table.width(); ++u) {
        row_ptr[u] = this->undistort(PixelImage(u, v)).template cast<float>();
      }
    }
    return table;
  }

  /// Projects 3-point in camera frame to a pixel in the image.
  [[nodiscard]] auto camProj(PointCamera const& point_in_camera) const
      -> PixelImage {
    return Distortion::template distort(params_, Proj::proj(point_in_camera));
  }

  [[nodiscard]] auto dxCamProjX(PointCamera const& point_in_camera) const
      -> Eigen::Matrix<Scalar, 2, 3> {
    ProjInCameraLifted point_in_lifted = Proj::proj(point_in_camera);
    return dxDistort(point_in_lifted) * Proj::dxProjX(point_in_camera);
  }

  /// Unprojects pixel in the image to point in camera frame.
  ///
  /// The point is projected onto the xy-plane at z = `depth_z`.
  [[nodiscard]] auto camUnproj(
      PixelImage const& pixel_in_image, Scalar depth_z) const -> PointCamera {
    ProjInCameraLifted dist =
        Distortion::template undistort(params_, pixel_in_image);
    return Proj::unproj(dist, depth_z);
  }

  /// Raw data access. To be used in ceres optimization only.
  auto data() -> Scalar* { return params_.data(); }

  /// Accessor of image size.
  [[nodiscard]] auto imageSize() const -> ImageSize const& {
    return image_size_;
  }

  /// Returns true if obs is within image.
  ///
  /// Note: Positive border makes the image frame smaller.
  [[nodiscard]] auto contains(Eigen::Vector2i const& obs, int border = 0) const
      -> bool {
    return this->image_size_.contains(obs, border);
  }

  /// Returns true if obs is within image.
  ///
  /// Note: Positive border makes the image frame smaller.
  [[nodiscard]] auto contains(
      PixelImage const& obs, Scalar border = Scalar(0)) const -> bool {
    return this->image_size_.contains(obs, border);
  }

  /// cast to different scalar type.
  template <typename TScalar2>
  [[nodiscard]] auto cast() const
      -> CameraModelT<TScalar2, TDistortion, TProj> {
    return CameraModelT<TScalar2, TDistortion, TProj>(
        image_size_, params_.template cast<TScalar2>());
  }

 private:
  ImageSize image_size_;
  Eigen::Matrix<Scalar, kNumParams, 1> params_;
};

/// Camera model projection type.
SOPHUS_ENUM(
    CameraDistortionType,
    (pinhole, brown_conrady, kannala_brandt_k3, orthographic));

/// Pinhole camera model.
using PinholeModel = CameraModelT<double, AffineTransform, ProjectionZ1>;

/// Brown Conrady camera model.
using BrownConradyModel =
    CameraModelT<double, BrownConradyTransform, ProjectionZ1>;

/// KannalaBrandt camera model with k0, k1, k2, k3.
using KannalaBrandtK3Model =
    CameraModelT<double, KannalaBrandtK3Transform, ProjectionZ1>;

using OrthographicModel =
    CameraModelT<double, AffineTransform, ProjectionOrtho>;

/// Variant of camera models.
using CameraDistortionVariant = std::variant<
    PinholeModel,
    BrownConradyModel,
    KannalaBrandtK3Model,
    OrthographicModel>;

static_assert(
    std::variant_size_v<CameraDistortionVariant> ==
        getCount(CameraDistortionType()),
    "When the variant CameraDistortionVariant is updated, one needs to "
    "update the enum CameraDistortionType as well, and vice versa.");

/// Concrete camera model class.
class CameraModel {
 public:
  CameraModel()
      : model_(PinholeModel({0, 0}, Eigen::Vector4d(1.0, 1.0, 0.0, 0.0))) {}

  CameraModel(CameraModel const& model) = default;

  CameraModel& operator=(CameraModel const& model) = default;

  template <class TScalar, class TDistortion, class TProj>
  CameraModel(CameraModelT<TScalar, TDistortion, TProj> const& cam)
      : model_(cam) {}
  /// Constructs camera model from `CameraDistortionVariant`.
  CameraModel(CameraDistortionVariant const& variant) : model_(variant) {}

  /// Constructs camera model from `frame_name`, `image_size`, `projection_type`
  /// flag and `params` vector.
  ///
  /// Precondition: ``params.size()`` must match the number of parameters of the
  ///               specified `projection_type` (TransformModel::kNumParams).
  CameraModel(
      ImageSize image_size,
      CameraDistortionType projection_type,
      Eigen::VectorXd const& params);

  /// Creates default pinhole model from `image_size`.
  static auto createDefaultPinholeModel(ImageSize image_size) -> CameraModel;

  /// Returns true if this camera remains default-initialized with zero image
  /// dimensions.
  [[nodiscard]] auto isEmpty() const -> bool {
    return imageSize() == ImageSize(0, 0);
  }

  /// Returns name of the camera distortion model.
  [[nodiscard]] auto distortionModelName() const -> std::string_view;

  /// Distortion variant mutator.
  auto modelVariant() -> CameraDistortionVariant& { return model_; }

  /// Distortion variant accessor.
  [[nodiscard]] auto modelVariant() const -> CameraDistortionVariant const& {
    return model_;
  }

  /// Camera transform flag
  [[nodiscard]] auto distortionType() const -> CameraDistortionType;

  [[nodiscard]] auto focalLength() const -> Eigen::Vector2d;

  /// Focal length.
  void setFocalLength(Eigen::Vector2d const& focal_length);

  [[nodiscard]] auto principalPoint() const -> Eigen::Vector2d;

  /// Focal length.
  void setPrincipalPoint(Eigen::Vector2d const& principal_point);

  /// Returns `params` vector by value.
  [[nodiscard]] auto params() const -> Eigen::VectorXd;

  /// Sets `params` vector.
  /// Batch8PinholeModel batch_model;
  /// Precontion: ``params.size()`` must match the number of parameters of the
  ///             specified `projection_type` (TransformModel::kNumParams).
  void setParams(Eigen::VectorXd const& params);

  /// Returns distortion parameters vector by value.
  [[nodiscard]] auto distortionParams() const -> Eigen::VectorXd;

  /// Given a point in 3D space in the camera frame, compute the corresponding
  /// pixel coordinates in the image.
  [[nodiscard]] auto camProj(Eigen::Vector3d const& point_camera) const
      -> Eigen::Vector2d;

  /// Maps a 2-point in the z=1 plane of the camera to a (distorted) pixel in
  /// the image.
  [[nodiscard]] auto distort(
      Eigen::Vector2d const& point2_in_camera_lifted) const -> Eigen::Vector2d;

  [[nodiscard]] auto dxDistort(
      Eigen::Vector2d const& point2_in_camera_lifted) const -> Eigen::Matrix2d;

  /// Maps a pixel in the image to a 2-point in the z=1 plane of the camera.
  [[nodiscard]] auto undistort(Eigen::Vector2d const& pixel_image) const
      -> Eigen::Vector2d;

  [[nodiscard]] auto undistortTable() const -> MutImage<Eigen::Vector2f>;

  /// Derivative of camProj(x) with respect to x=0.
  [[nodiscard]] auto dxCamProjX(Eigen::Vector3d const& point_in_camera) const
      -> Eigen::Matrix<double, 2, 3>;

  /// Derivative of camProj(exp(x) * point) with respect to x=0.
  [[nodiscard]] auto dxCamProjExpXPointAt0(
      Eigen::Vector3d const& point_in_camera) const
      -> Eigen::Matrix<double, 2, 6>;

  /// Given pixel coordinates in the distorted image, and a corresponding
  /// depth, reproject to a 3d point in the camera's reference frame.
  [[nodiscard]] auto camUnproj(
      Eigen::Vector2d const& pixel_image, double depth_z) const
      -> Eigen::Vector3d;

  /// Subsamples pixel down, factor of 0.5.
  ///
  /// See for details:
  /// https://docs.google.com/document/d/1xmhCMWklP2UoQMGaMqFnsoPWoeMvBfXN7S8-ie9k0UA/edit#heading=h.97r8rr8owwpc
  ///
  /// If the original width [height] is oddBatch8PinholeModel batch_model;, the
  /// new width [height] will be: (width+1)/2 [height+1)/2].
  [[nodiscard]] auto subsampleDown() const -> CameraModel;

  /// Subsamples pixel up, factor of 2.0.
  [[nodiscard]] auto subsampleUp() const -> CameraModel;

  /// Bins pixel down, factor of 0.5.
  ///
  /// See for details:
  /// https://docs.google.com/document/d/1xmhCMWklP2UoQMGaMqFnsoPWoeMvBfXN7S8-ie9k0UA/edit#heading=h.elfm6123mecj
  ///
  /// If the original width [height] is odd, the new width [height] will be:
  /// (width+1)/2 [height+1)/2].
  [[nodiscard]] auto binDown() const -> CameraModel;

  /// Bins pixel up, factor of 2.0.
  ///
  /// See for details:
  /// https://docs.google.com/document/d/1xmhCMWklP2UoQMGaMqFnsoPWoeMvBfXN7S8-ie9k0UA/edit#heading=h.elfm6123mecj
  [[nodiscard]] auto binUp() const -> CameraModel;

  /// Image size accessor
  [[nodiscard]] auto imageSize() const -> ImageSize const&;

  /// Region of interest given `top_left` and ``roi_size`.
  [[nodiscard]] auto roi(
      Eigen::Vector2i const& top_left, ImageSize roi_size) const -> CameraModel;

  /// Returns true if obs is within image.
  ///
  /// Note: Positive border makes the image frame smaller.
  [[nodiscard]] auto contains(Eigen::Vector2i const& obs, int border = 0) const
      -> bool;

  /// Returns true if obs is within image.
  ///
  /// Positive border makes the image frame smaller.
  [[nodiscard]] auto contains(
      Eigen::Vector2d const& obs, double border = 0) const -> bool;

  [[nodiscard]] auto scale(ImageSize image_size) const -> CameraModel;

 private:
  CameraDistortionVariant model_;
};

/// Creates default pinhole model from `image_size`.
auto createDefaultPinholeModel(ImageSize image_size) -> PinholeModel;

/// Creates default orthographic model from `image_size`.
auto createDefaultOrthoModel(ImageSize image_size) -> OrthographicModel;

}  // namespace sophus2
