#include "project/network.hpp"
#include "project/simulator.hpp"
#include "project/traffic_light.hpp"
#include "project/csv.hpp"
#include "project/log.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

using namespace neon;
namespace fs = std::filesystem;

static void print_help() {
  std::cout << "NeonTrafficSimulator 3.0.0\n";CLI\n"
            << "Usage:\n"
            << "  neon_traffic_cli simulate [options]\n"
            << "  neon_traffic_cli --version\n"
            << "Options (simulate):\n"
            << "  --network <dir>    directory with nodes.csv and edges.csv\n"
            << "  --vehicles <file>  vehicles.csv\n"
            << "  --lights <file>    lights.csv\n"
            << "  --duration <sec>   simulation duration (default 300)\n"
            << "  --tick-ms <ms>     tick interval in ms (default 100)\n"
            << "  --out <dir>        output directory (default outputs)\n"
            << "  --seed <int>       deterministic seed placeholder (reserved)\n  --router <name>   astar|dijkstra (default astar)\n";
}

int main(int argc, char** argv) {
  if (argc<2) { print_help(); return 1; }
  std::string cmd = argv[1];
  if (cmd=="--version") {
    std::cout << "NeonTrafficSimulator 3.0.0\n";2.0.0\\n";
    return 0;
  }
  if (cmd!="simulate") {
    print_help();
    return 1;
  }

  std::string network_dir = "examples/grid4/network";
  std::string vehicles_csv = "examples/grid4/vehicles.csv";
  std::string lights_csv = "examples/grid4/lights.csv";
  double duration_s = 300.0;
  double tick_ms = 100.0;
  std::string out_dir = "outputs";
  std::string router = "astar";
  int seed = 0;

  for (int i=2;i<argc;i++) {
    std::string a = argv[i];
    auto next = [&](const char* name){ if (i+1 < argc) return std::string(argv[++i]); std::cerr << "Missing value for " << name << "\n"; exit(2); };
    if (a=="--network") network_dir = next("--network");
    else if (a=="--vehicles") vehicles_csv = next("--vehicles");
    else if (a=="--lights") lights_csv = next("--lights");
    else if (a=="--duration") duration_s = std::stod(next("--duration"));
    else if (a=="--tick-ms") tick_ms = std::stod(next("--tick-ms"));
    else if (a=="--out") out_dir = next("--out");
    else if (a=="--seed") seed = std::stoi(next("--seed"));
    else if (a=="--router") router = next("--router");
    else if (a=="--help" || a=="-h") { print_help(); return 0; }
  }

  log(LogLevel::kInfo, "Starting simulation");
  fs::create_directories(out_dir);

  RoadNetwork net;
  read_csv(fs::path(network_dir)/"nodes.csv", [&](const std::vector<std::string>& r){
    if (r.size() < 3) return;
    Node n{std::stoi(r[0]), std::stod(r[1]), std::stod(r[2])};
    net.add_node(n);
  });
  read_csv(fs::path(network_dir)/"edges.csv", [&](const std::vector<std::string>& r){
    if (r.size() < 6) return;
    Edge e{std::stoi(r[0]), std::stoi(r[1]), std::stoi(r[2]), std::stod(r[3]), std::stod(r[4]), std::stoi(r[5])};
    if (r.size()>=7) e.lanes = std::stoi(r[6]);
    net.add_edge(e);
  });

  TrafficLights tl;
  read_csv(lights_csv, [&](const std::vector<std::string>& r){
    if (r.size() < 5) return;
    int node = std::stoi(r[0]);
    SignalPlan p{std::stod(r[1]), std::stod(r[2]), std::stod(r[3]), std::stod(r[4])};
    tl.set_plan(node, p);
  });

  Simulator sim(net, tl);
  if (router=="dijkstra") sim.set_use_astar(false);
  read_csv(vehicles_csv, [&](const std::vector<std::string>& r){
    if (r.size() < 4) return;
    Vehicle v{std::stoi(r[0]), std::stoi(r[1]), std::stoi(r[2]), std::stod(r[3])};
    sim.add_vehicle(v);
  });

  sim.run(duration_s, tick_ms/1000.0, out_dir);

  log(LogLevel::kInfo, "Simulation complete. Outputs in: " + out_dir);
  return 0;
}
