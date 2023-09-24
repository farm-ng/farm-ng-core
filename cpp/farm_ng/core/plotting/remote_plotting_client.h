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

#include "farm_ng/core/misc/shared.h"
#include "farm_ng/core/pipeline/component.h"
#include "farm_ng/core/plotting/types.h"
#include "sophus/calculus/region.h"

#pragma once

namespace farm_ng {

static uint64_t const kPlottingComponentDefaultPort = 1980;

class RemotePlottingClient {
 public:
  struct Params {
    std::string host = "localhost";
    uint32_t port = kPlottingComponentDefaultPort;
    double rate_limit_hz = 2.0;
  };

  virtual ~RemotePlottingClient() = default;

  static Shared<RemotePlottingClient> createAndConnect(Params const& params);

  virtual void onMessages(std::vector<plotting::Message> const& messages) = 0;
};

}  // namespace farm_ng
