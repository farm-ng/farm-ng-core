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

#include <CLI/CLI.hpp>
#include <farm_ng/core/event_service.grpc.pb.h>
#include <farm_ng/core/logging/logger.h>
#include <farm_ng/core/pipeline/input.h>
#include <farm_ng/core/pipeline/output.h>
#include <farm_ng/core/prototools/event_and_payload.h>
#include <grpcpp/grpcpp.h>
#include <unistd.h>

#include <csignal>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>

namespace farm_ng {

class EventService : public core::proto::EventService::Service {
  typedef Output<SharedEventAndPayload> Publisher;
  typedef Input<SharedEventAndPayload> Subscriber;
  typedef std::unique_ptr<Publisher> PublisherPtr;

 public:
  EventService() : component_(context_, "server", "EventService") {}

  ~EventService() = default;
  void run(std::string const& server_address) {
    FARM_ASSERT(!server_, "Server already running");
    server_address_ = server_address;
    FARM_INFO("Starting EventService {}", server_address_);
    grpc::ServerBuilder builder;
    builder.AddListeningPort(
        server_address_, grpc::InsecureServerCredentials());
    builder.RegisterService(this);
    server_ = builder.BuildAndStart();

    server_->Wait();
  }
  void shutdown() {
    context_.clearWorkGuard();
    context_.getAsioIoContext().stop();
    {
      std::unique_lock<std::mutex> lock(topic_mtx_);
      for (auto context : contexts_) {
        context->clearWorkGuard();
        context->getAsioIoContext().stop();
      }
    }
    FARM_INFO("Stopping EventService {}", server_address_);
    if (server_) {
      server_->Shutdown();
    }
    server_.reset();
  }

  Publisher& getPublisher(std::string const& topic) {
    std::lock_guard<std::mutex> lock(topic_mtx_);
    PublisherPtr& pub = topics_[topic];
    if (!pub) {
      pub = std::make_unique<Publisher>(&component_, topic);
    }
    return *pub;
  }

  virtual ::grpc::Status subscribeToEvents(
      ::grpc::ServerContext* context,
      ::farm_ng::core::proto::SubscribeToEventsRequest const* request,
      ::grpc::ServerWriter<::farm_ng::core::proto::SubscribeToEventsReply>*
          writer) override {
    auto subscription = request->subscription();

    std::shared_ptr<Context> sub_ctx = std::make_shared<Context>();
    Component sub_component(
        *sub_ctx, "client", uriString(subscription.consumer_uri()));
    core::proto::SubscribeToEventsReply reply;

    ::grpc::Status status(::grpc::StatusCode::OK, "");
    uint64_t k = 0;
    uint64_t n = 0;
    uint64_t every_n = subscription.every_n();
    uint64_t max_k = subscription.k_messages();
    bool stop = false;
    Subscriber sub(
        &sub_component,
        subscription.topic(),
        [&](SharedEventAndPayload event_and_payload) {
          if (((n++) % every_n) != 0) {
            return;
          }

          reply.mutable_event()->CopyFrom(event_and_payload->event());
          reply.set_payload(event_and_payload->readPayload());
          *reply.mutable_event()->add_timestamps() = makeSendStamp();
          if (!writer->Write(reply)) {
            status =
                ::grpc::Status(::grpc::StatusCode::CANCELLED, "Stream closed.");
            sub_ctx->getAsioIoContext().stop();
          }
          k++;
          if (max_k != 0 && k >= max_k) {
            status = ::grpc::Status(::grpc::StatusCode::OK, "K reached.");
            sub_ctx->getAsioIoContext().stop();
          }
        },
        InputConfig{.max_queue_length = 1});

    Publisher& pub = getPublisher(subscription.topic());
    pub.connect(sub);

    {
      std::unique_lock<std::mutex> lock(topic_mtx_);
      contexts_.insert(sub_ctx);
    }
    sub_ctx->run();
    {
      std::unique_lock<std::mutex> lock(topic_mtx_);
      contexts_.erase(sub_ctx);
    }

    return status;
  }

  virtual ::grpc::Status publishEvent(
      ::grpc::ServerContext* context,
      ::farm_ng::core::proto::PublishEventRequest const* request,
      ::farm_ng::core::proto::PublishEventReply* response) override {
    Publisher& pub = getPublisher(request->event().uri().path());
    pub.send(EventAndPayload::make(request->event(), request->payload()));
    response->Clear();
    return ::grpc::Status(::grpc::StatusCode::OK, "");
  }
  virtual ::grpc::Status publishEvents(
      ::grpc::ServerContext* context,
      ::grpc::ServerReader<::farm_ng::core::proto::PublishEventsRequest>*
          reader,
      ::farm_ng::core::proto::PublishEventsReply* response) {
    farm_ng::core::proto::PublishEventsRequest request;
    while (reader->Read(&request)) {
      Publisher& pub = getPublisher(request.event().uri().path());
      pub.send(EventAndPayload::make(request.event(), request.payload()));
    }
    response->Clear();
    return ::grpc::Status(::grpc::StatusCode::OK, "");
  }

  std::string server_address_;
  std::unique_ptr<grpc::Server> server_;
  std::mutex topic_mtx_;
  Context context_;
  std::set<std::shared_ptr<Context>> contexts_;
  Component component_;
  std::unordered_map<std::string, PublisherPtr> topics_;
};

farm_ng::EventService& getEventService() {
  static farm_ng::EventService service;
  return service;
}

}  // namespace farm_ng

std::atomic<int> g_signal_status = 0;

// Define the function to be called when ctrl-c (SIGINT) is sent to process
void signal_callback_handler(int signum) {
  std::cout << "Caught signal " << signum << std::endl;
  g_signal_status = signum;
}

int main(int argc, char** argv) {
  signal(SIGINT, signal_callback_handler);
  signal(SIGTERM, signal_callback_handler);

  std::string server_address = "0.0.0.0:95076";
  CLI::App app{argv[0]};
  app.add_option("--address", server_address, "GRPC host server address");
  CLI11_PARSE(app, argc, argv);

  std::thread run_thread(
      [server_address]() { farm_ng::getEventService().run(server_address); });
  while (true) {
    if (g_signal_status != 0) {
      farm_ng::getEventService().shutdown();
      run_thread.join();
      return g_signal_status;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  run_thread.join();
  return 0;
}
