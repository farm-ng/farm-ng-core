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

#include <farm_ng/core/event.pb.h>
#include <farm_ng/core/misc/shared.h>

#include <exception>
#include <filesystem>
#include <memory>
#include <string>

namespace farm_ng {
class EventAndPayload {
 public:
  EventAndPayload();
  virtual ~EventAndPayload();

  virtual core::proto::Event const& event() const = 0;
  virtual void stamp(core::proto::Timestamp const& stamp) const = 0;
  virtual std::string const& readPayload() const = 0;
  static Shared<EventAndPayload> make(
      core::proto::Event const& event, std::string const& payload);

  static Shared<EventAndPayload> make(
      std::string const& path,
      google::protobuf::Message const& message,
      google::protobuf::RepeatedPtrField<core::proto::Timestamp> const&
          timestamps);

  static Shared<EventAndPayload> make(
      std::string const& path,
      google::protobuf::Message const& message,
      std::vector<core::proto::Timestamp> const& timestamps =
          std::vector<core::proto::Timestamp>());
};

typedef Shared<EventAndPayload> SharedEventAndPayload;

std::string const& getAuthority();
core::proto::Timestamp makeWriteStamp();
core::proto::Timestamp makeRecvStamp();
core::proto::Timestamp makeSendStamp();

std::string uriString(core::proto::Uri const& uri);

}  // namespace farm_ng
