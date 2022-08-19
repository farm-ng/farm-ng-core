// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <fng_core/utils/filesystem.h>
#include <gtest/gtest.h>

using namespace fng_core;

TEST(filesystem, smoke) {
  std::filesystem::path tmp_path = std::string("/tmp");
  std::filesystem::remove_all(tmp_path / "foo.00");
  std::filesystem::remove_all(tmp_path / "foo.01");
  std::filesystem::remove_all(tmp_path / "foo.02");

  std::optional<std::filesystem::path> output_folder;

  output_folder = createVersionedFolder(tmp_path / "foo/");
  ASSERT_TRUE(output_folder);
  ASSERT_EQ(*output_folder, "/tmp/foo.00");

  output_folder = createVersionedFolder(tmp_path / "foo");
  ASSERT_TRUE(output_folder);
  ASSERT_EQ(*output_folder, "/tmp/foo.01");

  output_folder = createVersionedFolder(tmp_path / "foo/");
  ASSERT_TRUE(output_folder);
  ASSERT_EQ(*output_folder, "/tmp/foo.02");

  // TODO: create a failing test. This throws an error
  // output_folder = createVersionedFolder(tmp_path / "foo/bar");
  // ASSERT_FALSE(output_folder);
}

TEST(filesystem, createUniqueTemporaryDirectory) {
  auto output_folder = createUniqueTemporaryDirectory();
  ASSERT_TRUE(output_folder);
  std::string prefix = "/tmp/farm-ng-";
  ASSERT_EQ(output_folder->native().substr(0, prefix.size()), prefix);
  ASSERT_TRUE(std::filesystem::exists(output_folder.value()));
}

TEST(filesystem, no_parent_path) {
  std::filesystem::path tmp_path = std::string("/tmp/tmp");
  auto output_folder = createVersionedFolder(tmp_path / "foo/bar");
  ASSERT_FALSE(output_folder);
}
