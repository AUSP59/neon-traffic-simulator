#include "project/csv.hpp"
#include "project/network.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
namespace fs = std::filesystem;
using namespace neon;
int main(int argc, char** argv){
  std::string netdir="examples/grid4/network", out="outputs/network.geojson";
  for (int i=1;i<argc;i++){ std::string a=argv[i];
    auto next=[&](const char* n){ if (i+1<argc) return std::string(argv[++i]); std::cerr<<"Missing value "<<n<<"\n"; exit(2); };
    if (a=="--network") netdir=next("--network");
    else if (a=="--out") out=next("--out");
  }
  RoadNetwork net;
  read_csv((fs::path(netdir)/"nodes.csv").string(), [&](const std::vector<std::string>& r){
    if (r.size()>=3) net.add_node({std::stoi(r[0]), std::stod(r[1]), std::stod(r[2])});
  });
  read_csv((fs::path(netdir)/"edges.csv").string(), [&](const std::vector<std::string>& r){
    if (r.size()>=6) net.add_edge({std::stoi(r[0]), std::stoi(r[1]), std::stoi(r[2]), std::stod(r[3]), std::stod(r[4]), std::stoi(r[5]), r.size()>=7?std::stoi(r[6]):1});
  });
  std::map<int,std::pair<double,double>> nm; for (auto& kv: net.nodes) nm[kv.first]={kv.second.x, kv.second.y};
  fs::create_directories(fs::path(out).parent_path());
  std::ofstream f(out);
  f<<"{\n  \"type\": \"FeatureCollection\",\n  \"features\": [\n";
  bool first=true;
  for (auto& kv : net.edges){
    auto e = kv.second; auto a=nm[e.from]; auto b=nm[e.to];
    if (!first) f<<",\n"; first=false;
    f<<"    { \"type\": \"Feature\", \"properties\": { \"id\": "<<e.id<<" }, \"geometry\": { \"type\": \"LineString\", \"coordinates\": [["<<a.first<<", "<<a.second<<"], ["<<b.first<<", "<<b.second<<"]] } }";
  }
  f<<"\n  ]\n}\n";
  std::cout<<"GeoJSON written to "<<out<<"\n";
  return 0;
}
