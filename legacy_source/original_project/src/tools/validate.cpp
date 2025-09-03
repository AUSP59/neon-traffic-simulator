#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include "project/csv.hpp"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
  std::string network_dir = "examples/grid4/network";
  for (int i=1;i<argc;i++) {
    std::string a = argv[i];
    if (a=="--network" && i+1<argc) network_dir = argv[++i];
  }
  std::unordered_set<int> nodes;
  bool ok=true;
  // nodes
  neon::read_csv(fs::path(network_dir)/"nodes.csv", [&](const std::vector<std::string>& r){
    if (r.size()<3) { ok=false; return; }
    nodes.insert(std::stoi(r[0]));
  });
  // edges
  neon::read_csv(fs::path(network_dir)/"edges.csv", [&](const std::vector<std::string>& r){
    if (r.size()<6) { ok=false; return; }
    int from = std::stoi(r[1]), to = std::stoi(r[2]);
    if (!nodes.count(from) || !nodes.count(to)) ok=false;
  });
  std::cout << (ok? "Network is valid\n" : "Network has issues\n");
  return ok? 0 : 2;
}
