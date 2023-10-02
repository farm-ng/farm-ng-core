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

#include "farm_ng/core/misc/shared.h"
#include "farm_ng/core/pipeline/component.h"
#include "farm_ng/core/plotting/remote_plotting_client.h"

#include <deque>

#pragma once

namespace farm_ng {

/// A plotting component sink which publishes messages to a remote plotting
/// service.
class PlottingComponent {
 public:
  /// Creates a plotting component.
  static Shared<PlottingComponent> create(
      Context const& ctx, RemotePlottingClient::Params const& params);
  virtual ~PlottingComponent() = default;

  /// Connect must be called before the component is run.
  virtual void connect() = 0;

  /// Returns the input channel for messages to be plotted.
  virtual Input<std::vector<plotting::Message>>& inMessages() = 0;
};

}  // namespace farm_ng
