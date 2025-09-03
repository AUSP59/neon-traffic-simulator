#include "project/network.hpp"
#include <cassert>
using namespace neon;

int main() {
  RoadNetwork net;
  net.add_node({1,0,0});
  net.add_node({2,1,0});
  net.add_node({3,1,1});
  net.add_edge({10,1,2,100,10,10,1});
  net.add_edge({11,2,3,100,10,10,1});
  auto p_astar = net.shortest_path(1,3,true);
  auto p_dijk  = net.shortest_path(1,3,false);
  assert(p_astar.has_value() && p_dijk.has_value());
  assert(p_astar->size()==p_dijk->size());
  return 0;
}
