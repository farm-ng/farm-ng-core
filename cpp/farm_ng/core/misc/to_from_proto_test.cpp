// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "farm_ng/core/misc/to_from_proto.h"

#include <farm_ng/core/logging/logger.h>
#include <gtest/gtest.h>

using namespace farm_ng;

TEST(to_from_proto, uri) {
  Uri uri(UriSchemeType::other, "[authority]", "foo/bar", "in=input");

  Uri to_from_uri = fromProto(toProto(uri));

  FARM_CHECK_EQ(uri.string(), "other://[authority]/foo/bar?in=input");
  FARM_CHECK_EQ(uri.string(), to_from_uri.string());
}
