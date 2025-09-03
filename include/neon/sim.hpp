#pragma once
#include <string>
namespace neon {
struct Result {
  double duration_s{};
  int finished{};
};
// Runs a deterministic tiny simulation and writes metrics CSVs.
Result run_sim(double duration_s, double tick_s, const std::string& out_dir);
} // namespace neon
