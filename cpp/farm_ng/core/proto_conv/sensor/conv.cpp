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

namespace farm_ng::core {

Expected<sophus::CameraModel> fromProto(proto::CameraModel const& proto) {
  auto get_params = [&proto]() -> Eigen::VectorXd {
    Eigen::VectorXd params(proto.params_size());
    for (int i = 0; i < params.rows(); ++i) {
      params[i] = proto.params(i);
    }
    return params;
  };

  sophus::CameraDistortionType model = sophus::CameraDistortionType::pinhole;
  SOPHUS_ASSERT_OR_ERROR(
      trySetFromString(model, proto.distortion_type()),
      "distortion type not supported: {}",
      proto.distortion_type());

  return sophus::CameraModel(
      fromProto(proto.image_size()), model, get_params());
}

proto::CameraModel toProto(sophus::CameraModel const& camera_model) {
  proto::CameraModel proto;
  *proto.mutable_image_size() = toProto(camera_model.imageSize());
  proto.set_distortion_type(toString(camera_model.distortionType()));
  Eigen::VectorXd params = camera_model.params();
  for (int i = 0; i < params.rows(); ++i) {
    proto.add_params(params[i]);
  }
  return proto;
}

Expected<std::vector<sophus::CameraModel>> fromProto(
    proto::CameraModels const& proto) {
  std::vector<sophus::CameraModel> models;
  for (int i = 0; i < proto.camera_models_size(); ++i) {
    SOPHUS_TRY(sophus::CameraModel, cam, fromProto(proto.camera_models(i)));
    models.push_back(cam);
  }
  return models;
}

proto::CameraModels toProto(
    std::vector<sophus::CameraModel> const& camera_models) {
  proto::CameraModels proto;
  for (auto const& model : camera_models) {
    *proto.add_camera_models() = toProto(model);
  }
  return proto;
}

}  // namespace farm_ng::core
