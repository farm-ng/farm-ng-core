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

#include "farm_ng/core/logging/format.h"

#include <unistd.h>

#include <filesystem>
#include <optional>

namespace farm_ng {

/// Given a `target_path` name "foo", this function will create a version folder
/// "foo.xy".
///
/// First, it attempt to create folder "foo.00".
/// If the path "foo.00" already exits, it attempts to create "foo.01", etc.
///
/// Precondition: `parent_path` of the versioned folder must exist
///
/// Returns nullopt if for unexpected reason creation of the folder fails.
/// Otherwise, it returns the path of the created directory.
inline std::optional<std::filesystem::path> createVersionedFolder(
    std::filesystem::path const& target_path) {
  /* the following removes trailing '/', only if it is there. */
  std::filesystem::path clean_path =
      (std::filesystem::path(target_path) / "foo").parent_path();

  if (!std::filesystem::exists(clean_path.parent_path())) {
    return std::nullopt;
  }

  int count = 0;
  std::filesystem::path path_attempt;
  do {
    path_attempt = FARM_FORMAT("{}.{:02d}", clean_path.string(), count++);
  } while (std::filesystem::exists(path_attempt));
  if (!std::filesystem::create_directory(path_attempt)) {
    return std::nullopt;
  }
  return path_attempt;
}

/// Creates a unique temporary directory with the prefix "/tmp/farm-ng-"
///
/// Returns nullopt if for unexpected reason creation of the folder fails.
/// Otherwise, it returns the path of the created directory.
inline std::optional<std::filesystem::path> createUniqueTemporaryDirectory() {
  char template_str[] = "/tmp/farm-ng-XXXXXX";
  char* directory_str = mkdtemp(template_str);
  if (directory_str == nullptr) {
    return std::nullopt;
  }
  return directory_str;
}

}  // namespace farm_ng
