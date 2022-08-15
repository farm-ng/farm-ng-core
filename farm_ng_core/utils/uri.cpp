// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng_core/utils/uri.h"

namespace farm_ng_core {

Uri::Uri(
    const UriSchemeType& scheme,
    const std::string& authority,
    const std::string& path,
    const std::string& query)
    : scheme(scheme), authority(authority), path(path), query(query) {}

std::string Uri::string() const {
  if (query.empty()) {
    return toString(scheme) + "://" + authority + "/" + path;
  }
  return toString(scheme) + "://" + authority + "/" + path + "?" + query;
}

}  // namespace farm_ng_core
