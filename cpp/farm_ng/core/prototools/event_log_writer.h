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

#pragma once

#include "farm_ng/core/event.pb.h"
#include "farm_ng/core/logging/expected.h"


#include <filesystem>
#include <memory>
#include <string>

namespace farm_ng {

/// Implementation of the `EventLogWriter` class
///
class EventLogWriterImpl {
 public:
  /// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-dtor
  virtual ~EventLogWriterImpl() {}

  virtual void write(
      std::string path,
      google::protobuf::Message const& message,
      google::protobuf::RepeatedPtrField<core::proto::Timestamp> const&
          timestamps) = 0;

  /// Returns the number of bytes written to the file so far
  virtual ssize_t getBytesWritten() = 0;
};

/// Class that serializes incoming protobuf events to a file in disk.
class EventLogWriter {
 public:
  /// Main constructor of the class.
  ///
  /// Precondition: It must be possible to create the folder ``log_path_`` if it
  /// does not exist. If ``log-path`` does exist, it must be a folder and it
  /// must be empty.
  ///
  static Expected<EventLogWriter> fromPath(std::filesystem::path const& log_path);

  /// Main destructor
  virtual ~EventLogWriter() noexcept;

  /// Writes an incoming protobuf in the log file
  void write(
      std::string const& path,
      google::protobuf::Message const& message,
      std::vector<core::proto::Timestamp> const& timestamps =
          std::vector<core::proto::Timestamp>()) noexcept;
  void write(
      std::string const& path,
      google::protobuf::Message const& message,
      google::protobuf::RepeatedPtrField<core::proto::Timestamp> const&
          timestamps) noexcept;

  /// Returns the path including the fileaname
  [[nodiscard]] std::filesystem::path getPath() const { return log_path_; }

  /// Returns the number of bytes written to the file so far
  [[nodiscard]] ssize_t getBytesWritten() noexcept;

 private:
   EventLogWriter(){}

  // The log path including the filename
  std::filesystem::path log_path_;
  // Implementation pointer of the class
  std::unique_ptr<EventLogWriterImpl> impl_;
};

core::proto::Timestamp makeWriteStamp();

}  // namespace farm_ng
