#pragma once
#include "project/network.hpp"
#include <deque>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>

namespace neon {

struct Vehicle { int id; int origin_node; int dest_node; double depart_time_s; };

struct Metrics { double time_s; int active; int finished; double mean_travel_time_s; double throughput_vps; };

class Simulator {
 public:
  Simulator(const RoadNetwork& net) : net_(net) {}
  void add_vehicle(const Vehicle& v) { vehicles_.push_back(v); }
  void run(double duration_s, double tick_s, const std::string& out_dir);

 private:
  const RoadNetwork& net_;
  std::vector<Vehicle> vehicles_;
};

}
