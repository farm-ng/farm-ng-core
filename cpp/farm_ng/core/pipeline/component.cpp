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

// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng/core/pipeline/component.h"

#include <cxxabi.h>
namespace farm_ng {

std::string demangleTypeid(std::type_info const& type) {
  std::string demangled_type;
  char* c_demangled = nullptr;
  int status = 0;
  c_demangled = abi::__cxa_demangle(type.name(), nullptr, nullptr, &status);
  FARM_ASSERT_EQ(status, 0);
  FARM_ASSERT(c_demangled);
  demangled_type = c_demangled;
  free(c_demangled);  // NOLINT(cppcoreguidelines-no-malloc)
  return demangled_type;
}

Context Component::getContext() const { return context_strand_.getContext(); };

ContextStrand Component::getContextStrand() const { return context_strand_; };

Uri const& Component::uri() const { return uri_; }

}  // namespace farm_ng
