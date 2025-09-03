#include "project/csv.hpp"
#include <cassert>
#include <fstream>

int main() {
  const char* p = "tmp.csv";
  std::ofstream f(p);
  f << "a,b,c\n"x,1",2,"3"\n";
  f.close();
  int rows=0;
  neon::read_csv(p, [&](const std::vector<std::string>& r){
    rows++;
    assert(r.size()==3);
    assert(r[0]=="x,1");
  }, true);
  assert(rows==1);
  std::remove(p);
  return 0;
}
