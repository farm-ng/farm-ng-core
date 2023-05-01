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

#include <farm_ng/core/event.pb.h>
#include <farm_ng/core/prototools/event_and_payload.h>

#include <exception>
#include <filesystem>
#include <memory>
#include <string>

namespace farm_ng {
class EventLogEof : public std::runtime_error {
 public:
  explicit EventLogEof(std::string const& what) : std::runtime_error(what) {}
};
class EventLogExist : public std::runtime_error {
 public:
  explicit EventLogExist(std::string const& what) : std::runtime_error(what) {}
};

class EventLogReaderImpl;
class EventLogPos : public EventAndPayload {
 public:
  EventLogPos(
      core::proto::Event event,
      std::streampos pos,
      std::weak_ptr<EventLogReaderImpl> log);

  virtual ~EventLogPos() = default;
  core::proto::Event const& event() const override;
  std::string const& readPayload() const override;
  void stamp(core::proto::Timestamp const& stamp) const override;

 private:
  mutable core::proto::Event event_;
  std::string payload_;
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

  // This function throws EventLogEof if the end of file is reached or a message
  // can not be decoded (typically due to an interrupted process).
  EventLogPos readNextEvent(std::string* payload = nullptr);

  // Returns an index of all the events contained in the file.  This function
  // caches the index, so its only computed once; the index requires seeking
  // through the entire file and decoding the events.  It should be fast as
  // payloads are skipped with seekg and not decoded.
  std::vector<EventLogPos> const& getIndex();

  /// Returns the path including the fileaname
  [[nodiscard]] std::filesystem::path getPath() const;

  /// Reset the writer to the beginning of the file
  void reset();

 private:
  std::shared_ptr<EventLogReaderImpl> impl_;
};

// finds the first matching stamp in the event.  If no stamp is found this
// returns a nullptr. Note that this does not allocate the stamp, its a
// reference to the underlying Timestamp owned by the event.
core::proto::Timestamp const* getStamp(
    core::proto::Event const& event,
    std::string const& clock_name,
    std::string const& semantics);

struct EventTimeCompareClockAndSemantics {
  std::string clock_name;
  std::string semantics;
  // precondition:
  //    lhs and rhs both contain a stamp from the target clock_name and
  //    semantics.
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
