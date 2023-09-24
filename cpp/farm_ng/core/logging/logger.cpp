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

#include <fmt/chrono.h>

namespace farm_ng {

auto stringFromLogLevel(LogLevel level) -> std::string {
  auto str = toString(level);
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  return str;
}

void StreamLogger::setHeaderFormat(std::string const& str) {
  header_format_ = str;
}
auto StreamLogger::getHeaderFormat() const -> std::string {
  return header_format_;
}

void StreamLogger::setLogLevel(LogLevel level) { log_level_ = level; }
auto StreamLogger::getLogLevel() -> LogLevel { return log_level_; }

void StreamLogger::setLogClock(LogClock log_clock) { log_clock_ = log_clock; }

void StreamLogger::writeHeader(
    LogLevel log_level,
    std::string const& header_text,
    std::string const& file,
    int line,
    std::string const& function) {
  write(
      disk_logging_,
      FARM_FORMAT(
          header_format_,
          fmt::arg("time", log_clock_.now()),
          fmt::arg("level", stringFromLogLevel(log_level)),
          fmt::arg("text", header_text),
          fmt::arg("file", file),
          fmt::arg("line", line),
          fmt::arg("function", function)));
}

bool StreamLogger::trySetLogDir(std::filesystem::path const& path) noexcept {
  if (!std::filesystem::exists(path)) {
    FARM_WARN(
        "Failed to enable text logging to disk. The path `{}` does not exist.",
        path);
    return false;
  }
  if (!std::filesystem::is_directory(path)) {
    FARM_WARN(
        "Failed to enable text logging to disk. The path `{}` is not a "
        "directory.",
        path);
    return false;
  }
  this->disk_logging_.log_dir = path;

  std::filesystem::path log_file_path = path / "text.log";

  this->disk_logging_.log_file_stream = std::ofstream(log_file_path.string());

  if (!this->disk_logging_.log_file_stream) {
    FARM_WARN(
        "Failed to enable text logging to disk. Unable to open stream for "
        "`{}`.",
        log_file_path);
    return false;
  }

  return true;
}

// Get the directory where log files are written to. Returns nullopt if no
// directory has been set.
std::optional<std::filesystem::path> StreamLogger::getLogDir() const noexcept {
  return disk_logging_.log_dir;
}

void StreamLogger::write(DiskLogging& disk_logging, std::string const& str) {
  std::cerr << str;

  if (disk_logging.log_dir) {
    // Note: "log_file_stream_ << str; would not work since writing to streams
    //        other than cout and cerr is not thread safe.
    std::scoped_lock lock(disk_logging.log_file_mutex);

    fmt::print(disk_logging.log_file_stream, "{}", str);
  }
}

void StreamLogger::flush(DiskLogging& disk_logging) {
  std::cerr << std::endl;
  if (disk_logging.log_dir) {
    std::scoped_lock lock(disk_logging.log_file_mutex);
    // Note: log_file_stream.flush is not threadsafe.
    fmt::print(disk_logging.log_file_stream, "\n");
    // Flush here or not?
    disk_logging.log_file_stream.flush();
  }
}

std::string const StreamLogger::kDefaultHeaderFormat =
    "[FARM {text} in {file}:{line}]\n";

StreamLogger::LogClock const StreamLogger::kDefaultLogClock =
    StreamLogger::LogClock{.now = []() {
      auto now = std::chrono::system_clock::now();
      auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(
                        now.time_since_epoch())
                        .count() %
                    1000;
      return FARM_FORMAT("{}.{:03}", now, millis);
    }};

}  // namespace farm_ng
