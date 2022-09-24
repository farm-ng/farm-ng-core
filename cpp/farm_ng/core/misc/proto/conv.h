// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "farm_ng/core/logging/expected.h"
#include "farm_ng/core/misc/uri.h"
#include "protos/farm_ng/core.pb.h"

namespace farm_ng {

/// Convert proto::Uri to Uri.
Expected<Uri> fromProto(proto::Uri const& proto);
/// Convert Uri to proto::Uri.
proto::Uri toProto(Uri const& uri);

}  // namespace farm_ng
