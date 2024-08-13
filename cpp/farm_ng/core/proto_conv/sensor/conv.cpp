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

#include "farm_ng/core/proto_conv/sensor/conv.h"

#include "farm_ng/core/proto_conv/image/conv.h"
#include "farm_ng/core/proto_conv/lie/conv.h"

namespace farm_ng {

template <>
auto fromProt<core::proto::ClippingPlanes>(
    core::proto::ClippingPlanes const& proto)
    -> Expected<sophus2::ClippingPlanes> {
  return sophus2::ClippingPlanes{.near = proto.near(), .far = proto.far()};
}

template <>
auto toProt<sophus2::ClippingPlanes>(sophus2::ClippingPlanes const& planes)
    -> core::proto::ClippingPlanes {
  core::proto::ClippingPlanes proto;
  proto.set_near(planes.near);
  proto.set_far(planes.far);
  return proto;
}

template <>
auto fromProt<core::proto::CameraModel>(core::proto::CameraModel const& proto)
    -> Expected<sophus2::CameraModel> {
  auto get_params = [&proto]() -> Eigen::VectorXd {
    Eigen::VectorXd params(proto.params_size());
    for (int i = 0; i < params.rows(); ++i) {
      params[i] = proto.params(i);
    }
    return params;
  };
  sophus2::CameraDistortionType model = sophus2::CameraDistortionType::pinhole;
  FARM_TRY_ASSERT(
      trySetFromString(model, proto.distortion_type()),
      "distortion type not supported: {}",
      proto.distortion_type());
  FARM_TRY(auto, size, fromProt(proto.image_size()));

  return sophus2::CameraModel(size, model, get_params());
}

template <>
auto toProt<sophus2::CameraModel>(sophus2::CameraModel const& camera_model)
    -> core::proto::CameraModel {
  core::proto::CameraModel proto;
  *proto.mutable_image_size() = toProt(camera_model.imageSize());
  proto.set_distortion_type(toString(camera_model.distortionType()));
  Eigen::VectorXd params = camera_model.params();
  for (int i = 0; i < params.rows(); ++i) {
    proto.add_params(params[i]);
  }
  return proto;
}

template <>
auto fromProt<core::proto::CameraModels>(core::proto::CameraModels const& proto)
    -> Expected<std::vector<sophus2::CameraModel>> {
  std::vector<sophus2::CameraModel> models;
  for (int i = 0; i < proto.camera_models_size(); ++i) {
    SOPHUS_TRY(sophus2::CameraModel, cam, fromProt(proto.camera_models(i)));
    models.push_back(cam);
  }
  return models;
}

template <>
auto toProt<std::vector<sophus2::CameraModel>>(
    std::vector<sophus2::CameraModel> const& camera_models)
    -> core::proto::CameraModels {
  core::proto::CameraModels proto;
  for (auto const& model : camera_models) {
    *proto.add_camera_models() = toProt(model);
  }
  return proto;
}

template <>
auto fromProt<core::proto::RigidCamera>(core::proto::RigidCamera const& proto)
    -> Expected<sophus2::RigidCamera> {
  sophus2::RigidCamera s;
  SOPHUS_TRY(auto, intrinsics, fromProt(proto.intrinsics()));
  SOPHUS_TRY(auto, extrinsics, fromProt(proto.rig_from_camera()));
  s.intrinsics = intrinsics;
  s.rig_from_camera = extrinsics;
  return s;
}

template <>
auto toProt<sophus2::RigidCamera>(sophus2::RigidCamera const& s)
    -> core::proto::RigidCamera {
  core::proto::RigidCamera proto;
  *proto.mutable_intrinsics() = toProt(s.intrinsics);
  *proto.mutable_rig_from_camera() = toProt(s.rig_from_camera);
  return proto;
}

template <>
auto fromProt<core::proto::MultiCameraRig>(
    core::proto::MultiCameraRig const& proto)
    -> Expected<sophus2::MultiCameraRig> {
  sophus2::MultiCameraRig cameras;
  for (int i = 0; i < proto.cameras_size(); ++i) {
    FARM_TRY(auto, cam, fromProt(proto.cameras(i)));
    cameras.push_back(cam);
  }
  return cameras;
}

template <>
auto toProt<sophus2::MultiCameraRig>(sophus2::MultiCameraRig const& cameras)
    -> core::proto::MultiCameraRig {
  core::proto::MultiCameraRig proto;
  for (auto const& cam : cameras) {
    *proto.add_cameras() = toProt(cam);
  }
  return proto;
}

}  // namespace farm_ng
