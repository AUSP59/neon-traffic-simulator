#include "project/network.hpp"
#include <queue>
#include <cmath>
#include <limits>

namespace neon {

std::optional<std::vector<PathStep>> RoadNetwork::shortest_path(int origin_node, int dest_node) const {
  if (!nodes.count(origin_node) || !nodes.count(dest_node)) return std::nullopt;
  const double INF = std::numeric_limits<double>::infinity();
  std::unordered_map<int,double> dist;
  std::unordered_map<int,int> prev_edge;
  std::unordered_map<int,int> prev_node;
  for (auto& kv : nodes) dist[kv.first]=INF;
  dist[origin_node]=0.0;

  struct QItem{int node; double dist;};
  auto cmp=[](const QItem&a, const QItem&b){ return a.dist>b.dist; };
  std::priority_queue<QItem,std::vector<QItem>,decltype(cmp)> pq(cmp);
  pq.push({origin_node,0.0});

  while(!pq.empty()){
    auto cur=pq.top(); pq.pop();
    if (cur.dist!=dist[cur.node]) continue;
    if (cur.node==dest_node) break;
    for (auto& ekv : edges) {
      const auto& e = ekv.second;
      if (e.from!=cur.node) continue;
      double w = e.length / std::max(1.0, e.speed_limit);
      if (dist[e.to] > dist[cur.node] + w) {
        dist[e.to] = dist[cur.node] + w;
        prev_edge[e.to] = e.id;
        prev_node[e.to] = cur.node;
        pq.push({e.to, dist[e.to]});
      }
    }
  }
  if (dist[dest_node]==INF) return std::nullopt;
  std::vector<PathStep> path; int cur = dest_node;
  while (cur!=origin_node) {
    int pe = prev_edge[cur];
    path.push_back({pe});
    cur = prev_node[cur];
  }
  std::reverse(path.begin(), path.end());
  return path;
}

}
