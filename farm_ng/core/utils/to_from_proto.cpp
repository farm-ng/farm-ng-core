// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng/core/utils/to_from_proto.h"

#include "farm_ng/core/logging/logger.h"

namespace farm_ng_core {

Uri fromProto(const proto::Uri& proto) {
  UriSchemeType type;
  FARM_NG_CHECK(trySetFromString(type, proto.scheme()), "{}", proto.scheme());
  return Uri(type, proto.authority(), proto.path(), proto.query());
}

proto::Uri toProto(const Uri& uri) {
  proto::Uri proto;
  proto.set_scheme(toString(uri.scheme));
  proto.set_authority(uri.authority);
  proto.set_path(uri.path);
  proto.set_query(uri.query);
  return proto;
}

Timestamp fromProto(const proto::Timestamp& proto) {
  ClockType clock_type;
  FARM_NG_CHECK(trySetFromString(clock_type, proto.clock_type()));
  return Timestamp(proto.clock_name(), proto.seconds(), clock_type);
}

proto::Timestamp toProto(const Timestamp& t) {
  proto::Timestamp proto;
  proto.set_clock_name(t.clock_name);
  proto.set_seconds(t.seconds);
  proto.set_clock_type(toString(t.clock_type));
  return proto;
}

GeoSpherical fromProto(const proto::GeoSpherical& proto) {
  return GeoSpherical({proto.longitude_rad(), proto.latitude_rad()});
}

proto::GeoSpherical toProto(const GeoSpherical& lat_lon) {
  proto::GeoSpherical proto;
  proto.set_longitude_rad(lat_lon.longitude_rad);
  proto.set_latitude_rad(lat_lon.latitude_rad);
  return proto;
}

}  // namespace farm_ng_core
