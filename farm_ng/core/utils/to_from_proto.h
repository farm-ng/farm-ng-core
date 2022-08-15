// Copyright (c) farm-ng, inc. All rights reserved.

#pragma once

#include "farm_ng/core/utils/conversions.h"
#include "farm_ng/core/utils/time.h"
#include "farm_ng/core/utils/uri.h"
#include "farm_ng/core/utils/utils.pb.h"

namespace farm_ng_core {

/// Convert proto::Uri to Uri.
Uri fromProto(const proto::Uri& proto);
/// Convert Uri to proto::Uri.
proto::Uri toProto(const Uri& uri);

/// Convert proto::Timestamp to Timestamp.
Timestamp fromProto(const proto::Timestamp& proto);
/// Convert Timestamp to proto::Timestamp.
proto::Timestamp toProto(const Timestamp& t);

GeoSpherical fromProto(const proto::GeoSpherical& proto);
proto::GeoSpherical toProto(const GeoSpherical& lat_lon);

}  // namespace farm_ng_core
