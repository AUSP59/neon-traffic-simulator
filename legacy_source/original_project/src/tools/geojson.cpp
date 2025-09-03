#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include "project/csv.hpp"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
  std::string network_dir = "examples/grid4/network";
  std::string out = "outputs/network.geojson";
  for (int i=1;i<argc;i++) {
    std::string a = argv[i];
    if (a=="--network" && i+1<argc) network_dir = argv[++i];
    else if (a=="--out" && i+1<argc) out = argv[++i];
  }
  std::vector<std::tuple<int,double,double>> nodes;
  neon::read_csv(fs::path(network_dir)/"nodes.csv", [&](const std::vector<std::string>& r){
    if (r.size()<3) return;
    nodes.emplace_back(std::stoi(r[0]), std::stod(r[1]), std::stod(r[2]));
  });
  std::vector<std::tuple<int,int,int>> edges;
  neon::read_csv(fs::path(network_dir)/"edges.csv", [&](const std::vector<std::string>& r){
    if (r.size()<6) return;
    edges.emplace_back(std::stoi(r[1]), std::stoi(r[2]), std::stoi(r[0]));
  });
  // Build quick node map
  std::map<int, std::pair<double,double>> nmap;
  for (auto& n : nodes) nmap[std::get<0>(n)] = {std::get<1>(n), std::get<2>(n)};
  fs::create_directories(fs::path(out).parent_path());
  std::ofstream f(out);
  f << "{\n  \"type\": \"FeatureCollection\",\n  \"features\": [\n";
  bool first=true;
  for (auto& e : edges) {
    int from = std::get<0>(e), to = std::get<1>(e), id = std::get<2>(e);
    auto a = nmap[from]; auto b = nmap[to];
    if (!first) f << ",\n";
    first = false;
    f << "    { \"type\": \"Feature\", \"properties\": { \"id\": " << id << " }, \"geometry\": { \"type\": \"LineString\", \"coordinates\": [["
      << a.first << ", " << a.second << "], [" << b.first << ", " << b.second << "]] } }";
  }
  f << "\n  ]\n}\n";
  std::cout << "GeoJSON written to " << out << "\n";
  return 0;
}
