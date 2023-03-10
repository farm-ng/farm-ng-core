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

#include "farm_ng/core/misc/filesystem.h"

#include <gtest/gtest.h>

using namespace farm_ng;

TEST(filesystem, smoke) {  // NOLINT
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

TEST(filesystem, createUniqueTemporaryDirectory) {  // NOLINT
  auto output_folder = createUniqueTemporaryDirectory();
  ASSERT_TRUE(output_folder);
  std::string prefix = "/tmp/farm-ng-";
  ASSERT_EQ(output_folder->native().substr(0, prefix.size()), prefix);
  ASSERT_TRUE(std::filesystem::exists(output_folder.value()));
}

TEST(filesystem, no_parent_path) {  // NOLINT
  std::filesystem::path tmp_path = std::string("/tmp/tmp");
  auto output_folder = createVersionedFolder(tmp_path / "foo/bar");
  ASSERT_FALSE(output_folder);
}
