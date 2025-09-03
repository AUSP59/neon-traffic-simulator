#include "project/network.hpp"
#include <cassert>
int main(){
  neon::RoadNetwork net; net.add_node({1,0,0}); net.add_node({2,10,0});
  net.add_edge({10,1,2,100,10,10,1});
  auto p = net.shortest_path(1,2);
  assert(p.has_value());
  return 0;
}
