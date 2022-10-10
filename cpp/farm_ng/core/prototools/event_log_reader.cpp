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
      : log_path(log_path), in(log_path.string(), std::ofstream::binary) {
    if (!in) {
      throw std::runtime_error(
          FARM_FORMAT("Could not open file: {}", log_path));
    }
  }

  std::string readBytes(uint64_t n_bytes) {
    std::string str;
    str.resize(n_bytes);
    in.read(str.data(), n_bytes);
    if (!in) {
      throw std::runtime_error("Could not read data.");
    }
    return str;
  }

  uint64_t readEventSize() {
    uint64_t n_bytes = 0;
    in.read(reinterpret_cast<char*>(&n_bytes), sizeof(n_bytes));
    if (!in) {
      throw std::runtime_error("Could not read packet length header");
    }
    return n_bytes;
  }

  virtual std::tuple<core::proto::Event, std::streampos> readNextEvent(
      std::string* payload = nullptr) override {
    core::proto::Event event;
    if (!event.ParseFromString(readBytes(readEventSize()))) {
      throw std::runtime_error("Could not parse event.");
    }
    std::streampos pos = in.tellg();
    if (payload) {
      *payload = readBytes(event.payload_length());
    } else {
      in.seekg(event.payload_length(), in.cur);
      if (!in) {
        throw std::runtime_error("Could not seek past payload.");
      }
    }
    return std::make_tuple(event, pos);
  }
  virtual std::string readPayload(
      core::proto::Event const& event, std::streampos pos) {
    in.seekg(pos);
    if (!in) {
      throw std::runtime_error("Could not seek to read payload.");
    }
    return readBytes(event.payload_length());
  }

  std::filesystem::path getPath() const override { return log_path; }

  std::filesystem::path log_path;
  std::ifstream in;
};

namespace {
void resetImpl(
    std::unique_ptr<EventLogReaderImpl>& impl,
    std::filesystem::path const& log_path) {
  impl = std::make_unique<EventLogReaderBinaryImpl>(log_path);
}
}  // namespace

EventLogReader::EventLogReader(std::filesystem::path const& log_path) {
  resetImpl(impl_, log_path);
}

EventLogReader::~EventLogReader() { impl_.reset(nullptr); }

std::tuple<core::proto::Event, std::streampos> EventLogReader::readNextEvent(
    std::string* payload) {
  return impl_->readNextEvent(payload);
}

std::string EventLogReader::readPayload(
    core::proto::Event const& event, std::streampos pos) {
  return impl_->readPayload(event, pos);
}

std::filesystem::path EventLogReader::getPath() const {
  return impl_->getPath();
}

void EventLogReader::reset() { resetImpl(impl_, impl_->getPath()); }

}  // namespace farm_ng
