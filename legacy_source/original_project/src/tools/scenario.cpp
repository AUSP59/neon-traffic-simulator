#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "project/network.hpp"
#include "project/traffic_light.hpp"
#include "project/simulator.hpp"
#include "project/csv.hpp"

namespace fs = std::filesystem;

// Minimal JSON reader for specific keys (no external deps)
static std::string read_file(const std::string& p){ std::ifstream f(p); return std::string((std::istreambuf_iterator<char>(f)),{}); }
static std::string get(const std::string& s, const std::string& key, const std::string& def="") {
  auto k = std::string("\"")+key+"\"";
  auto pos = s.find(k); if (pos==std::string::npos) return def;
  pos = s.find(':', pos); if (pos==std::string::npos) return def;
  auto start = s.find_first_of(""0123456789.-", pos+1);
  if (start==std::string::npos) return def;
  if (s[start]=='"') { auto end = s.find('"', start+1); return s.substr(start+1, end-start-1); }
  auto end = s.find_first_not_of("0123456789.-", start);
  return s.substr(start, end-start);
}

int main(int argc, char** argv) {
  if (argc<3 || std::string(argv[1])!="--config") {
    std::cerr << "Usage: neon_scenario --config scenario.json\n";
    return 2;
  }
  std::string cfg = argv[2];
  auto js = read_file(cfg);
  std::string network_dir = get(js, "network", "examples/grid4/network");
  std::string vehicles_csv = get(js, "vehicles", "examples/grid4/vehicles.csv");
  std::string lights_csv = get(js, "lights", "examples/grid4/lights.csv");
  std::string out_dir = get(js, "out", "outputs");
  std::string router = get(js, "router", "astar");
  double duration_s = std::stod(get(js, "duration", "300"));
  double tick_ms = std::stod(get(js, "tick_ms", "100"));

  neon::RoadNetwork net;
  neon::read_csv(fs::path(network_dir)/"nodes.csv", [&](const std::vector<std::string>& r){
    if (r.size()<3) return;
    net.add_node({std::stoi(r[0]), std::stod(r[1]), std::stod(r[2])});
  });
  neon::read_csv(fs::path(network_dir)/"edges.csv", [&](const std::vector<std::string>& r){
    if (r.size()<6) return;
    neon::Edge e{std::stoi(r[0]), std::stoi(r[1]), std::stoi(r[2]), std::stod(r[3]), std::stod(r[4]), std::stoi(r[5])};
    if (r.size()>=7) e.lanes = std::stoi(r[6]);
    net.add_edge(e);
  });
  neon::TrafficLights tl;
  neon::read_csv(lights_csv, [&](const std::vector<std::string>& r){
    if (r.size() < 5) return;
    int node = std::stoi(r[0]);
    neon::SignalPlan p{std::stod(r[1]), std::stod(r[2]), std::stod(r[3]), std::stod(r[4])};
    tl.set_plan(node, p);
  });
  neon::Simulator sim(net, tl);
  if (router=="dijkstra") sim.set_use_astar(false);
  neon::read_csv(vehicles_csv, [&](const std::vector<std::string>& r){
    if (r.size() < 4) return;
    neon::Vehicle v{std::stoi(r[0]), std::stoi(r[1]), std::stoi(r[2]), std::stod(r[3])};
    sim.add_vehicle(v);
  });
  sim.run(duration_s, tick_ms/1000.0, out_dir);
  std::cout << "Scenario completed. Outputs at " << out_dir << "\n";
  return 0;
}
