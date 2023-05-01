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

#include <farm_ng/core/event_service.grpc.pb.h>
#include <farm_ng/core/prototools/event_log_writer.h>
#include <farm_ng/core/prototools/event_service.h>
#include <grpcpp/grpcpp.h>
namespace farm_ng {

EventServiceClient::EventServiceClient(
    Context const& ctx, std::string const& path)
    : Component(ctx, "grpc", this, path) {}
EventServiceClient::~EventServiceClient() {}

using std::placeholders::_1;
class EventServiceClientImpl : public EventServiceClient {
 public:
  EventServiceClientImpl(
      Context const& ctx,
      std::string const& path,
      std::string const& server_address)
      : EventServiceClient(ctx, path),
        in_events_(
            this,
            "events",
            std::bind(&EventServiceClientImpl::onEvents, this, _1)),
        out_events_(this, "events"),
        server_address_{server_address} {
    this->getContextStrand().getAsioStrand().post(
        [this]() { this->connect(); });
  }
  virtual ~EventServiceClientImpl() = default;

  virtual Input<Shared<EventAndPayload>>& inEvents() { return in_events_; }
  virtual Output<Shared<EventAndPayload>>& outEvents() { return out_events_; }

  void subscribeToEvents() {
    grpc::ClientContext context;
    core::proto::SubscribeToEventsRequest request;
    core::proto::SubscribeToEventsReply reply;
    std::unique_ptr<grpc::ClientReader<core::proto::SubscribeToEventsReply>>
        reader(stub_->subscribeToEvents(&context, request));

    FARM_INFO("Start subscribing to events.");

    while (true) {
      if (reader->Read(&reply)) {
        *reply.mutable_event()->add_timestamps() = makeRecvStamp();
        FARM_INFO("{}", reply.event().DebugString());
        out_events_.send(EventAndPayload::make(reply.event(), reply.payload()));
      } else {
        FARM_WARN("Events stream ended.");
        break;
      }
    }
    FARM_INFO("Stop subscribing to events.");
  }
  void connect() {
    stub_ = core::proto::EventService::NewStub(grpc::CreateChannel(
        server_address_, grpc::InsecureChannelCredentials()));
    subscribeToEvents();
  }
  void onEvents(SharedEventAndPayload event_and_payload) {
    core::proto::PublishEventRequest request;
    request.mutable_event()->CopyFrom(event_and_payload->event());
    request.set_payload(event_and_payload->readPayload());

    *request.mutable_event()->add_timestamps() = makeSendStamp();
    core::proto::PublishEventReply reply;
    stub_->publishEvent(&context_, request, &reply);
    FARM_INFO("publish: {}", reply.ShortDebugString());
  }

  Input<SharedEventAndPayload> in_events_;
  Output<SharedEventAndPayload> out_events_;
  std::string server_address_;

  grpc::ClientContext context_;
  std::unique_ptr<core::proto::EventService::Stub> stub_;
};

Shared<EventServiceClient> EventServiceClient::create(
    Context const& ctx,
    std::string const& path,
    std::string const& server_address) {
  return std::make_shared<EventServiceClientImpl>(ctx, path, server_address);
}

}  // namespace farm_ng
