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
#include <farm_ng/core/pipeline/thread_pool.h>
#include <farm_ng/core/prototools/event_log_writer.h>
#include <farm_ng/core/prototools/event_service.h>
#include <grpcpp/grpcpp.h>
namespace farm_ng {

EventServiceClient::EventServiceClient(
    Context const& ctx,
    std::string const& path,
    core::proto::EventSubscriptions const& subscriptions)
    : Component(ctx, "grpc", this, path), subscriptions_(subscriptions) {}
core::proto::EventSubscriptions const& EventServiceClient::getSubscriptions()
    const {
  return subscriptions_;
}

EventServiceClient::~EventServiceClient() {}

using std::placeholders::_1;
class EventServiceClientImpl : public EventServiceClient {
 public:
  EventServiceClientImpl(
      Context const& ctx,
      std::string const& path,
      std::string const& server_address,
      core::proto::EventSubscriptions const& subscriptions)

      : EventServiceClient(ctx, path, subscriptions),
        in_events_(
            this,
            "events",
            std::bind(&EventServiceClientImpl::onEvents, this, _1)),
        out_events_(this, "events"),
        server_address_{server_address} {
    subscriber_pool_.getAsioIoContext().post([this]() { this->connect(); });
    getContextStrand().getAsioStrand().post([this]() {
      subscriber_pool_.start(getSubscriptions().subscriptions_size());
    });
  }

  virtual ~EventServiceClientImpl() = default;

  virtual Input<Shared<EventAndPayload>>& inEvents() { return in_events_; }
  virtual Output<Shared<EventAndPayload>>& outEvents() { return out_events_; }

  // subscribeToEvents is running in the subscriber pool, a thread per
  // subscription; because we are using blocking grpc calls and don't want to
  // block the pipeline.
  void subscribeToEvents(core::proto::EventSubscription const& subscription) {
    FARM_INFO(
        "Start subscribing to events: {}", subscription.ShortDebugString());

    grpc::ClientContext context;
    core::proto::SubscribeToEventsRequest request;
    request.mutable_subscription()->CopyFrom(subscription);

    core::proto::SubscribeToEventsReply reply;
    std::unique_ptr<grpc::ClientReader<core::proto::SubscribeToEventsReply>>
        reader(stub_->subscribeToEvents(&context, request));

    while (true) {
      if (reader->Read(&reply)) {
        *reply.mutable_event()->add_timestamps() = makeRecvStamp();
        FARM_INFO("{}", reply.event().DebugString());
        out_events_.send(EventAndPayload::make(reply.event(), reply.payload()));
      } else {
        FARM_WARN("Events stream ended: {}", subscription.ShortDebugString());
        break;
      }
    }

    FARM_INFO(
        "Stop subscribing to events: {}", subscription.ShortDebugString());
    // TODO handle reconnect
    // https://grpc.github.io/grpc/cpp/md_doc_connectivity-semantics-and-api.html
  }

  // connect is running in the subscriber pool so it doesn't block the pipeline
  // because we're using grpc blocking calls
  void connect() {
    stub_ = core::proto::EventService::NewStub(grpc::CreateChannel(
        server_address_, grpc::InsecureChannelCredentials()));

    for (core::proto::EventSubscription subscription :
         getSubscriptions().subscriptions()) {
      subscriber_pool_.getAsioIoContext().post(
          [this, subscription]() { subscribeToEvents(subscription); });
    }
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

  ThreadPool subscriber_pool_;

  Input<SharedEventAndPayload> in_events_;
  Output<SharedEventAndPayload> out_events_;
  std::string server_address_;

  grpc::ClientContext context_;
  std::unique_ptr<core::proto::EventService::Stub> stub_;
};

Shared<EventServiceClient> EventServiceClient::create(
    Context const& ctx,
    std::string const& path,
    std::string const& server_address,
    core::proto::EventSubscriptions const& subscriptions) {
  return std::make_shared<EventServiceClientImpl>(
      ctx, path, server_address, subscriptions);
}

}  // namespace farm_ng
