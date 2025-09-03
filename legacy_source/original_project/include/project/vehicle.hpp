#pragma once
#include <string>
#include <vector>
#include <optional>

namespace neon {

struct Vehicle {
  int id;
  int origin_node;
  int dest_node;
  double depart_time_s{0.0};

  // runtime:
  size_t route_index{0};
  double edge_progress_m{0.0};
  bool finished{false};
};

} // namespace neon
