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

#define FARM_CONV_IMPL_REPEATED_MESSAGE(Proto_Type_, Cpp_Type_Container_) \
  template <>                                                             \
  auto fromProt<Proto_Type_>(Proto_Type_ const& proto)                    \
      -> Expected<Cpp_Type_Container_> {                                  \
    Cpp_Type_Container_ v;                                                \
    for (int i = 0; i < proto.value_size(); ++i) {                        \
      FARM_TRY(auto, value, fromProt(proto.value(i)));                    \
      v.push_back(value);                                                 \
    }                                                                     \
    return v;                                                             \
  }                                                                       \
                                                                          \
  template <>                                                             \
  auto toProt<Cpp_Type_Container_>(Cpp_Type_Container_ const& v)          \
      -> Proto_Type_ {                                                    \
    Proto_Type_ proto;                                                    \
    for (auto const& value : v) {                                         \
      *proto.add_value() = toProt(value);                                 \
    }                                                                     \
    return proto;                                                         \
  }
