#pragma once
#include <unordered_map>

namespace neon {

struct SignalPlan {
  double green_ns_s{20};
  double green_ew_s{20};
  double amber_s{3};
  double offset_s{0};
};

struct TrafficLights {
  // intersection id -> plan
  std::unordered_map<int, SignalPlan> plans;

  void set_plan(int node_id, const SignalPlan& p) { plans[node_id] = p; }
  // returns true if movement from N<->S is green at time t
  bool ns_green(int node_id, double t) const;
  // returns true if movement from E<->W is green at time t
  bool ew_green(int node_id, double t) const;
};

} // namespace neon
