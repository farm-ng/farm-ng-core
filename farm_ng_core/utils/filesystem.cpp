// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng_core/utils/filesystem.h"

#include "farm_ng_core/logging/format.h"

namespace farm_ng_core {

std::optional<std::filesystem::path> createVersionedFolder(
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

std::optional<std::filesystem::path> createUniqueTemporaryDirectory() {
  char template_str[] = "/tmp/farm-ng-XXXXXX";
  char* directory_str = mkdtemp(template_str);
  if (directory_str == nullptr) {
    return std::nullopt;
  }
  return directory_str;
}

}  // namespace farm_ng_core
