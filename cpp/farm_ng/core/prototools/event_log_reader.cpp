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

#include "farm_ng/core/prototools/event_log_reader.h"

#include "farm_ng/core/logging/logger.h"

#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>

namespace farm_ng {

namespace {
class EventLogReaderImpl : public EventLogReaderBase {
 public:
  EventLogReaderImpl() : instream_(), log_path_(), index_() {}
  static ExpectShared<EventLogReaderImpl> fromPath(
      std::filesystem::path const& log_path) noexcept {
    auto shared_impl = Shared<EventLogReaderImpl>::make();

    shared_impl->log_path_ = log_path;

    FARM_TRY(auto, ok, shared_impl->reset());

    return shared_impl;
  }

  Expected<Success> reset() noexcept {
    instream_ = std::ifstream(log_path_.string(), std::ofstream::binary);
    if (!instream_) {
      return FARM_UNEXPECTED("Could not open file: {}", log_path_);
    }

    return Success{};
  }
  auto readBytes(uint64_t n_bytes) -> Expected<std::string> {
    FARM_ASSERT(instream_.good());
    std::string str;
    str.resize(n_bytes);
    instream_.read(str.data(), n_bytes);
    if (!instream_) {
      FARM_TRY(auto, ok, reset());
      return FARM_UNEXPECTED("Could not read data");
    }
    return str;
  }

  auto readEventSize() noexcept -> Expected<uint64_t> {
    FARM_ASSERT(instream_.good());
    uint32_t n_bytes = 0;
    instream_.read(reinterpret_cast<char*>(&n_bytes), sizeof(n_bytes));
    if (!instream_) {
      FARM_TRY(auto, ok, reset());
      return FARM_UNEXPECTED("Could not read packet length header");
    }
    return n_bytes;
  }

  Expected<std::pair<core::proto::Event, std::streampos>> readNextEventImpl(
      std::string* payload) noexcept final {
    FARM_ASSERT(instream_.good());
    core::proto::Event event;
    FARM_TRY(auto, read, readEventSize());
    FARM_TRY(auto, bytes, readBytes(read));
    if (!event.ParseFromString(bytes)) {
      FARM_TRY(auto, ok, reset());
      return FARM_UNEXPECTED("Could not parse event.");
    }
    std::streampos pos = instream_.tellg();
    if (payload != nullptr) {
      FARM_TRY(auto, payload_bytes, readBytes(event.payload_length()));
      *payload = payload_bytes;
    } else {
      instream_.seekg(event.payload_length(), std::ifstream::cur);
      if (!instream_) {
        FARM_TRY(auto, ok, reset());

        return FARM_UNEXPECTED("Could not seek past payload.");
      }
    }
    return std::make_pair(event, pos);
  }

  auto readPayload(core::proto::Event const& event, std::streampos pos) noexcept
      -> Expected<std::string> {
    FARM_ASSERT(instream_.good());

    instream_.seekg(pos);
    if (!instream_) {
      FARM_TRY(auto, ok, reset());
    }
    return readBytes(event.payload_length());
  }

  std::vector<EventLogPos>& index() { return index_; }
  std::vector<EventLogPos> const& index() const { return index_; }

  std::ifstream instream_;
  std::filesystem::path log_path_;
  std::vector<EventLogPos> index_;
};
}  // namespace

auto getStamp(
    core::proto::Event const& event,
    std::string const& clock_name,
    std::string const& semantics) -> core::proto::Timestamp const* {
  for (core::proto::Timestamp const& stamp : event.timestamps()) {
    if (stamp.clock_name() == clock_name && stamp.semantics() == semantics) {
      return &stamp;
    }
  }
  return nullptr;
}

auto EventTimeCompareClockAndSemantics::operator()(
    EventLogPos const& lhs, EventLogPos const& rhs) const -> bool {
  auto const* maybe_lhs_stamp = getStamp(lhs.event(), clock_name, semantics);
  auto const* maybe_rhs_stamp = getStamp(rhs.event(), clock_name, semantics);
  FARM_ASSERT(
      !!maybe_lhs_stamp,
      "Event has no stamp from the reference clock: clock_name {} semantics {} "
      "event: {}",
      clock_name,
      semantics,
      lhs.event().ShortDebugString());
  FARM_ASSERT(
      !!maybe_rhs_stamp,
      "Event has no stamp from the reference clock: clock_name {} semantics {} "
      "event: {}",
      clock_name,
      semantics,
      rhs.event().ShortDebugString());
  return maybe_lhs_stamp->stamp() < maybe_rhs_stamp->stamp() ||
         (!(maybe_rhs_stamp->stamp() < maybe_lhs_stamp->stamp()) &&
          lhs.event().uri().path() < rhs.event().uri().path());
}

auto eventLogTimeOrderedIndex(
    std::string const& clock_name,
    std::string const& semantics,
    std::vector<EventLogReader> const& readers) -> std::vector<EventLogPos> {
  std::vector<EventLogPos> ordered_index;
  for (EventLogReader reader : readers) {
    for (EventLogPos const& pos : reader.getIndex()) {
      if (getStamp(pos.event(), clock_name, semantics) == nullptr) {
        FARM_WARN(
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
      EventTimeCompareClockAndSemantics{clock_name, semantics});
  return ordered_index;
}

Expected<EventLogReader> EventLogReader::fromPath(
    std::filesystem::path const& log_path) noexcept {
  FARM_TRY(auto, reader_impl, EventLogReaderImpl::fromPath(log_path));
  EventLogReader reader;
  reader.impl_ = Shared(reader_impl).sharedPtr();
  return reader;
}

Expected<EventLogPos> EventLogReader::readNextEvent(
    std::string* payload) noexcept {
  FARM_TRY(auto, pair, impl_->readNextEventImpl(payload));

  return EventLogPos(pair.first, pair.second, this->impl_);
}

auto EventLogReader::getIndex() noexcept -> std::vector<EventLogPos> const& {
  if (impl_->index().empty()) {
    auto reset_ok = reset();
    if (!reset_ok) {
      FARM_WARN("{}", reset_ok.error());
      return impl_->index();
    }

    while (auto maybe_event = readNextEvent()) {
      impl_->index().push_back(FARM_UNWRAP(maybe_event));
    }
  }
  return impl_->index();
}

auto EventLogReader::readPayload(
    core::proto::Event const& event, std::streampos pos) const noexcept
    -> Expected<std::string> {
  return impl_->readPayload(event, pos);
}

auto EventLogReader::reset() noexcept -> Expected<Success> {
  return impl_->reset();
}

auto EventLogPos::event() const -> core::proto::Event const& { return event_; }

auto EventLogPos::readPayload() const -> Expected<std::string> {
  auto maybe_reader = reader_weak_ptr_.lock();

  if (!maybe_reader) {
    return FARM_UNEXPECTED("Log closed: {}", event_.ShortDebugString());
  }
  return maybe_reader->readPayload(this->event_, this->pos_);
}

}  // namespace farm_ng
