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
std::string getHostName() {
  char hostname[_POSIX_HOST_NAME_MAX];
  gethostname(hostname, _POSIX_HOST_NAME_MAX);
  return std::string(hostname);
}
}  // namespace
TEST(event_log, no_file) {
  EXPECT_THROW(EventLogReader("file_does_not_exist"), EventLogExist);
  EXPECT_THROW(
      EventLogWriter("/no/perms/file_does_not_exist"),
      std::filesystem::filesystem_error);
  { EventLogWriter writer_success("relative_event.log"); }
  EXPECT_TRUE(std::filesystem::exists("relative_event.log"));
  std::filesystem::remove("relative_event.log");
  auto maybe_log_dir = createUniqueTemporaryDirectory();

  EventLogWriter writer_success2(
      FARM_UNWRAP(maybe_log_dir) / "tmplocal_file.log");
}
TEST(event_log, roundtrip) {
  auto maybe_log_dir = createUniqueTemporaryDirectory();
  auto log_dir = FARM_UNWRAP(maybe_log_dir);
  auto file = log_dir / "events.log";
  {
    EventLogWriter writer(file);
    core::proto::Timestamp x;
    for (int i = 0; i < 10; ++i) {
      x.set_stamp(i);
      writer.write("my_stamps", x);
    }
  }
  {
    EventLogReader reader(file);
    int i = 0;
    while (true) {
      std::string payload;
      if (i == 10) {
        EXPECT_THROW(reader.readNextEvent(), EventLogEof);
        break;
      }
      EventLogPos pos = reader.readNextEvent(&payload);
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

TEST(event_log, index) {
  auto maybe_log_dir = createUniqueTemporaryDirectory();
  auto log_dir = FARM_UNWRAP(maybe_log_dir);
  auto file = log_dir / "events.log";
  {
    EventLogWriter writer(file);
    core::proto::Timestamp x;
    for (int i = 0; i < 10; ++i) {
      x.set_stamp(i);
      writer.write("my_stamps", x);
    }
  }
  {
    EventLogReader reader(file);
    int i = 0;
    for (EventLogPos const& pos : reader.getIndex()) {
      core::proto::Timestamp x;
      x.ParseFromString(pos.readPayload());
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

TEST(event_log, time_ordered_zip) {
  auto maybe_log_dir = createUniqueTemporaryDirectory();
  auto log_dir = FARM_UNWRAP(maybe_log_dir);
  auto file1 = log_dir / "events1.log";
  auto file2 = log_dir / "events2.log";

  {
    EventLogWriter writer1(file1);
    EventLogWriter writer2(file2);
    core::proto::Timestamp x;
    for (int i = 0; i < 10; ++i) {
      x.set_stamp(i);
      writer1.write("my_stamps1", x);
      writer2.write("my_stamps2", x);
    }
  }
  {
    EventLogReader reader1(file1);
    EventLogReader reader2(file2);
    // Grab a reference write stamp from the first reader.
    core::proto::Timestamp ref =
        reader1.readNextEvent().event().timestamps().Get(0);
    EXPECT_EQ(ref.semantics(), "log/write");
    std::vector<EventLogPos> index = eventLogTimeOrderedIndex(
        ref.clock_name(), ref.semantics(), {reader1, reader2});
    int i = 0;
    for (EventLogPos const& pos : index) {
      core::proto::Timestamp x;
      x.ParseFromString(pos.readPayload());
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

TEST(event_log, post_processed_time_ordered_zip) {
  auto maybe_log_dir = createUniqueTemporaryDirectory();
  auto log_dir = FARM_UNWRAP(maybe_log_dir);
  auto file1 = log_dir / "events1.log";
  auto file2 = log_dir / "events2.log";
  {
    EventLogWriter writer1(file1);
    core::proto::Timestamp x;
    for (int i = 0; i < 10; ++i) {
      x.set_stamp(i);
      writer1.write("my_stamps1", x);
    }
  }
  {
    EventLogReader reader1(file1);

    EventLogWriter writer2(file2);
    for (EventLogPos pos : reader1.getIndex()) {
      core::proto::Timestamp x;
      x.ParseFromString(pos.readPayload());
      x.set_stamp(x.stamp() + 1.0);
      writer2.write("my_stamps2", x, pos.event().timestamps());
    }
  }

  {
    EventLogReader reader1(file1);
    EventLogReader reader2(file2);
    // Grab a reference write stamp from the first reader.
    core::proto::Timestamp ref =
        reader1.readNextEvent().event().timestamps().Get(0);
    EXPECT_EQ(ref.semantics(), "log/write");
    std::vector<EventLogPos> index = eventLogTimeOrderedIndex(
        ref.clock_name(), ref.semantics(), {reader1, reader2});
    int i = 0;
    for (EventLogPos const& pos : index) {
      core::proto::Timestamp x;
      x.ParseFromString(pos.readPayload());

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
