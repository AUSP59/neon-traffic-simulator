#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <limits>
#include <optional>

namespace neon {

struct Node {
  int id;
  double x{0}, y{0};
};

struct Edge {
  int id;
  int from;
  int to;
  double length{1.0};      // meters
  double speed_limit{13.9}; // m/s (~50 km/h)
  int capacity{20};        // vehicles capacity (queue limit)
  int lanes{1};            // number of lanes (>=1)
};

struct PathStep {
  int edge_id;
};

struct RoadNetwork {
  std::unordered_map<int, Node> nodes;
  std::unordered_map<int, Edge> edges;
  // adjacency: from node -> outgoing edge ids
  std::unordered_map<int, std::vector<int>> adj;

  void add_node(const Node& n);
  void add_edge(const Edge& e);
  std::optional<std::vector<PathStep>> shortest_path(int origin_node, int dest_node) const;
  std::optional<std::vector<PathStep>> shortest_path(int origin_node, int dest_node, bool use_astar) const;
};

} // namespace neon
