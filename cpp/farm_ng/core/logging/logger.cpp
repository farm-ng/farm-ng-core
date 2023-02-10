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

#include "farm_ng/core/logging/logger.h"

namespace farm_ng {

namespace details {
std::string stringFromLogLevel(LogLevel level) {
  switch (level) {
    case LogLevel::trace:
      return "TRACE";
    case LogLevel::debug:
      return "DEBUG";
    case LogLevel::info:
      return "INFO";
    case LogLevel::warning:
      return "WARN";
    case LogLevel::error:
      return "ERROR";
    case LogLevel::critical:
      return "CRITICAL";
    case LogLevel::off:
      return "OFF";
    default:
      return "";
  }
}
}  // namespace details

void StreamLogger::setHeaderFormat(std::string const& str) {
  header_format_ = str;
}
std::string StreamLogger::getHeaderFormat() const { return header_format_; }

void StreamLogger::setLogLevel(LogLevel level) { log_level_ = level; }

void StreamLogger::log(
    LogLevel log_level,
    std::string const& header,
    std::string const& file,
    int line,
    std::string const& function) {
  if (log_level_ <= log_level) {
    writeHeader(log_level, header, file, line, function);
    flush();
  }
}

void StreamLogger::writeHeader(
    LogLevel log_level,
    std::string const& header_text,
    std::string const& file,
    int line,
    std::string const& function) {
  write(FARM_FORMAT(
      header_format_,
      fmt::arg("level", details::stringFromLogLevel(log_level)),
      fmt::arg("text", header_text),
      fmt::arg("file", file),
      fmt::arg("line", line),
      fmt::arg("function", function)));
}
void StreamLogger::write(std::string const& str) { std::cerr << str; }
void StreamLogger::flush() { std::cerr << std::endl; }

}  // namespace farm_ng
