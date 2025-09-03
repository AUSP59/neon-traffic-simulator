#pragma once
#include "network.hpp"
#include "vehicle.hpp"
#include "traffic_light.hpp"
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

namespace neon {

struct Event {
  int vehicle_id;
  std::string type; // "depart","arrive","edge_enter","edge_exit"
  double time_s;
  int ref_id; // edge id or node id depending on type
};

struct Metrics {
  double time_s{0};
  int active{0};
  int finished{0};
  double mean_travel_time_s{0};
  double throughput_vps{0}; // vehicles per second (completed/time)
};

class Simulator {
 public:
  Simulator(const RoadNetwork& net, const TrafficLights& tl);
  void add_vehicle(const Vehicle& v);
  void run(double duration_s, double tick_s, const std::string& out_dir = "outputs");
  const std::vector<Event>& events() const { return events_; }
  const Metrics& metrics() const { return metrics_; }

 public:
  void set_use_astar(bool b) { use_astar_ = b; }
 private:
  struct Occupant {
    int vehicle_id;
    double pos_m; // [0, edge.length]
    double speed_mps; // current speed
  };
  struct EdgeState {
    int edge_id;
      // occupants kept sorted by pos_m ascending
    std::deque<Occupant> occ;
  };

  const RoadNetwork& net_;
  const TrafficLights& tl_;
  std::unordered_map<int, Vehicle> vehicles_;
  std::unordered_map<int, EdgeState> edges_state_;
  std::vector<Event> events_;
  Metrics metrics_;
  bool use_astar_{true};

  // dynamics params (IDM)
  double min_gap_m_ = 2.0;   // s0 minimal gap
  double headway_s_ = 1.2;   // T desired headway
  double idm_a_ = 1.2;       // max accel m/s^2
  double idm_b_ = 2.0;       // comfortable decel m/s^2

  void ensure_edge_state_(int edge_id);
  bool can_enter_edge_(int edge_id) const;
  void enter_edge_(int vid, int edge_id, double t);
};

} // namespace neon
