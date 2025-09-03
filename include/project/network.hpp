#pragma once
#include <unordered_map>
#include <vector>
#include <optional>
#include <string>
namespace neon {

struct Node { int id; double x; double y; };
struct Edge { int id; int from; int to; double length; double speed_limit; int capacity; int lanes{1}; };

struct PathStep { int edge_id; };

struct RoadNetwork {
  std::unordered_map<int, Node> nodes;
  std::unordered_map<int, Edge> edges;
  void add_node(const Node& n) { nodes[n.id]=n; }
  void add_edge(const Edge& e) { edges[e.id]=e; }
  std::optional<std::vector<PathStep>> shortest_path(int origin_node, int dest_node) const;
};

}
