#pragma once
#include <functional>
#include <string>
#include <vector>

namespace neon {

// Robust CSV reader supporting quoted fields with commas and escaped quotes.
void read_csv(const std::string& path,
              const std::function<void(const std::vector<std::string>&)>& row_cb,
              bool has_header = true);

} // namespace neon
