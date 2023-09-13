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

#include "farm_ng/core/prototools/event_log_writer.h"

#include "farm_ng/core/prototools/event_log_reader.h"

#include <farm_ng/core/logging/logger.h>
#include <farm_ng/core/misc/filesystem.h>
#include <gtest/gtest.h>

#ifndef __USE_POSIX
#define __USE_POSIX
#endif
#include <unistd.h>

using namespace farm_ng;
namespace {
auto getHostName() -> std::string {
  char hostname[_POSIX_HOST_NAME_MAX];
  gethostname(hostname, _POSIX_HOST_NAME_MAX);
  return std::string(hostname);
}
}  // namespace

TEST(event_log, roundtrip) {  // NOLINT
  auto maybe_log_dir = createUniqueTemporaryDirectory();
  auto log_dir = FARM_UNWRAP(maybe_log_dir);
  auto file = log_dir / "events.log";
  {
    auto maybe_writer = EventLogWriter::fromPath(file);
    auto writer = FARM_UNWRAP(maybe_writer);
    core::proto::Timestamp x;
    for (int i = 0; i < 10; ++i) {
      x.set_stamp(i);
      writer.write("my_stamps", x);
    }
  }
  {
    auto maybe_reader = EventLogReader::fromPath(file);
    auto reader = FARM_UNWRAP(maybe_reader);

    int i = 0;
    while (true) {
      std::string payload;
      if (i == 10) {
        auto maybe_event = reader.readNextEvent();

        FARM_ASSERT(!maybe_event, "should be false, we are at the end");
        break;
      }
      auto maybe_pos = reader.readNextEvent(&payload);
      auto pos = FARM_UNWRAP(maybe_pos);
      core::proto::Timestamp x;
      EXPECT_EQ(true, x.ParseFromString(payload));
      EXPECT_EQ(i, x.stamp());
      EXPECT_EQ("protobuf", pos.event().uri().scheme());
      EXPECT_EQ("my_stamps", pos.event().uri().path());
      EXPECT_EQ(getHostName(), pos.event().uri().authority());
      EXPECT_EQ(
          "type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto",
          pos.event().uri().query());
      i++;
    }
  }
}

TEST(event_log, index) {  // NOLINT
  auto maybe_log_dir = createUniqueTemporaryDirectory();
  auto log_dir = FARM_UNWRAP(maybe_log_dir);
  auto file = log_dir / "events.log";
  {
    auto maybe_writer = EventLogWriter::fromPath(file);
    auto writer = FARM_UNWRAP(maybe_writer);
    core::proto::Timestamp x;
    for (int i = 0; i < 10; ++i) {
      x.set_stamp(i);
      writer.write("my_stamps", x);
    }
  }
  {
    auto maybe_reader = EventLogReader::fromPath(file);
    auto reader = FARM_UNWRAP(maybe_reader);
    int i = 0;
    auto index = reader.getIndex();
    for (EventLogPos& pos : index) {
      core::proto::Timestamp x;
      auto maybe_payload = pos.readPayload();
      auto payload = FARM_UNWRAP(maybe_payload);
      x.ParseFromString(payload);
      EXPECT_EQ(i, x.stamp());
      EXPECT_EQ("protobuf", pos.event().uri().scheme());
      EXPECT_EQ("my_stamps", pos.event().uri().path());
      EXPECT_EQ(getHostName(), pos.event().uri().authority());
      EXPECT_EQ(
          "type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto",
          pos.event().uri().query());
      ++i;
    }
  }
}

