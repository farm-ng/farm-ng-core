// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <fng_core/logging/format.h>

#include <filesystem>
#include <optional>

namespace fng_core {

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
    const std::filesystem::path& target_path) {
  /* the following removes trailing '/', only if it is there. */
  std::filesystem::path clean_path =
      (std::filesystem::path(target_path) / "foo").parent_path();

  if (!std::filesystem::exists(clean_path.parent_path())) {
    return std::nullopt;
  }

  int count = 0;
  std::filesystem::path path_attempt;
  do {
    path_attempt = FNG_FORMAT("{}.{:02d}", clean_path.string(), count++);
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

}  // namespace fng_core
