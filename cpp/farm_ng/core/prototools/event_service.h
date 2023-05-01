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

#include <farm_ng/core/misc/shared.h>
#include <farm_ng/core/pipeline/component.h>
#include <farm_ng/core/pipeline/input.h>
#include <farm_ng/core/pipeline/output.h>
#include <farm_ng/core/prototools/event_and_payload.h>

#include <filesystem>
#include <memory>
#include <string>

namespace farm_ng {

class EventServiceClient : public Component {
 public:
  EventServiceClient(Context const &ctx, std::string const &path);
  virtual ~EventServiceClient();
  virtual Input<SharedEventAndPayload> &inEvents() = 0;
  virtual Output<SharedEventAndPayload> &outEvents() = 0;
  static Shared<EventServiceClient> create(
      Context const &ctx,
      std::string const &path,
      std::string const &server_address);
};

}  // namespace farm_ng
