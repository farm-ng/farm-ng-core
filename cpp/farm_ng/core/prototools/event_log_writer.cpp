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

// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng/core/prototools/event_log_writer.h"

#include "farm_ng/core/logging/logger.h"
#include "farm_ng/core/prototools/event_log_reader.h"

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
auto getAuthority() -> std::string const& {
  static std::string Static_Host_Name = getAuthoritySlow();
  return Static_Host_Name;
}
auto monotonic() -> double {
  double now = 1e-6 * std::chrono::duration_cast<std::chrono::microseconds>(
                          std::chrono::steady_clock::now().time_since_epoch())
                          .count();
  return now;
}

}  // namespace

auto makeWriteStamp() -> core::proto::Timestamp {
  core::proto::Timestamp stamp;
  stamp.set_stamp(monotonic());
  stamp.set_semantics("log/write");
  stamp.set_clock_name(getAuthority() + "/monotonic");
  return stamp;
}

class EventLogWriterBinaryImpl : public EventLogWriterImpl {
 public:
  EventLogWriterBinaryImpl(std::filesystem::path const& log_path)
      : log_path(log_path), out(log_path.string(), std::ofstream::binary) {}

  void write(
      std::string path,
      google::protobuf::Message const& message,
      google::protobuf::RepeatedPtrField<core::proto::Timestamp> const&
          timestamps) override {
    std::string payload;
    message.SerializeToString(&payload);
    core::proto::Event event;
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
    event.add_timestamps()->CopyFrom(makeWriteStamp());
    event.set_payload_length(payload.size());
    std::string event_str;
    event.SerializeToString(&event_str);
    uint32_t n_bytes = event_str.size();
    out.write(reinterpret_cast<char const*>(&n_bytes), sizeof(n_bytes));
    out << event_str;
    out << payload;
    out.flush();
  }

  std::filesystem::path log_path;
  std::ofstream out;
};

EventLogWriter::EventLogWriter(std::filesystem::path const& log_path)
    : log_path_(log_path) {
  // generate the directory tree in case it's empty or doesn't exist

  std::filesystem::path path_prefix = log_path;
  path_prefix.remove_filename();
  if (!path_prefix.empty() && !std::filesystem::exists(path_prefix)) {
    if (!std::filesystem::create_directories(path_prefix)) {
      throw EventLogExist(FARM_FORMAT(
          "Could not create log directory: {}", path_prefix.string()));
    }
  }
  impl_ = std::make_unique<EventLogWriterBinaryImpl>(log_path);
}

EventLogWriter::~EventLogWriter() noexcept { impl_.reset(nullptr); }

void EventLogWriter::write(
    std::string const& path,
    google::protobuf::Message const& message,
    std::vector<core::proto::Timestamp> const& timestamps) {
  google::protobuf::RepeatedPtrField<core::proto::Timestamp> proto_stamps;
  for (auto stamp : timestamps) {
    proto_stamps.Add()->CopyFrom(stamp);
  }
  impl_->write(path, message, proto_stamps);
}

void EventLogWriter::write(
    std::string const& path,
    google::protobuf::Message const& message,
    google::protobuf::RepeatedPtrField<core::proto::Timestamp> const&
        timestamps) {
  impl_->write(path, message, timestamps);
}

}  // namespace farm_ng
