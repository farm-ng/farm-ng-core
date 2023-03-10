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

#include "farm_ng/core/misc/proto/conv.h"

#include "farm_ng/core/logging/logger.h"

namespace farm_ng {

auto fromProto(core::proto::Uri const& proto) -> Uri {
  return Uri(proto.scheme(), proto.authority(), proto.path(), proto.query());
}

auto toProto(Uri const& uri) -> core::proto::Uri {
  core::proto::Uri proto;
  proto.set_scheme(uri.scheme);
  proto.set_authority(uri.authority);
  proto.set_path(uri.path);
  proto.set_query(uri.query);
  return proto;
}

}  // namespace farm_ng
