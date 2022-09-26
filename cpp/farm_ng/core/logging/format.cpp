#include <farm_ng/core/logging/format.h>

namespace farm_ng {

std::function<void(std::string const &)> &getLogLineFunction() {
  static std::function<void(std::string const &)> Static_Log_Function;
  return Static_Log_Function;
}

void setLogLineFunction(std::function<void(std::string const &)> f) {
  getLogLineFunction() = f;
}

void logLine(std::string const &line) {
  std::cerr << line << std::endl;
  if (getLogLineFunction()) {
    getLogLineFunction()(line);
  }
}

}  // namespace farm_ng