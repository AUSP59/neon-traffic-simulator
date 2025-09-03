#include "project/csv.hpp"
#include <string>
#include <vector>
#include <functional>

extern "C" int LLVMFuzzerTestOneInput(const unsigned char *data, size_t size) {
  std::string s(reinterpret_cast<const char*>(data), size);
  // simulate read_csv line splitting logic
  size_t pos = 0;
  while (true) {
    auto end = s.find('\n', pos);
    std::string line = s.substr(pos, end == std::string::npos ? s.size()-pos : end-pos);
    (void)line; // intentionally ignore
    if (end == std::string::npos) break;
    pos = end + 1;
  }
  return 0;
}
