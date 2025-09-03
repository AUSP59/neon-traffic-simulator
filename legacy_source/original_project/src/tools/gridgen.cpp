#include "project/csv.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
  int rows=4, cols=4; double length=100; double speed=10; int cap=10;
  std::string out="examples/grid4/network";
  for (int i=1;i<argc;i++) {
    std::string a = argv[i];
    auto next = [&](const char* name){ if (i+1 < argc) return std::string(argv[++i]); std::cerr << "Missing value for " << name << "\n"; exit(2); };
    if (a=="--rows") rows = std::stoi(next("--rows"));
    else if (a=="--cols") cols = std::stoi(next("--cols"));
    else if (a=="--len") length = std::stod(next("--len"));
    else if (a=="--speed") speed = std::stod(next("--speed"));
    else if (a=="--cap") cap = std::stoi(next("--cap"));
    else if (a=="--out") out = next("--out");
  }
  fs::create_directories(out);
  std::ofstream nodes(fs::path(out)/"nodes.csv");
  nodes << "id,x,y\n";
  int id=1;
  for (int r=0;r<rows;r++)
    for (int c=0;c<cols;c++)
      nodes << id++ << "," << c << "," << r << "\n";
  std::ofstream edges(fs::path(out)/"edges.csv");
  edges << "id,from,to,length,speed_limit,capacity\n";
  int e=10;
  auto idx=[&](int r,int c){return r*cols+c+1;};
  for (int r=0;r<rows;r++) for (int c=0;c<cols;c++) {
    if (c+1<cols) edges << e++ << "," << idx(r,c) << "," << idx(r,c+1) << "," << length << "," << speed << "," << cap << "\n";
    if (r+1<rows) edges << e++ << "," << idx(r,c) << "," << idx(r+1,c) << "," << length << "," << speed << "," << cap << "\n";
  }
  std::cout << "Grid generated at " << out << "\n";
  return 0;
}
