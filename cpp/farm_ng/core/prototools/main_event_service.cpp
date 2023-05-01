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
#include <grpcpp/grpcpp.h>
#include <unistd.h>

#include <csignal>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>

namespace farm_ng {

class EventService : public core::proto::EventService::Service {
 public:
  EventService() = default;
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
    FARM_INFO("Stopping EventService {}", server_address_);
    if (server_) {
      server_->Shutdown();
    }
    server_.reset();
  }

  virtual ::grpc::Status subscribeToEvents(
      ::grpc::ServerContext* context,
      ::farm_ng::core::proto::SubscribeToEventsRequest const* request,
      ::grpc::ServerWriter< ::farm_ng::core::proto::SubscribeToEventsReply>*
          writer) override {
    return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
  }
  virtual ::grpc::Status publishEvent(
      ::grpc::ServerContext* context,
      ::farm_ng::core::proto::PublishEventRequest const* request,
      ::farm_ng::core::proto::PublishEventReply* response) override {
    return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
  }
  virtual ::grpc::Status publishEvents(
      ::grpc::ServerContext* context,
      ::grpc::ServerReader< ::farm_ng::core::proto::PublishEventsRequest>*
          reader,
      ::farm_ng::core::proto::PublishEventsReply* response) {
    return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
  }

  std::string server_address_;
  std::unique_ptr<grpc::Server> server_;
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
