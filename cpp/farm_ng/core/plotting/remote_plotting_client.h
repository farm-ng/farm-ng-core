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
#include "sophus2/calculus/region.h"

#pragma once

namespace farm_ng {

static uint64_t const kPlottingComponentDefaultPort = 1980;

/// A plotting component that can be used to plot curves and images.
class RemotePlottingClient {
 public:
  /// Parameters for the plotting component.
  struct Params {
    int grpc_max_reconnect_backoff_ms = 500;
    /// The host of the plotting service.
    std::string host = "localhost";
    /// The port of the plotting service.
    uint32_t port = kPlottingComponentDefaultPort;
    /// The rate limit in Hz.
    double rate_limit_hz = 2.0;
  };

  virtual ~RemotePlottingClient() = default;

  /// Creates a plotting component.
  static Shared<RemotePlottingClient> createAndConnect(Params const& params);

  /// Returns the input channel for messages to be plotted.
  virtual void onMessages(std::vector<plotting::Message> const& messages) = 0;
};

}  // namespace farm_ng
