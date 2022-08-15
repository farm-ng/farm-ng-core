// Copyright (c) farm-ng, inc. All rights reserved.

#pragma once

#include <filesystem>
#include <optional>

namespace farm_ng_core {

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
std::optional<std::filesystem::path> createVersionedFolder(
    const std::filesystem::path& target_path);

/// Creates a unique temporary directory with the prefix "/tmp/farm-ng-"
///
/// Returns nullopt if for unexpected reason creation of the folder fails.
/// Otherwise, it returns the path of the created directory.
std::optional<std::filesystem::path> createUniqueTemporaryDirectory();

}  // namespace farm_ng_core
