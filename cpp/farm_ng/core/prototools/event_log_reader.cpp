// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng/core/prototools/event_log_reader.h"

#include "farm_ng/core/logging/logger.h"

#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>

namespace farm_ng {

class EventLogReaderBinaryImpl : public EventLogReaderImpl {
 public:
  EventLogReaderBinaryImpl(std::filesystem::path const& log_path)
      : log_path(log_path) {
    reset();
  }

  virtual void reset() {
    in = std::ifstream(log_path.string(), std::ofstream::binary);
    if (!in) {
      throw EventLogExist(FARM_FORMAT("Could not open file: {}", log_path));
    }
  }
  std::string readBytes(uint64_t n_bytes) {
    FARM_CHECK(in.good());
    std::string str;
    str.resize(n_bytes);
    in.read(str.data(), n_bytes);
    if (!in) {
      reset();
      throw EventLogEOF("Could not read data.");
    }
    return str;
  }

  uint64_t readEventSize() {
    FARM_CHECK(in.good());
    uint32_t n_bytes = 0;
    in.read(reinterpret_cast<char*>(&n_bytes), sizeof(n_bytes));
    if (!in) {
      reset();
      throw EventLogEOF("Could not read packet length header");
    }
    return n_bytes;
  }

  virtual EventLogPos readNextEvent(std::string* payload = nullptr) override {
    FARM_CHECK(in.good());
    core::proto::Event event;
    if (!event.ParseFromString(readBytes(readEventSize()))) {
      reset();
      throw EventLogEOF("Could not parse event.");
    }
    std::streampos pos = in.tellg();
    if (payload) {
      *payload = readBytes(event.payload_length());
    } else {
      in.seekg(event.payload_length(), in.cur);
      if (!in) {
        reset();
        throw EventLogEOF("Could not seek past payload.");
      }
    }
    return EventLogPos(event, pos, this->shared_from_this());
  }

  virtual std::string readPayload(
      core::proto::Event const& event, std::streampos pos) {
    FARM_CHECK(in.good());

    in.seekg(pos);
    if (!in) {
      reset();
      throw EventLogEOF("Could not seek to read payload.");
    }
    return readBytes(event.payload_length());
  }

  std::filesystem::path getPath() const override { return log_path; }

  std::filesystem::path log_path;
  std::ifstream in;
};

EventLogPos::EventLogPos(
    core::proto::Event event,
    std::streampos pos,
    std::weak_ptr<EventLogReaderImpl> log)
    : event_(std::move(event)), pos_(pos), log_(log) {}

core::proto::Event const& EventLogPos::event() const { return event_; }

std::string EventLogPos::readPayload() const {
  std::shared_ptr<EventLogReaderImpl> log = log_.lock();
  FARM_CHECK(!!log, "Log closed: {}", event_.ShortDebugString());
  return log->readPayload(event_, pos_);
}

std::vector<EventLogPos> const& EventLogReaderImpl::getIndex() {
  if (index_.empty()) {
    reset();
    while (true) {
      try {
        index_.push_back(readNextEvent());
      } catch (EventLogEOF const& e) {
        // throws exception at end of file.
        break;
      }
    }
  }
  return index_;
}

EventLogReader::EventLogReader(std::filesystem::path const& log_path)
    : impl_(std::make_shared<EventLogReaderBinaryImpl>(log_path)) {}

EventLogReader::~EventLogReader() {}

EventLogPos EventLogReader::readNextEvent(std::string* payload) {
  return impl_->readNextEvent(payload);
}

std::vector<EventLogPos> const& EventLogReader::getIndex() {
  return impl_->getIndex();
}

std::filesystem::path EventLogReader::getPath() const {
  return impl_->getPath();
}

void EventLogReader::reset() {
  impl_ = std::make_shared<EventLogReaderBinaryImpl>(impl_->getPath());
}

core::proto::Timestamp const* getStamp(
    core::proto::Event const& event,
    std::string const& clock_name,
    std::string const& semantics) {
  for (core::proto::Timestamp const& stamp : event.timestamps()) {
    if (stamp.clock_name() == clock_name && stamp.semantics() == semantics) {
      return &stamp;
    }
  }
  return nullptr;
}

bool EventTimeCompare::operator()(
    EventLogPos const& lhs, EventLogPos const& rhs) const {
  auto maybe_lhs_stamp = getStamp(lhs.event(), clock_name, semantics);
  auto maybe_rhs_stamp = getStamp(rhs.event(), clock_name, semantics);
  if (maybe_lhs_stamp && maybe_rhs_stamp) {
    return maybe_lhs_stamp->stamp() < maybe_rhs_stamp->stamp();
  }
  return false;
}

std::vector<EventLogPos> eventLogTimeOrderedIndex(
    std::string const& clock_name,
    std::string const& semantics,
    std::vector<EventLogReader> const& readers) {
  std::vector<EventLogPos> ordered_index;
  for (EventLogReader reader : readers) {
    for (EventLogPos const& pos : reader.getIndex()) {
      if (!getStamp(pos.event(), clock_name, semantics)) {
        FARM_LOG_WARNING(
            "Event doesn't have target clock: {} clock_name: {} semantics: {}",
            pos.event().ShortDebugString(),
            clock_name,
            semantics);
        continue;
      }
      ordered_index.push_back(pos);
    }
  }
  std::sort(
      ordered_index.begin(),
      ordered_index.end(),
      EventTimeCompare{clock_name, semantics});
  return ordered_index;
}

}  // namespace farm_ng
