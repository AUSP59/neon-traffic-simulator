#include "project/simulator.hpp"
#include "project/csv.hpp"
#include <algorithm>

namespace fs = std::filesystem;
namespace neon {

void Simulator::run(double duration_s, double tick_s, const std::string& out_dir) {
  fs::create_directories(out_dir);
  std::ofstream metrics(out_dir + "/metrics.csv");
  metrics << "time_s,active,finished,mean_travel_time_s,throughput_vps
";
  std::ofstream ts(out_dir + "/metrics_timeseries.csv");
  ts << "time_s,active,finished,mean_travel_time_s,throughput_vps
";

  // Dumb queue model: vehicles "finish" after a fixed travel time derived from path length
  struct Runtime { int id; double start; double eta; };
  std::vector<Runtime> actives;
  double t=0.0; int finished=0; double total_tt=0.0;
  // Pre-assign ETA for each vehicle upon its depart
  std::map<int,double> eta_by_id;

  while (t <= duration_s + 1e-9) {
    // depart any vehicles scheduled before t
    for (auto& v : vehicles_) {
      if (!eta_by_id.count(v.id) && v.depart_time_s <= t) {
        // Compute a crude path-based travel time
        double path_len = 100.0; // default
        if (auto p = net_.shortest_path(v.origin_node, v.dest_node)) {
          path_len = 0.0;
          for (auto& step : *p) path_len += net_.edges.at(step.edge_id).length;
        }
        double speed = 10.0; // m/s default free-flow
        double travel = path_len / speed;
        eta_by_id[v.id] = t + travel;
        actives.push_back({v.id, t, t + travel});
      }
    }

    // finish events
    for (auto it = actives.begin(); it != actives.end();) {
      if (it->eta <= t + 1e-9) {
        finished++;
        total_tt += (it->eta - it->start);
        it = actives.erase(it);
      } else ++it;
    }

    double mean_tt = finished ? total_tt / finished : 0.0;
    double thr = t>0 ? (double)finished / t : 0.0;
    ts << std::fixed << std::setprecision(6) << t << "," << actives.size() << "," << finished << "," << mean_tt << "," << thr << "
";

    t += tick_s;
  }
  double mean_tt = finished ? total_tt / finished : 0.0;
  double thr = duration_s>0 ? (double)finished / duration_s : 0.0;
  metrics << std::fixed << std::setprecision(6) << duration_s << "," << 0 << "," << finished << "," << mean_tt << "," << thr << "
";
}

}