TEST(event_log, time_ordered_zip) {  // NOLINT
  auto maybe_log_dir = createUniqueTemporaryDirectory();
  auto log_dir = FARM_UNWRAP(maybe_log_dir);
  auto file1 = log_dir / "events1.log";
  auto file2 = log_dir / "events2.log";

  {
    auto maybe_writer1 = EventLogWriter::fromPath(file1);
    auto writer1 = FARM_UNWRAP(maybe_writer1);

    auto maybe_writer2 = EventLogWriter::fromPath(file2);
    auto writer2 = FARM_UNWRAP(maybe_writer2);

    core::proto::Timestamp x;
    for (int i = 0; i < 10; ++i) {
      x.set_stamp(i);
      writer1.write("my_stamps1", x);
      writer2.write("my_stamps2", x);
    }
  }
  {
    auto maybe_reader1 = EventLogReader::fromPath(file1);
    auto reader1 = FARM_UNWRAP(maybe_reader1);
    auto maybe_reader2 = EventLogReader::fromPath(file2);
    auto reader2 = FARM_UNWRAP(maybe_reader2);

    // Grab a reference write stamp from the first reader.
    auto maybe_ref = reader1.readNextEvent();
    auto ref = FARM_UNWRAP(maybe_ref).event().timestamps().Get(0);
    EXPECT_EQ(ref.semantics(), "log/write");
    std::vector<EventLogPos> index = eventLogTimeOrderedIndex(
        ref.clock_name(), ref.semantics(), {reader1, reader2});
    int i = 0;
    for (EventLogPos const& pos : index) {
      core::proto::Timestamp x;
      auto maybe_payload = pos.readPayload();
      x.ParseFromString(FARM_UNWRAP(maybe_payload));
      EXPECT_EQ(i / 2, x.stamp());
      EXPECT_EQ("protobuf", pos.event().uri().scheme());
      if (i % 2 == 0) {
        EXPECT_EQ("my_stamps1", pos.event().uri().path());
      } else {
        EXPECT_EQ("my_stamps2", pos.event().uri().path());
      }
      EXPECT_EQ(getHostName(), pos.event().uri().authority());
      EXPECT_EQ(
          "type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto",
          pos.event().uri().query());
      ++i;
    }
  }
}

TEST(event_log, post_processed_time_ordered_zip) {  // NOLINT
  auto maybe_log_dir = createUniqueTemporaryDirectory();
  auto log_dir = FARM_UNWRAP(maybe_log_dir);
  auto file1 = log_dir / "events1.log";
  auto file2 = log_dir / "events2.log";
  {
    auto maybe_writer = EventLogWriter::fromPath(file1);
    auto writer1 = FARM_UNWRAP(maybe_writer);
    core::proto::Timestamp x;
    for (int i = 0; i < 10; ++i) {
      x.set_stamp(i);
      writer1.write("my_stamps1", x);
    }
  }
  {
    auto maybe_reader1 = EventLogReader::fromPath(file1);
    auto reader1 = FARM_UNWRAP(maybe_reader1);

    auto maybe_writer2 = EventLogWriter::fromPath(file2);
    auto writer2 = FARM_UNWRAP(maybe_writer2);
    for (EventLogPos pos : reader1.getIndex()) {
      core::proto::Timestamp x;
      auto maybe_payload = pos.readPayload();
      x.ParseFromString(FARM_UNWRAP(maybe_payload));
      x.set_stamp(x.stamp() + 1.0);
      writer2.write("my_stamps2", x, pos.event().timestamps());
    }
  }

  {
    auto maybe_reader1 = EventLogReader::fromPath(file1);
    auto reader1 = FARM_UNWRAP(maybe_reader1);

    auto maybe_reader2 = EventLogReader::fromPath(file2);
    auto reader2 = FARM_UNWRAP(maybe_reader2);

    // Grab a reference write stamp from the first reader.
    auto maybe_ref = reader1.readNextEvent();
    auto ref = FARM_UNWRAP(maybe_ref).event().timestamps().Get(0);
    EXPECT_EQ(ref.semantics(), "log/write");
    std::vector<EventLogPos> index = eventLogTimeOrderedIndex(
        ref.clock_name(), ref.semantics(), {reader1, reader2});
    int i = 0;
    for (EventLogPos const& pos : index) {
      core::proto::Timestamp x;

      auto maybe_payload = pos.readPayload();
      x.ParseFromString(FARM_UNWRAP(maybe_payload));

      EXPECT_EQ("protobuf", pos.event().uri().scheme());
      if (i % 2 == 0) {
        EXPECT_EQ(i / 2, x.stamp());
        EXPECT_EQ("my_stamps1", pos.event().uri().path());
      } else {
        EXPECT_EQ(i / 2 + 1, x.stamp());
        EXPECT_EQ("my_stamps2", pos.event().uri().path());
      }
      EXPECT_EQ(getHostName(), pos.event().uri().authority());
      EXPECT_EQ(
          "type=farm_ng.core.proto.Timestamp&pb=farm_ng/core/timestamp.proto",
          pos.event().uri().query());
      ++i;
    }
  }
}
