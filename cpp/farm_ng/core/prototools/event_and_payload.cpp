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

#include "farm_ng/core/logging/logger.h"
#include "farm_ng/core/prototools/event_log_reader.h"

#include <farm_ng/core/prototools/event_and_payload.h>

#ifndef __USE_POSIX
#define __USE_POSIX
#endif
#include <unistd.h>

#include <climits>
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>

char Static_Hostname[_POSIX_HOST_NAME_MAX];
char Static_Username[_POSIX_LOGIN_NAME_MAX];

namespace farm_ng {
namespace {
auto getAuthoritySlow() -> std::string {
  char hostname[_POSIX_HOST_NAME_MAX];
  gethostname(hostname, _POSIX_HOST_NAME_MAX);
  return std::string(hostname);
}

auto monotonic() -> double {
  double now = 1e-6 * std::chrono::duration_cast<std::chrono::microseconds>(
                          std::chrono::steady_clock::now().time_since_epoch())
                          .count();
  return now;
}

core::proto::Timestamp makeSemanticStamp(std::string const& semantics) {
  core::proto::Timestamp stamp;
  stamp.set_stamp(monotonic());
  stamp.set_semantics(semantics);
  stamp.set_clock_name(getAuthority() + "/monotonic");
  return stamp;
}
}  // namespace

auto getAuthority() -> std::string const& {
  static std::string Static_Host_Name = getAuthoritySlow();
  return Static_Host_Name;
}
auto makeWriteStamp() -> core::proto::Timestamp {
  return makeSemanticStamp("log/write");
}

core::proto::Timestamp makeRecvStamp() { return makeSemanticStamp("recv"); }

core::proto::Timestamp makeSendStamp() { return makeSemanticStamp("send"); }

EventAndPayload::EventAndPayload() = default;
EventAndPayload::~EventAndPayload() = default;
class EventAndPayloadImpl : public EventAndPayload {
 public:
  EventAndPayloadImpl() = default;
  EventAndPayloadImpl(
      core::proto::Event const& event, std::string const& payload)
      : event_(event), payload_(payload) {
    event_.set_payload_length(payload_.size());
  }
  virtual ~EventAndPayloadImpl() override = default;
  virtual void stamp(core::proto::Timestamp const& stamp) const override {
    event_.add_timestamps()->CopyFrom(stamp);
  }

  virtual core::proto::Event const& event() const override { return event_; }
  virtual std::string const& readPayload() const override { return payload_; }
  mutable core::proto::Event event_;
  std::string payload_;
};
Shared<EventAndPayload> EventAndPayload::make(
    core::proto::Event const& event, std::string const& payload) {
  return std::make_shared<EventAndPayloadImpl>(event, payload);
}

Shared<EventAndPayload> EventAndPayload::make(
    std::string const& path,
    google::protobuf::Message const& message,
    google::protobuf::RepeatedPtrField<core::proto::Timestamp> const&
        timestamps) {
  std::shared_ptr<EventAndPayloadImpl> event_and_payload(
      new EventAndPayloadImpl);
  message.SerializeToString(&event_and_payload->payload_);
  core::proto::Event& event = event_and_payload->event_;
  // see py/farm_ng/core/uri.py
  core::proto::Uri* uri = event.mutable_uri();
  uri->set_scheme("protobuf");
  uri->set_authority(getAuthority());
  uri->set_path(path);
  uri->set_query(
      "type=" + message.GetDescriptor()->full_name() +
      "&pb=" + message.GetDescriptor()->file()->name());

  for (core::proto::Timestamp const& stamp : timestamps) {
    event.add_timestamps()->CopyFrom(stamp);
  }
  event.set_payload_length(event_and_payload->payload_.size());
  return event_and_payload;
}

Shared<EventAndPayload> EventAndPayload::make(
    std::string const& path,
    google::protobuf::Message const& message,
    std::vector<core::proto::Timestamp> const& timestamps) {
  google::protobuf::RepeatedPtrField<core::proto::Timestamp> proto_stamps;
  for (auto stamp : timestamps) {
    proto_stamps.Add()->CopyFrom(stamp);
  }
  return EventAndPayload::make(path, message, proto_stamps);
}

std::string uriString(core::proto::Uri const& uri) {
  if (uri.query().empty()) {
    return uri.scheme() + "://" + uri.authority() + "/" + uri.path();
  }
  return uri.scheme() + "://" + uri.authority() + "/" + uri.path() + "?" +
         uri.query();
}

}  // namespace farm_ng
