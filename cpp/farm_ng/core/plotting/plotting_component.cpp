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

#include "farm_ng/core/plotting/plotting_component.h"

#include "farm_ng/core/plotting/plotting.grpc.pb.h"
#include "farm_ng/core/proto_conv/linalg/conv.h"
#include "farm_ng/core/proto_conv/plotting/conv.h"

#include <grpcpp/create_channel.h>

namespace farm_ng {

struct PlottingComponentImpl : public Component, public PlottingComponent {
  PlottingComponentImpl(
      Context const& ctx, RemotePlottingClient::Params const& params)
      : Component(ctx, "plotting", this, ""),
        input_messages_(
            this,
            "on_messages",
            std::bind(
                &PlottingComponentImpl::onMessages,
                this,
                std::placeholders::_1)),

        params_(params)

  {}

  void connect() override {
    remote_client_ = RemotePlottingClient::createAndConnect(params_);
  }

  void onMessages(std::vector<plotting::Message> const& messages) {
    if (!remote_client_) {
      FARM_INFO_EVERY_N(
          1000,
          "Not connected to plotting server, so dropping messages. Call "
          "this->connect() to connect.");
      return;
    }
    FARM_UNWRAP(remote_client_).onMessages(messages);
  }

  Input<std::vector<plotting::Message>>& inMessages() override {
    return input_messages_;
  }

  std::shared_ptr<RemotePlottingClient> remote_client_;
  Input<std::vector<plotting::Message>> input_messages_;
  RemotePlottingClient::Params params_;
};

Shared<PlottingComponent> PlottingComponent::create(
    Context const& ctx, RemotePlottingClient::Params const& params) {
  return std::make_shared<PlottingComponentImpl>(ctx, params);
}

}  // namespace farm_ng
