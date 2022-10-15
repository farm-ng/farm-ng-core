// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

// Copyright (c) farm-ng, inc. All rights reserved.

#pragma once

#include "farm_ng/core/event.pb.h"

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
  EventLogWriter(std::filesystem::path const& log_path);

  /// Main destructor
  virtual ~EventLogWriter() noexcept;

  /// Writes an incoming protobuf in the log file
  void write(
      std::string const& path,
      google::protobuf::Message const& message,
      std::vector<core::proto::Timestamp> const& timestamps =
          std::vector<core::proto::Timestamp>());
  void write(
      std::string const& path,
      google::protobuf::Message const& message,
      google::protobuf::RepeatedPtrField<core::proto::Timestamp> const&
          timestamps);

  /// Returns the path including the fileaname
  std::filesystem::path getPath() const { return log_path_; }

 private:
  // The log path including the filename
  std::filesystem::path log_path_;
  // Implementation pointer of the class
  std::unique_ptr<EventLogWriterImpl> impl_;
};

core::proto::Timestamp makeWriteStamp();

}  // namespace farm_ng
