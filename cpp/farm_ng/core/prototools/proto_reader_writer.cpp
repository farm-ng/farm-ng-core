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

#include "farm_ng/core/prototools/proto_reader_writer.h"

namespace farm_ng {

Expected<farm_ng::Success> writeProtobufToJsonFile(
    std::filesystem::path const& path, google::protobuf::Message const& proto) {
  google::protobuf::util::JsonPrintOptions print_options;
  print_options.add_whitespace = true;
  print_options.always_print_primitive_fields = true;
  std::string json_str;
  google::protobuf::util::MessageToJsonString(proto, &json_str, print_options);
  std::ofstream outf(path.string());
  try {
    outf << json_str;
  } catch (std::exception& e) {
    return FARM_ERROR(
        "Failed to write proto to json file ({}). Error: {}",
        path.string(),
        e.what());
  }
  return Success{};
}

Expected<Success> writeProtobufToBinaryFile(
    std::filesystem::path const& path, google::protobuf::Message const& proto) {
  std::string binary_str;
  proto.SerializeToString(&binary_str);
  std::ofstream outf(path.string(), std::ofstream::binary);
  try {
    outf << binary_str;
  } catch (std::exception& e) {
    return FARM_ERROR(
        "Failed to write proto to json file ({}). Error: {}",
        path.string(),
        e.what());
  }
  return Success{};
}

Expected<std::string> readJsonStringFromJsonFile(
    std::filesystem::path const& path) {
  if (!std::filesystem::is_regular_file(path)) {
    return FARM_ERROR("{} is not a regular file", path);
  }

  std::ifstream json_in(path.string());
  std::string json_str(
      (std::istreambuf_iterator<char>(json_in)),
      std::istreambuf_iterator<char>());

  if (json_str.empty()) {
    return FARM_ERROR("Did not load any text from: {}", path);
  }
  return json_str;
}

Expected<std::string> readBytesFromBinaryFile(
    std::filesystem::path const& path) {
  std::ifstream bin_in(path.string(), std::ifstream::binary);
  if (!bin_in) {
    return FARM_ERROR("Could not open path: {}", path.string());
  }
  std::string bin_str(
      (std::istreambuf_iterator<char>(bin_in)),
      std::istreambuf_iterator<char>());

  if (bin_str.empty()) {
    return FARM_ERROR("Did not load any text from: {}", path.string());
  }

  return bin_str;
}

}  // namespace farm_ng
