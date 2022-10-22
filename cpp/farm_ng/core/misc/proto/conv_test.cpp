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

#include "farm_ng/core/misc/proto/conv.h"

#include <farm_ng/core/logging/logger.h>
#include <gtest/gtest.h>

using namespace farm_ng;

TEST(to_from_proto, uri) {
  Uri uri("other", "[authority]", "foo/bar", "in=input");

  Expected<Uri> maybe_to_from_uri = fromProto(toProto(uri));
  auto to_from_uri = FARM_UNWRAP(maybe_to_from_uri);

  FARM_CHECK_EQ(uri.string(), "other://[authority]/foo/bar?in=input");
  FARM_CHECK_EQ(uri.string(), to_from_uri.string());
}
