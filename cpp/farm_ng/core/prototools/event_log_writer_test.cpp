// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

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
    for (int i = 0; i < 10; ++i) {
      std::string payload;
      EventLogPos pos = reader.readNextEvent(&payload);
      core::proto::Timestamp x;
      EXPECT_EQ(true, x.ParseFromString(payload));
      EXPECT_EQ(i, x.stamp());
      EXPECT_EQ("protobuf", pos.event().uri().scheme());
      EXPECT_EQ("my_stamps", pos.event().uri().path());
      EXPECT_EQ(getHostName(), pos.event().uri().authority());
      EXPECT_EQ("type=farm_ng.core.proto.Timestamp", pos.event().uri().query());
    }
    EXPECT_THROW(reader.readNextEvent(), std::runtime_error);
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
      EXPECT_EQ("type=farm_ng.core.proto.Timestamp", pos.event().uri().query());
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
        reader1.readNextEvent().event().timestamps()[0];
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
      EXPECT_EQ("type=farm_ng.core.proto.Timestamp", pos.event().uri().query());
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
        reader1.readNextEvent().event().timestamps()[0];
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
      EXPECT_EQ("type=farm_ng.core.proto.Timestamp", pos.event().uri().query());
      ++i;
    }
  }
}
