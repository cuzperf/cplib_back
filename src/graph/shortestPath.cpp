#include "shortestPath.h"

#include <stdint.h>
#include <string>
#include <queue>

namespace cuzperf {

namespace Floyd {
constexpr int N = 1003;
int64_t dp[N][N], path[N][N];
void Floyd(int n);
std::vector<int> getPath(int x, int y);
} // namespace Floyd

std::vector<int64_t> Dijkstra(const std::vector<edge>& e, int s) {
  std::priority_queue<std::pair<int64_t, int>> Q;
  std::vector<int64_t> d(e.size(), std::numeric_limits<int64_t>::min());
  d[s] = 0;
  Q.push({0, s});
  while (!Q.empty()) {
    auto [du, u] = Q.top();
    Q.pop();
    if (d[u] != -du) continue;
    for (auto [v, w] : e[u]) if (d[v] > d[u] + w) {
      d[v] = d[u] + w;
      Q.emplace(-d[v], v);
    }
  }
  return d;
}

bool BellmanFord(std::vector<Edge>& e, int n, int s) {
  std::vector<int> dist(n + 1, std::numeric_limits<int>::max());
  dist[s] = 0;
  for (int i = 0; i <= n; ++i) {
    bool judge = false;
    for (auto [u, v, w] : e) if (dist[u] != std::numeric_limits<int>::max()) {
      if (dist[v] > dist[u] + w) {
        dist[v] = dist[u] + w;
        judge = true;
      }
    }
    if (!judge) return true;
  }
  return false;
}

bool spfa(std::vector<edge>& e, int s) {
  int n = (int)e.size();
  std::queue<int> Q;
  std::vector<int> dist(n, std::numeric_limits<int>::max()), cnt(n), inQ(n);
  Q.push(s);
  inQ[s] = 1;
  dist[s] = 0;
  ++cnt[s];
  while (!Q.empty()) {
    int u = Q.front();
    Q.pop();
    inQ[u] = 0;
    for (auto [v, w]: e[u]) {
      if (dist[v] > dist[u] + w) {
        dist[v] = dist[u] + w;
        if (!inQ[v]) {
          Q.push(v);
          inQ[v] = 1;
          if (++cnt[v] == n) return false;
        }
      }
    }
  }
  return true;
}

}  // namespace cuzperf
