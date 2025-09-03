#pragma once
#include <functional>
#include <string>
#include <vector>

namespace neon {
void read_csv(const std::string& path,
              const std::function<void(const std::vector<std::string>&)>& on_row,
              bool skip_header=true);
}
