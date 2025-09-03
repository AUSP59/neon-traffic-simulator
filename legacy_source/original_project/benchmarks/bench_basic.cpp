#include "project/network.hpp"
#include "project/simulator.hpp"
#include "project/traffic_light.hpp"
#include <chrono>
#include <iostream>

using namespace neon;

int main() {
  RoadNetwork net;
  for (int i=0;i<50;i++) net.add_node({i, double(i%10), double(i/10)});
  int eid=0;
  for (int i=0;i<49;i++) net.add_edge({eid++, i, i+1, 100.0, 15.0, 10});
  TrafficLights tl;
  Simulator sim(net, tl);
  for (int i=0;i<200;i++) sim.add_vehicle({i, 0, 49, 0.0});

  auto t0 = std::chrono::steady_clock::now();
  sim.run(300.0, 0.1);
  auto t1 = std::chrono::steady_clock::now();
  std::chrono::duration<double> dt = t1 - t0;
  std::cout << "Sim time: " << dt.count() << " s\n";
  std::cout << "Finished: " << sim.metrics().finished << "\n";
  return 0;
}
