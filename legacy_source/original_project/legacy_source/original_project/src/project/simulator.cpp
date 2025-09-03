#include "project/simulator.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <unordered_set>

namespace fs = std::filesystem;
namespace neon {

Simulator::Simulator(const RoadNetwork& net, const TrafficLights& tl)
  : net_(net), tl_(tl) {
  for (const auto& kv : net_.edges) edges_state_[kv.first] = EdgeState{kv.first, {}};
}

void Simulator::add_vehicle(const Vehicle& v) { vehicles_[v.id] = v; }

void Simulator::ensure_edge_state_(int edge_id) {
  if (!edges_state_.count(edge_id)) edges_state_[edge_id] = EdgeState{edge_id, {}};
}

bool Simulator::can_enter_edge_(int edge_id) const {
  auto it = edges_state_.find(edge_id);
  if (it == edges_state_.end()) return true;
  const auto& st = it->second;
  const auto& e = net_.edges.at(edge_id);
  return (int)st.occ.size() < e.capacity;
}

void Simulator::enter_edge_(int vid, int edge_id, double t) {
  auto& st = edges_state_.at(edge_id);
  st.occ.push_back({vid, 0.0});
  events_.push_back({vid, "edge_enter", t, edge_id});
}

void Simulator::run(double duration_s, double tick_s, const std::string& out_dir) {
  fs::create_directories(out_dir);

  double t = 0.0;
  // Precompute routes
  std::unordered_map<int, std::vector<PathStep>> routes;
  for (auto& kv : vehicles_) {
    const auto& v = kv.second;
    auto path = net_.shortest_path(v.origin_node, v.dest_node, use_astar_);
    if (path) routes[v.id] = *path;
  }

  // Departures
  auto try_depart = [&](int vid, double tcur){
    auto& v = vehicles_.at(vid);
    if (v.finished) return;
    if (!routes.count(v.id)) { v.finished = true; return; }
    if (v.route_index >= routes[v.id].size()) { v.finished = true; return; }
    int eid = routes[v.id][v.route_index].edge_id;
    if (can_enter_edge_(eid)) {
      ensure_edge_state_(eid);
      enter_edge_(v.id, eid, tcur);
      events_.push_back({v.id, "depart", tcur, v.origin_node});
    }
  };

  metrics_ = Metrics{};
  std::unordered_map<int, double> depart_time;
  int completed_count = 0;

  std::ofstream ts((fs::path(out_dir) / "metrics_timeseries.csv").string());
  ts << "time_s,active,finished,mean_travel_time_s,throughput_vps\n";
  while (t <= duration_s + 1e-9) {
    // Depart vehicles whose time has come
    for (auto& kv : vehicles_) {
      if (kv.second.depart_time_s <= t && kv.second.route_index==0 && kv.second.edge_progress_m==0.0) {
        try_depart(kv.first, t);
        if (!depart_time.count(kv.first)) depart_time[kv.first] = t;
      }
    }

    // Advance vehicles on edges (front to back to avoid passing)
    for (auto& kv : edges_state_) {
      int eid = kv.first;
      auto& st = kv.second;
      if (st.occ.empty()) continue;
      const auto& e = net_.edges.at(eid);

      // First vehicle free-flow
      double v_free = e.speed_limit;
      if (!st.occ.empty()) {
        auto& front = st.occ.front();
        front.pos_m = std::min(e.length, front.pos_m + v_free * tick_s);
      }

      // Followers constrained by leader
      for (size_t i=1; i<st.occ.size(); ++i) {
        auto& follower = st.occ[i];
        const auto& leader = st.occ[i-1];
        double follower_speed = e.speed_limit;
        double desired_gap = min_gap_m_ + headway_s_ * follower_speed;
        double max_pos = std::min(e.length, leader.pos_m - desired_gap);
        double next_pos = std::min(e.length, follower.pos_m + follower_speed * tick_s);
        follower.pos_m = std::min(next_pos, std::max(0.0, max_pos));
      }

      // Handle exits
      while (!st.occ.empty() && st.occ.front().pos_m >= e.length - 1e-6) {
        int vid = st.occ.front().vehicle_id;
        st.occ.pop_front();
        auto& v = vehicles_.at(vid);
        events_.push_back({vid, "edge_exit", t, eid});
        v.route_index++;

        if (!routes.count(v.id) || v.route_index >= routes[v.id].size()) {
          v.finished = true;
          events_.push_back({v.id, "arrive", t, e.to});
          completed_count++;
        } else {
          int next_eid = routes[v.id][v.route_index].edge_id;
          // Light gating
          auto nf = net_.nodes.at(net_.edges.at(eid).to);
          auto nt = net_.nodes.at(net_.edges.at(next_eid).to);
          bool ns = std::abs(nt.y - nf.y) > std::abs(nt.x - nf.x);
          bool green = ns ? tl_.ns_green(nf.id, t) : tl_.ew_green(nf.id, t);
          if (green && can_enter_edge_(next_eid)) {
            ensure_edge_state_(next_eid);
            enter_edge_(v.id, next_eid, t);
          } else {
            // Couldn't enter; keep vehicle at end of edge (reinsert at front with pos slightly less than length)
            st.occ.push_front({vid, e.length - 0.01});
            break; // Avoid infinite loop; process next edge in next tick
          }
        }
      }
    }

    // Metrics
    int active = 0, finished = 0;
    double total_tt = 0.0; int total_finished = 0;
    for (const auto& kvv : vehicles_) {
      if (kvv.second.finished) finished++;
      else if (kvv.second.depart_time_s <= t) active++;
    }
    for (const auto& ev : events_) if (ev.type=="arrive") {
      auto it = depart_time.find(ev.vehicle_id);
      if (it!=depart_time.end()) { total_tt += (ev.time_s - it->second); total_finished++; }
    }
    metrics_ = Metrics{t, active, finished, total_finished? total_tt/total_finished : 0.0,
                       t>0 ? completed_count / t : 0.0};
    ts << std::fixed << std::setprecision(6) << metrics_.time_s << "," << metrics_.active << ","
       << metrics_.finished << "," << metrics_.mean_travel_time_s << "," << metrics_.throughput_vps << "\n";
    t += tick_s;
  }

  // Write outputs
  fs::create_directories(out_dir);
  std::ofstream mcsv(fs::path(out_dir) / "metrics.csv");
  mcsv << "time_s,active,finished,mean_travel_time_s,throughput_vps
";
  mcsv << std::fixed << std::setprecision(6)
       << metrics_.time_s << "," << metrics_.active << "," << metrics_.finished << ","
       << metrics_.mean_travel_time_s << "," << metrics_.throughput_vps << "
";

  std::ofstream ecsv(fs::path(out_dir) / "events.csv");
  ecsv << "vehicle_id,type,time_s,ref_id
";
  for (const auto& ev : events_) {
    ecsv << ev.vehicle_id << "," << ev.type << "," << ev.time_s << "," << ev.ref_id << "
";
  }

  // Manifest
  std::ofstream man(fs::path(out_dir) / "manifest.json");
  man << "{\n"
      << "  \"engine\": \"NeonTrafficSimulator\",\n"
      << "  \"version\": \"0.2.0-ultra\",\n"
      << "  \"tick_s\": " << tick_s << ",\n"
      << "  \"duration_s\": " << duration_s << "\n"
      << "}\n";
}

} // namespace neon
