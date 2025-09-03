#include "project/csv.hpp"
#include <fstream>
#include <sstream>

namespace neon {

static std::vector<std::string> parse_csv_line(const std::string& line) {
  std::vector<std::string> out;
  std::string field;
  bool in_quotes = false;
  for (size_t i=0;i<line.size();++i) {
    char c = line[i];
    if (in_quotes) {
      if (c=='"') {
        if (i+1<line.size() && line[i+1]=='"') { field.push_back('"'); ++i; }
        else { in_quotes = false; }
      } else {
        field.push_back(c);
      }
    } else {
      if (c==',') { out.push_back(field); field.clear(); }
      else if (c=='"') { in_quotes = true; }
      else { field.push_back(c); }
    }
  }
  out.push_back(field);
  return out;
}

void read_csv(const std::string& path,
              const std::function<void(const std::vector<std::string>&)>& row_cb,
              bool has_header) {
  std::ifstream f(path);
  if (!f.is_open()) return;
  std::string line;
  bool first = has_header;
  while (std::getline(f, line)) {
    if (line.empty()) continue;
    if (first) { first=false; continue; }
    auto cols = parse_csv_line(line);
    row_cb(cols);
  }
}

} // namespace neon
