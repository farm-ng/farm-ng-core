// Copyright (c) farm-ng, inc. All rights reserved.

#pragma once

#include <my_enum/EnumO.h>

namespace farm_ng_core {

/// Fix list of uri-schemes used in the codebase.
/// file:    the uri-path is a file on disk
/// device:  a hardware device such as sensor on the robot, or canbus, or the
///          robot itself, or a simulated device, or ...
/// other:   other category, e.g. for tests.
MY_ENUM_O(UriSchemeType, (file, device, other));

/// https://en.wikipedia.org/w/index.php?title=Uniform_Resource_Identifier&oldid=1072892451#Syntax
struct Uri {
  Uri() {}

  /// Creates Uri.
  Uri(const UriSchemeType& scheme,
      const std::string& authority,
      const std::string& path,
      const std::string& query = {});

  /// Returns string representation of uri.
  std::string string() const;

  /// The uri scheme, e.g. UriSchemeType::device.
  UriSchemeType scheme;

  /// If scheme is device, then authority is `[robot_name]`.
  std::string authority;

  /// if scheme is device, then path is path to underlying sensor such as
  /// `sensor_rig_name/camera_stream_name`.
  std::string path;

  /// If URI is used in component, then query is used to denote inputs or
  /// outputs.
  /// For instance: '?in=input_channel_name`.
  std::string query;

  friend inline bool operator<(const Uri& lhs, const Uri& rhs) {
    return lhs.string() < rhs.string();
  }
};
}  // namespace farm_ng_core
