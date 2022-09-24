// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "farm_ng/core/misc/proto/conv.h"

#include "farm_ng/core/logging/logger.h"

namespace farm_ng {

Expected<Uri> fromProto(proto::Uri const& proto) {
  UriSchemeType type;
  if (!trySetFromString(type, proto.scheme())) {
    return FARM_ERROR("{}", proto.scheme());
  }
  return Uri(type, proto.authority(), proto.path(), proto.query());
}

proto::Uri toProto(Uri const& uri) {
  proto::Uri proto;
  proto.set_scheme(toString(uri.scheme));
  proto.set_authority(uri.authority);
  proto.set_path(uri.path);
  proto.set_query(uri.query);
  return proto;
}

}  // namespace farm_ng
