
// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "farm_ng/core/misc/to_from_proto.h"
#include "farm_ng/core/protobuf/proto_reader_writer.h"

using namespace farm_ng;

int main() {
  Uri uri;
  uri.path = "foo";

  auto proto = toProto(uri);

  writeProtobufToJsonFile("/tmp/foo.json", proto);
}
