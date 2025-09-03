#pragma once
#include <string>
namespace neon {
struct Result {
  double duration_s{};
  int finished{};
};
Result run_sim(double duration_s, double tick_s, const std::string& out_dir);
}
