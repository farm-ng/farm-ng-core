// Copyright (c) farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "farm_ng/core/logging/expected.h"
#include "farm_ng/core/logging/logger.h"

#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>

#include <filesystem>
#include <fstream>

namespace farm_ng {

/// TODO: Add API docs.

Expected<Success> writeProtobufToJsonFile(
    std::filesystem::path const& path, google::protobuf::Message const& proto);

Expected<Success> writeProtobufToBinaryFile(
    std::filesystem::path const& path, google::protobuf::Message const& proto);

Expected<std::string> readJsonStringFromJsonFile(
    std::filesystem::path const& path);

template <class ProtobufT>
Expected<ProtobufT> readProtobufFromJsonFile(
    std::filesystem::path const& path) {
  FARM_TRY(std::string json_string, readJsonStringFromJsonFile(path));
  ProtobufT message;
  google::protobuf::util::JsonOptions print_options;
  print_options.add_whitespace = true;
  print_options.always_print_primitive_fields = true;
  auto status = google::protobuf::util::JsonStringToMessage(
      json_string, &message, print_options);
  if (!status) {
    return FARM_ERROR("Failed to parse json string: {}", status);
  }
  return message;
}

Expected<std::string> readBytesFromBinaryFile(
    std::filesystem::path const& path);

template <class ProtobufT>
Expected<ProtobufT> readProtobufFromBinaryFile(
    std::filesystem::path const& path) {
  ProtobufT message;
  FARM_TRY(std::string bytes, readBytesFromBinaryFile(path));
  if (message.ParseFromString(bytes)) {
    return FARM_ERROR("Failed to parse {}", path.string());
  }
  return message;
}

}  // namespace farm_ng
