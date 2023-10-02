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

#pragma once

#include "farm_ng/core/enum/enum.h"

namespace farm_ng {

/// Uri is a Uniform Resource Identifier.
///
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

  /// Lexicographical comparison of uri.
  friend inline bool operator<(Uri const& lhs, Uri const& rhs) {
    return lhs.string() < rhs.string();
  }
};
}  // namespace farm_ng
