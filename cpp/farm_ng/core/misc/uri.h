// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "farm_ng/core/enum/enum.h"

namespace farm_ng {

/// https://en.wikipedia.org/w/index.php?title=Uniform_Resource_Identifier&oldid=1072892451#Syntax
struct Uri {
  Uri() {}

  /// Creates Uri.
  Uri(std::string const& scheme,
      std::string const& authority,
      std::string const& path,
      std::string const& query = "")
      : scheme(scheme), authority(authority), path(path), query(query) {}

  /// Returns string representation of uri.
  [[nodiscard]] std::string string() const {
    if (query.empty()) {
      return scheme + "://" + authority + "/" + path;
    }
    return scheme + "://" + authority + "/" + path + "?" + query;
  }
  /// The uri scheme.
  std::string scheme;

  /// If scheme is device, then authority is `[robot_name]`.
  std::string authority;

  /// if scheme is device, then path is path to underlying sensor such as
  /// `sensor_rig_name/camera_stream_name`.
  std::string path;

  /// If URI is used in component, then query is used to denote inputs or
  /// outputs.
  /// For instance: '?in=input_channel_name`.
  std::string query;

  friend inline bool operator<(Uri const& lhs, Uri const& rhs) {
    return lhs.string() < rhs.string();
  }
};
}  // namespace farm_ng
