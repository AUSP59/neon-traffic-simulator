#include "project/network.hpp"
#include "project/simulator.hpp"
#include "project/traffic_light.hpp"
#include <cassert>

using namespace neon;

int main() {
  RoadNetwork net;
  net.add_node({1,0,0});
  net.add_node({2,1,0});
  net.add_edge({10,1,2,100.0, 10.0, 10});
  TrafficLights tl;
  Simulator sim(net, tl);
  sim.add_vehicle({1,1,2,0.0});
  sim.run(20.0, 0.1);
  // expect finished >=1
  assert(sim.metrics().finished >= 1);
  return 0;
}
