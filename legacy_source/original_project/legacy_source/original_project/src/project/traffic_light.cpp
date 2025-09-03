#include "project/traffic_light.hpp"
#include <cmath>

namespace neon {

static double mod(double a, double b) {
  return a - std::floor(a / b) * b;
}

bool TrafficLights::ns_green(int node_id, double t) const {
  auto it = plans.find(node_id);
  if (it == plans.end()) return true; // default permissive
  const auto& p = it->second;
  double cycle = p.green_ns_s + p.amber_s + p.green_ew_s + p.amber_s;
  if (cycle <= 0.1) return true;
  double tt = mod(t + p.offset_s, cycle);
  return tt < p.green_ns_s;
}

bool TrafficLights::ew_green(int node_id, double t) const {
  auto it = plans.find(node_id);
  if (it == plans.end()) return true;
  const auto& p = it->second;
  double cycle = p.green_ns_s + p.amber_s + p.green_ew_s + p.amber_s;
  if (cycle <= 0.1) return true;
  double tt = mod(t + p.offset_s, cycle);
  return (tt > (p.green_ns_s + p.amber_s)) && (tt < (p.green_ns_s + p.amber_s + p.green_ew_s));
}

} // namespace neon
