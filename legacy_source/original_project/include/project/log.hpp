#pragma once
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace neon {
enum class LogLevel { kInfo, kWarn, kError };

inline std::string now_iso8601() {
  using namespace std::chrono;
  auto t = system_clock::now();
  std::time_t tt = system_clock::to_time_t(t);
  std::tm tm;
#ifdef _WIN32
  localtime_s(&tm, &tt);
#else
  localtime_r(&tt, &tm);
#endif
  std::ostringstream oss;
  oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
  return oss.str();
}

inline void log(LogLevel lvl, const std::string& msg) {
  const char* tag = (lvl==LogLevel::kInfo? "INFO" : (lvl==LogLevel::kWarn? "WARN":"ERROR"));
  std::cout << now_iso8601() << " [" << tag << "] " << msg << std::endl;
}
} // namespace neon
