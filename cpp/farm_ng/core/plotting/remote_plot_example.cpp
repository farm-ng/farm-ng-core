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

#include "farm_ng/core/plotting/plotting_component.h"

#include <farm_ng/core/pipeline/thread_pool.h>

using namespace farm_ng;

int main(int argc, char* argv[]) {
  farm_ng::Context context;
  auto plotting = PlottingComponent::create(context, {});
  plotting->connect();

  std::shared_ptr<ThreadPool> pool =
      std::make_shared<ThreadPool>(context.getAsioIoContextPtr());

  pool->start(8);

  plotting::Vec3Curve trig_graph;
  trig_graph.bounds = {
      .x_bounds = {.len = 2.5 * M_PI},
      .y_bounds = {.height = 2.0, .offset = -0.8}};
  trig_graph.path = "trig/ {sin,cos,tan}";
  trig_graph.color = {
      sophus::Color::neonRed(),
      sophus::Color::neonGreen(),
      sophus::Color::neonBlue()};

  plotting::Curve sawtooth;
  sawtooth.bounds = {
      .x_bounds = {.len = 2.5 * M_PI},
      .y_bounds = {.height = 2.0, .offset = 0.0}};
  sawtooth.path = "trig1/sawtooth";
  sawtooth.color = sophus::Color::blue();

  plotting::Vec3CurveWithConfInterval conf_trig_graph;
  conf_trig_graph.bounds = {
      .x_bounds = {.len = 2.5 * M_PI},
      .y_bounds = {.height = 2.5, .offset = 0.0}};
  conf_trig_graph.path = "trig3/ {sin,cos,tan}_conf";
  conf_trig_graph.color = {
      sophus::Color::neonRed(),
      sophus::Color::neonGreen(),
      sophus::Color::neonBlue()};
  conf_trig_graph.conf_color = {
      sophus::Color::darkRed(),
      sophus::Color::darkGreen(),
      sophus::Color::darkBlue()};

  plotting::RectPlot timestamps;
  timestamps.path = "t/timestamps";

  double x = 0.0;

  std::vector<plotting::Message> messages;

  plotting->inMessages().send(messages);
  int counter = 0;
  while (true) {
    double sin_x = std::sin(x);
    double cos_x = std::cos(x);
    double tan_x = std::clamp(std::tan(x), -1.0, 1.0);
    double conf = 0.1 * std::abs(sin_x);

    double sawtooth_x = x - std::floor(x / (2.0 * M_PI)) * (2.0 * M_PI);

    trig_graph.x_vec_pairs.clear();
    sawtooth.x_y_pairs.clear();
    conf_trig_graph.x_vec_conf_tuples.clear();

    trig_graph.x_vec_pairs.push_back(Eigen::Vector4d(x, sin_x, cos_x, tan_x));

    Eigen::Vector<double, 7> vec;

    vec << x, sin_x, cos_x, tan_x, conf, conf, conf;

    conf_trig_graph.x_vec_conf_tuples.push_back(vec);

    sawtooth.x_y_pairs.push_back({x, sawtooth_x});

    messages.clear();

    messages.push_back(trig_graph);
    messages.push_back(sawtooth);
    messages.push_back(conf_trig_graph);

    ++counter;

    plotting->inMessages().send(messages);

    x += 0.01;
    trig_graph.reset.clear_x_smaller_than = x - 2.0 * M_PI;
    sawtooth.reset.clear_x_smaller_than = x - 2.0 * M_PI;
    conf_trig_graph.reset.clear_x_smaller_than = x - 2.0 * M_PI;

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }

  pool->join();

  return 0;
}
