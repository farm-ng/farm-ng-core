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
#include "farm_ng/core/misc/uri.h"

#include <farm_ng/core/uri.pb.h>

namespace farm_ng {

/// Convert proto::Uri to Uri.
Uri fromProto(farm_ng::core::proto::Uri const& proto);
/// Convert Uri to proto::Uri.
farm_ng::core::proto::Uri toProto(Uri const& uri);

}  // namespace farm_ng
