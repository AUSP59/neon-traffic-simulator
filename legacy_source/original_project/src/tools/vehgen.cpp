#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
  int count=100;
  int origin=1, destination=3;
  double rate=0.5; // departures per second
  std::string out="examples/grid4/vehicles.csv";
  unsigned seed=12345;

  for (int i=1;i<argc;i++) {
    std::string a=argv[i];
    auto next = [&](const char* name){ if (i+1 < argc) return std::string(argv[++i]); std::cerr << "Missing value for " << name << "\n"; exit(2); };
    if (a=="--count") count = std::stoi(next("--count"));
    else if (a=="--origin") origin = std::stoi(next("--origin"));
    else if (a=="--destination") destination = std::stoi(next("--destination"));
    else if (a=="--rate") rate = std::stod(next("--rate"));
    else if (a=="--out") out = next("--out");
    else if (a=="--seed") seed = (unsigned)std::stoul(next("--seed"));
  }

  fs::create_directories(fs::path(out).parent_path());
  std::ofstream f(out);
  f << "id,origin,destination,depart_time_s\n";
  std::mt19937 rng(seed);
  std::exponential_distribution<double> exp(rate);
  double t=0.0;
  for (int i=0;i<count;i++) {
    t += exp(rng);
    f << (i+1) << "," << origin << "," << destination << "," << t << "\n";
  }
  std::cout << "vehicles.csv generated at " << out << "\n";
  return 0;
}
