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

template <class TProtobufT>
Expected<TProtobufT> readProtobufFromJsonFile(
    std::filesystem::path const& path) {
  FARM_TRY_ASSIGN(std::string, json_string, readJsonStringFromJsonFile(path));
  TProtobufT message;
  google::protobuf::util::JsonParseOptions parse_options;  // default
  auto status = google::protobuf::util::JsonStringToMessage(
      json_string, &message, parse_options);
  if (!status.ok()) {
    return FARM_UNEXPECTED("Failed to parse json string: {}", status);
  }
  return message;
}

Expected<std::string> readBytesFromBinaryFile(
    std::filesystem::path const& path);

template <class TProtobufT>
Expected<TProtobufT> readProtobufFromBinaryFile(
    std::filesystem::path const& path) {
  TProtobufT message;
  FARM_TRY_ASSIGN(std::string, bytes, readBytesFromBinaryFile(path));
  if (message.ParseFromString(bytes)) {
    return FARM_UNEXPECTED("Failed to parse {}", path.string());
  }
  return message;
}

}  // namespace farm_ng
