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

#include <Eigen/Dense>
#include <fmt/ostream.h>

template <typename T>
    requires std::is_base_of_v<Eigen::DenseBase<T>, T>
struct fmt::formatter<T> : ostream_formatter {};

template <typename T, int N>
struct fmt::formatter<Eigen::VectorBlock<T, N>> : ostream_formatter {};