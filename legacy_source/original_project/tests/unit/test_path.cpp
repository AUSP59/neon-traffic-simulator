#include "project/network.hpp"
#include <cassert>
using namespace neon;

int main() {
  RoadNetwork net;
  net.add_node({1,0,0});
  net.add_node({2,1,0});
  net.add_node({3,2,0});
  net.add_edge({10,1,2,100,10,10});
  net.add_edge({11,2,3,100,10,10});
  auto p = net.shortest_path(1,3);
  assert(p.has_value());
  assert(p->size()==2);
  return 0;
}
