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

/// Implementation of the `EventLogReader` class
///
class EventLogReaderImpl {
 public:
  /// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-dtor
  virtual ~EventLogReaderImpl() {}

  /// Returns next event.
  virtual std::tuple<core::proto::Event, std::streampos> readNextEvent(
      std::string* payload = nullptr) = 0;

  virtual std::string readPayload(
      core::proto::Event const& event, std::streampos pos) = 0;

  /// Returns the path including the fileaname
  virtual std::filesystem::path getPath() const = 0;
};

/// Reader to deserialize data written by the EventLogWriter.
///
class EventLogReader {
 public:
  /// Open's log file to read.
  ///
  /// Throws runtime-error if files could not be opened or if logfile does not
  /// contain a valid header.
  explicit EventLogReader(std::filesystem::path const& log_path);

  EventLogReader(EventLogReader&&) = default;
  EventLogReader& operator=(EventLogReader&&) = default;

  /// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-dtor
  virtual ~EventLogReader();

  std::tuple<core::proto::Event, std::streampos> readNextEvent(
      std::string* payload = nullptr);

  std::string readPayload(core::proto::Event const& event, std::streampos pos);

  /// Returns the path including the fileaname
  std::filesystem::path getPath() const;

  /// Reset the writer to the beginning of the file
  void reset();

 private:
  std::unique_ptr<EventLogReaderImpl> impl_;
};

}  // namespace farm_ng
