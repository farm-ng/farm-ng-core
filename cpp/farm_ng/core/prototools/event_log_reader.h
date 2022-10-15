// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

// Copyright (c) farm-ng, inc. All rights reserved.

#pragma once

#include "farm_ng/core/event.pb.h"

#include <exception>
#include <filesystem>
#include <memory>
#include <string>

namespace farm_ng {
class EventLogEOF : public std::runtime_error {
 public:
  EventLogEOF(std::string const& what) : std::runtime_error(what) {}
};
class EventLogExist : public std::runtime_error {
 public:
  EventLogExist(std::string const& what) : std::runtime_error(what) {}
};

class EventLogReaderImpl;
class EventLogPos {
 public:
  EventLogPos(
      core::proto::Event event,
      std::streampos pos,
      std::weak_ptr<EventLogReaderImpl> log);

  core::proto::Event const& event() const;
  std::string readPayload() const;

 private:
  core::proto::Event event_;
  std::streampos pos_;
  std::weak_ptr<EventLogReaderImpl> log_;
};

/// Implementation of the `EventLogReader` class
///
class EventLogReaderImpl
    : public std::enable_shared_from_this<EventLogReaderImpl> {
 public:
  /// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-dtor
  virtual ~EventLogReaderImpl() {}

  virtual void reset() = 0;

  /// Returns next event.
  virtual EventLogPos readNextEvent(std::string* payload = nullptr) = 0;

  virtual std::string readPayload(
      core::proto::Event const& event, std::streampos pos) = 0;

  /// Returns the path including the fileaname
  virtual std::filesystem::path getPath() const = 0;

  std::vector<EventLogPos> const& getIndex();

 private:
  std::vector<EventLogPos> index_;
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

  /// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-dtor
  virtual ~EventLogReader();

  EventLogPos readNextEvent(std::string* payload = nullptr);

  std::vector<EventLogPos> const& getIndex();

  /// Returns the path including the fileaname
  std::filesystem::path getPath() const;

  /// Reset the writer to the beginning of the file
  void reset();

 private:
  std::shared_ptr<EventLogReaderImpl> impl_;
};

core::proto::Timestamp const* getStamp(
    core::proto::Event const& event,
    std::string const& clock_name,
    std::string const& semantics);

struct EventTimeCompare {
  std::string clock_name;
  std::string semantics;
  bool operator()(EventLogPos const& lhs, EventLogPos const& rhs) const;
};

// Returns events in time order, using the given reference clock and semantics
// If two timestamps are exactly the same, the uri().path() of the event is used
// to disambiguate the sorting.
std::vector<EventLogPos> eventLogTimeOrderedIndex(
    std::string const& clock_name,
    std::string const& semantics,
    std::vector<EventLogReader> const& readers);

}  // namespace farm_ng
