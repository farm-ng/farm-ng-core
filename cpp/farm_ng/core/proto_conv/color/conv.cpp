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
#include "farm_ng/core/proto_conv/color/conv.h"

namespace farm_ng {

template <>
auto fromProt<core::proto::Color>(core::proto::Color const& proto)
    -> Expected<sophus::Color> {
  return sophus::Color(proto.r(), proto.g(), proto.b(), proto.a());
}

template <>
auto toProt<sophus::Color>(sophus::Color const& c) -> core::proto::Color {
  core::proto::Color proto;
  proto.set_r(c.r);
  proto.set_g(c.g);
  proto.set_b(c.b);
  proto.set_a(c.a);
  return proto;
}

}  // namespace farm_ng
