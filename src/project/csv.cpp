#include "project/csv.hpp"
#include <fstream>
#include <sstream>

namespace neon {

static inline std::vector<std::string> parse_csv_line(const std::string& line) {
  std::vector<std::string> out;
  std::string cur;
  bool inq=false;
  for (size_t i=0;i<line.size();++i) {
    char c=line[i];
    if (c=='"') {
      if (inq && i+1<line.size() && line[i+1]=='"') { cur.push_back('"'); ++i; }
      else inq=!inq;
    } else if (c==',' && !inq) {
      out.push_back(cur); cur.clear();
    } else cur.push_back(c);
  }
  out.push_back(cur);
  return out;
}

void read_csv(const std::string& path,
              const std::function<void(const std::vector<std::string>&)>& on_row,
              bool skip_header) {
  std::ifstream f(path);
  if (!f.good()) return;
  std::string line; bool first=true;
  while (std::getline(f, line)) {
    if (first && skip_header) { first=false; continue; }
    auto row = parse_csv_line(line);
    if (!row.empty() && !(row.size()==1 && row[0].empty()))
      on_row(row);
    first=false;
  }
}

}
