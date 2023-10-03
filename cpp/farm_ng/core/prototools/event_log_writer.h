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

#pragma once

#include "farm_ng/core/event.pb.h"
#include "farm_ng/core/logging/expected.h"

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

namespace farm_ng {

/// Class that serializes incoming protobuf events to a file in disk.
class EventLogWriter {
 public:
  /// Creates a new EventLogWriter that writes to the file ``log_path``.
  ///
  /// Return an error if
  ///  - ``log_path`` exists and but it is not a folder or is not empty.
  ///  - ``log_path`` does not exist and it cannot be created.
  static Expected<EventLogWriter> fromPath(
      std::filesystem::path const& log_path) noexcept;

  /// Writes an incoming protobuf in the log file
  void write(
      std::string const& path,
      google::protobuf::Message const& message,
      std::vector<core::proto::Timestamp> const& timestamps =
          std::vector<core::proto::Timestamp>()) noexcept;
  void write(
      std::string const& path,
      google::protobuf::Message const& message,
      google::protobuf::RepeatedPtrField<core::proto::Timestamp>
          proto_stamps) noexcept;

  /// Returns the path including the fileaname
  [[nodiscard]] std::filesystem::path getPath() const { return log_path_; }

  /// Returns the number of bytes written to the file so far
  [[nodiscard]] ssize_t getBytesWritten() noexcept;

 private:
  EventLogWriter() { maybe_outstream_ = std::make_shared<std::ofstream>(); }

  std::filesystem::path log_path_;
  std::shared_ptr<std::ofstream> maybe_outstream_;
};

core::proto::Timestamp makeWriteStamp();

}  // namespace farm_ng
