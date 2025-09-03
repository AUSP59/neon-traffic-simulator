#include "project/network.hpp"
#include <queue>
#include <cmath>

namespace neon {

void RoadNetwork::add_node(const Node& n) {
  nodes[n.id] = n;
}

void RoadNetwork::add_edge(const Edge& e) {
  edges[e.id] = e;
  adj[e.from].push_back(e.id);
}

std::optional<std::vector<PathStep>> RoadNetwork::shortest_path(int origin_node, int dest_node) const {
  return shortest_path(origin_node, dest_node, true);
}

std::optional<std::vector<PathStep>> RoadNetwork::shortest_path(int origin_node, int dest_node, bool use_astar) const {
  struct QItem { int node; double dist; int prev_edge; };
  const double INF = std::numeric_limits<double>::infinity();
  std::unordered_map<int, double> g; std::unordered_map<int, double> f;
  std::unordered_map<int, int> prev_edge;
  std::unordered_map<int, int> prev_node;

  for (const auto& kv : nodes) g[kv.first] = INF; f[kv.first] = INF;
  if (!nodes.count(origin_node) || !nodes.count(dest_node)) return std::nullopt;
  g[origin_node] = 0.0; f[origin_node] = 0.0;

  auto cmp = [](const QItem& a, const QItem& b){ return a.dist > b.dist; };
  std::priority_queue<QItem, std::vector<QItem>, decltype(cmp)> pq(cmp);
  pq.push({origin_node, 0.0, -1});

  while (!pq.empty()) {
    auto cur = pq.top(); pq.pop();
    if (cur.dist != f[cur.node]) continue;
    if (cur.node == dest_node) break;
    auto it = adj.find(cur.node);
    if (it == adj.end()) continue;
    for (int eid : it->second) {
      const auto& e = edges.at(eid);
      double w = e.length / std::max(1.0, e.speed_limit); // time weight
      double tentative_g = g[cur.node] + w;
      if (g[e.to] > tentative_g) {
        g[e.to] = tentative_g;
        double hx = 0.0;
        if (use_astar) {
          auto a = nodes.at(e.to); auto b = nodes.at(dest_node);
          double dx = a.x - b.x, dy = a.y - b.y;
          double eu = std::sqrt(dx*dx + dy*dy);
          double vref = std::max(1.0, e.speed_limit);
          hx = eu / vref; // optimistic time
        }
        f[e.to] = g[e.to] + hx;
        prev_edge[e.to] = eid;
        prev_node[e.to] = cur.node;
        pq.push({e.to, f[e.to], eid});
      }
    }
  }

  if (g[dest_node] == INF) return std::nullopt;

  // Reconstruct path
  std::vector<PathStep> path;
  int cur = dest_node;
  while (cur != origin_node) {
    int pe = prev_edge.at(cur);
    path.push_back({pe});
    cur = prev_node.at(cur);
  }
  std::reverse(path.begin(), path.end());
  return path;
}

} // namespace neon
