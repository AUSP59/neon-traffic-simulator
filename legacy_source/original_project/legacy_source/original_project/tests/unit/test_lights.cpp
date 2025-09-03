#include "project/traffic_light.hpp"
#include <cassert>
int main() {
  neon::TrafficLights tl;
  tl.set_plan(1, {10, 10, 2, 0});
  // At t=0, NS should be green
  assert(tl.ns_green(1,0.0));
  // After NS+amber, EW should be green
  assert(tl.ew_green(1,12.5));
  return 0;
}
