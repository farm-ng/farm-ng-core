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

#include "farm_ng/core/logging/expected.h"
#include "farm_ng/core/misc/shared.h"

#include <farm_ng/core/event.pb.h>

#include <exception>
#include <filesystem>
#include <memory>
#include <string>

namespace farm_ng {

/// Exception thrown when the event log file does not exist.
class EventLogExist : public std::runtime_error {
 public:
  /// From error message string.
  explicit EventLogExist(std::string const& what) : std::runtime_error(what) {}
};

class EventLogPos;

/// Position in the event log file (Base class)
struct EventLogReaderBase {
  virtual ~EventLogReaderBase() {}

  virtual Expected<std::pair<core::proto::Event, std::streampos>>

  /// Implementation detail of readNextEvent.
  readNextEventImpl(std::string* payload) noexcept = 0;

  /// Returns an index of all the events contained in the file.
  virtual std::vector<EventLogPos>& index() = 0;

  /// reads payload for event at pos
  [[nodiscard]] virtual auto readPayload(
      core::proto::Event const& event, std::streampos pos) noexcept
      -> Expected<std::string> = 0;

  /// Resets the reader to the beginning of the file.
  [[nodiscard]] virtual Expected<Success> reset() noexcept = 0;
};

/// Position in the event log file.
class EventLogPos {
 public:
  /// Create a new EventLogPos.
  ///
  /// A reference to the EventLogReader is stored in a weak_ptr.
  EventLogPos(
      core::proto::Event event,
      std::streampos pos,
      Shared<EventLogReaderBase> reader_impl)
      : event_(std::move(event)),
        pos_(pos),
        reader_weak_ptr_(reader_impl.sharedPtr()) {}

  /// Returns the event.
  core::proto::Event const& event() const;

  /// Returns the payload.
  Expected<std::string> readPayload() const;

 private:
  core::proto::Event event_;
  std::streampos pos_;
  std::weak_ptr<EventLogReaderBase> reader_weak_ptr_;
};

auto getStamp(
    core::proto::Event const& event,
    std::string const& clock_name,
    std::string const& semantics) -> core::proto::Timestamp const*;

/// Reader to deserialize data written by the EventLogWriter.
///
class EventLogReader {
 public:
  /// Open's log file to read.
  ///
  /// Returns error if files could not be opened or if logfile does not
  /// contain a valid header.
  static Expected<EventLogReader> fromPath(
      std::filesystem::path const& log_path) noexcept;

  /// This function returns error if the end of file is reached or a message
  /// can not be decoded (typically due to an interrupted process).
  Expected<EventLogPos> readNextEvent(std::string* payload = nullptr) noexcept;

  /// Returns an index of all the events contained in the file.  This function
  /// caches the index, so its only computed once; the index requires seeking
  /// through the entire file and decoding the events.  It should be fast as
  /// payloads are skipped with seekg and not decoded.
  std::vector<EventLogPos> const& getIndex() noexcept;

  /// Returns the path including the fileaname
  [[nodiscard]] std::filesystem::path getPath() const noexcept;

  /// Reset the reader to the beginning of the file
  [[nodiscard]] Expected<Success> reset() noexcept;

 private:
  auto readPayload(core::proto::Event const& event, std::streampos pos)
      const noexcept -> Expected<std::string>;

  EventLogReader() {}
  std::shared_ptr<EventLogReaderBase> impl_;
};

/// Compare two EventLogPos by their timestamp.
struct EventTimeCompareClockAndSemantics {
  /// The clock_name and semantics to use for comparison.
  std::string clock_name;

  /// meaning of that clock / type of timestamp
  std::string semantics;

  /// precondition:
  ///    lhs and rhs both contain a stamp from the target clock_name and
  ///    semantics.
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
