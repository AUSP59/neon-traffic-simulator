#include "neon/sim.hpp"
#include <cassert>
int main(){
  auto r = neon::run_sim(1.0, 0.1, "outputs_test");
  assert(r.finished==1);
  return 0;
}
