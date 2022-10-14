// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "farm_ng/core/logging/expected.h"
#include "farm_ng/core/misc/uri.h"
#include "farm_ng/core/uri.pb.h"

namespace farm_ng {

/// Convert proto::Uri to Uri.
Uri fromProto(farm_ng::core::proto::Uri const& proto);
/// Convert Uri to proto::Uri.
farm_ng::core::proto::Uri toProto(Uri const& uri);

}  // namespace farm_ng
