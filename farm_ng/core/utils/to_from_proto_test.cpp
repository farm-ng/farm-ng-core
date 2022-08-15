// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng/core/utils/to_from_proto.h"

#include "farm_ng/core/logging/logger.h"

#include <gtest/gtest.h>

using namespace farm_ng_core;

TEST(to_from_proto, uri) {
  Uri uri(UriSchemeType::other, "[authority]", "foo/bar", "in=input");

  Uri to_from_uri = fromProto(toProto(uri));

  FARM_NG_CHECK_EQ(uri.string(), "other://[authority]/foo/bar?in=input");
  FARM_NG_CHECK_EQ(uri.string(), to_from_uri.string());
}
