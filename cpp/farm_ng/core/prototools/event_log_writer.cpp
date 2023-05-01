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

#include <climits>
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>

namespace farm_ng {
class EventLogWriterBinaryImpl : public EventLogWriterImpl {
 public:
  EventLogWriterBinaryImpl(std::filesystem::path const& log_path)
      : log_path(log_path), out(log_path.string(), std::ofstream::binary) {}

  void write(SharedEventAndPayload const& event_and_payload) override {
    event_and_payload->stamp(makeWriteStamp());
    std::string event_str;
    event_and_payload->event().SerializeToString(&event_str);
    uint32_t n_bytes = event_str.size();
    out.write(reinterpret_cast<char const*>(&n_bytes), sizeof(n_bytes));
    out << event_str;
    out << event_and_payload->readPayload();
    out.flush();
  }
  void write(
      std::string path,
      google::protobuf::Message const& message,
      google::protobuf::RepeatedPtrField<core::proto::Timestamp> const&
          timestamps) override {
    write(EventAndPayload::make(path, message, timestamps));
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
