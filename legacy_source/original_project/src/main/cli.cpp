#include "neon/sim.hpp"
#include <iostream>
#include <string>

int main(int argc, char** argv){
  if (argc>=2 && std::string(argv[1])=="--version"){
    std::cout << "NeonTrafficSimulator 3.1.0\n";
    return 0;
  }
  if (argc<2 || std::string(argv[1])!="simulate"){
    std::cout << "Usage: neon_traffic_cli simulate [--duration <sec>] [--tick-ms <ms>] [--out <dir>]\n";
    return 2;
  }
  double duration=5.0; double tick_ms=100.0; std::string out="outputs";
  for (int i=2;i<argc;i++){
    std::string a=argv[i];
    auto next=[&](const char* name){ if (i+1<argc) return std::string(argv[++i]); std::cerr<<"Missing value for "<<name<<"\n"; std::exit(2); };
    if (a=="--duration") duration = std::stod(next("--duration"));
    else if (a=="--tick-ms") tick_ms = std::stod(next("--tick-ms"));
    else if (a=="--out") out = next("--out");
  }
  auto res = neon::run_sim(duration, tick_ms/1000.0, out);
  std::cout << "OK duration="<<res.duration_s<<" finished="<<res.finished<<"\n";
  return 0;
}
