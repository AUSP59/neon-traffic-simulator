#include "project/csv.hpp"
#include "project/network.hpp"
#include "project/simulator.hpp"
#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;
using namespace neon;

int main(int argc, char** argv) {
  if (argc>=2 && std::string(argv[1])=="--version") {
    std::cout << "NeonTrafficSimulator 3.0.3\n";
    return 0;
  }
  if (argc<2 || std::string(argv[1])!="simulate") {
    std::cout << "Usage: neon_traffic_cli simulate --network <dir> --vehicles <file> --lights <file> --duration <sec> --tick-ms <ms> --out <dir>\n";
    return 2;
  }
  std::string netdir="examples/grid4/network";
  std::string vehicles="examples/grid4/vehicles.csv";
  std::string lights="examples/grid4/lights.csv";
  double duration=60; double tick_ms=100; std::string out="outputs";
  for (int i=2;i<argc;i++) {
    std::string a=argv[i];
    auto next=[&](const char* n){ if (i+1<argc) return std::string(argv[++i]); std::cerr<<"Missing value for "<<n<<"\n"; exit(2); };
    if (a=="--network") netdir = next("--network");
    else if (a=="--vehicles") vehicles = next("--vehicles");
    else if (a=="--lights") lights = next("--lights");
    else if (a=="--duration") duration = std::stod(next("--duration"));
    else if (a=="--tick-ms") tick_ms = std::stod(next("--tick-ms"));
    else if (a=="--out") out = next("--out");
  }

  RoadNetwork net;
  read_csv((fs::path(netdir)/"nodes.csv").string(), [&](const std::vector<std::string>& r){
    if (r.size()>=3) net.add_node({std::stoi(r[0]), std::stod(r[1]), std::stod(r[2])});
  }, true);
  read_csv((fs::path(netdir)/"edges.csv").string(), [&](const std::vector<std::string>& r){
    if (r.size()>=6) {
      Edge e{std::stoi(r[0]), std::stoi(r[1]), std::stoi(r[2]), std::stod(r[3]), std::stod(r[4]), std::stoi(r[5])};
      if (r.size()>=7) e.lanes = std::stoi(r[6]);
      net.add_edge(e);
    }
  }, true);

  Simulator sim(net);
  read_csv(vehicles, [&](const std::vector<std::string>& r){
    if (r.size()>=4) sim.add_vehicle({std::stoi(r[0]), std::stoi(r[1]), std::stoi(r[2]), std::stod(r[3])});
  }, true);

  sim.run(duration, tick_ms/1000.0, out);
  std::cout << "Done. Outputs at " << out << "\n";
  return 0;
}
