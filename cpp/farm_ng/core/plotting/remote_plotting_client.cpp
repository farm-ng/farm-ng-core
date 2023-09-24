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

#include "farm_ng/core/plotting/remote_plotting_client.h"

#include "farm_ng/core/plotting/plotting.grpc.pb.h"
#include "farm_ng/core/proto_conv/linalg/conv.h"
#include "farm_ng/core/proto_conv/plotting/conv.h"

#include <grpcpp/create_channel.h>

namespace farm_ng {

namespace {
// seconds from any std::chrono duration
//
// TODO: move to a common place such as farm_ng/core/misc/time.h
template <class Rep, class Period>
double secondsFromDuration(std::chrono::duration<Rep, Period> const& duration) {
  return 1e-6 * std::chrono::duration_cast<std::chrono::microseconds>(duration)
                    .count();
}
}  // namespace

class PlottingRemoteClientImpl : public RemotePlottingClient {
 public:
  PlottingRemoteClientImpl(RemotePlottingClient::Params const& params)
      : params_(params),
        channel_(grpc::CreateChannel(
            FARM_FORMAT("{}:{}", params.host, params.port),
            grpc::InsecureChannelCredentials())),
        stub_(core::plotting::proto::PlottingWidget::NewStub(channel_)) {}

  ~PlottingRemoteClientImpl() { flush(); }

  void onMessages(std::vector<plotting::Message> const& messages) {
    messages_.insert(messages_.end(), messages.begin(), messages.end());

    auto now = std::chrono::steady_clock::now();

    // rate limit grpc calls to `rate_limit_hz` per second
    std::chrono::steady_clock::duration elapsed =
        last_message_time_ ? now - *last_message_time_
                           : std::chrono::steady_clock::duration::max();
    double seconds_elapsed = secondsFromDuration(elapsed);
    if (seconds_elapsed > 1.0 / params_.rate_limit_hz) {
      flush();
      last_message_time_ = now;
    } else {
    }
  }

  void flush() {
    send(messages_);
    messages_.clear();
  }

 private:
  void send(std::vector<plotting::Message> const& messages) {
    grpc::ClientContext context;
    core::plotting::proto::PlottingReply response;
    core::plotting::proto::Messages proto = toProt(messages);
    grpc::Status status = stub_->send(&context, proto, &response);
    if (!status.ok()) {
      FARM_INFO_EVERY_N(
          1000,
          "Error sending point: [{}] {}",
          status.error_code(),
          status.error_message());
    }
  }
  RemotePlottingClient::Params params_;
  std::shared_ptr<grpc::ChannelInterface> channel_;
  std::unique_ptr<core::plotting::proto::PlottingWidget::Stub> stub_;
  std::shared_ptr<RemotePlottingClient> remote_client_;
  std::optional<std::chrono::steady_clock::time_point> last_message_time_;
  std::vector<plotting::Message> messages_;
};

Shared<RemotePlottingClient> RemotePlottingClient::createAndConnect(
    RemotePlottingClient::Params const& params) {
  return std::make_shared<PlottingRemoteClientImpl>(params);
}

}  // namespace farm_ng
