#include "graph/miniSpanTree.h"

#include <limits>
#include <queue>
#include <tuple>

namespace cuzperf {

// Minimum Spanning Tree
int64_t Prim(const std::vector<edge>& e) {
  int64_t r = 0;
  int n = (int)e.size(), cnt = 0;
  std::priority_queue<std::pair<int, int>> Q;
  std::vector<int> vis(n);
  Q.push({0, 0});
  while (!Q.empty()) {
    auto [w, u] = Q.top();
    Q.pop();
    if (vis[u]) {
      continue;
    }
    ++cnt;
    r -= w;
    vis[u] = 1;
    for (auto [v, c] : e[u]) {
      if (!vis[v]) {
        Q.push({-c, v});
      }
    }
  }
  return cnt == n ? r : std::numeric_limits<int64_t>::min();
}

// LiuZhu: Minimum tree diagram
int64_t LiuZhu(std::vector<Edge> e, int n, int rt) {  // e has no self-loop
  int64_t ans = 0;
  while (1) {
    std::vector<int> in(n, std::numeric_limits<int>::max()), pre(n, -1);
    for (auto [u, v, w] : e) {
      if (u != v && in[v] > w) {
        in[v] = w;
        pre[v] = u;
      }
    }
    for (int i = 0; i < n; ++i) {
      if (i != rt && pre[i] == -1) {
        return -1;
      }
    }
    // judge if there is a loop
    int cnt = 0;
    std::vector<int> vis(n, -1), id(n, -1);
    for (int i = 0; i < n; ++i) {
      if (i != rt) {
        ans += in[i];
        int v = i;
        // note there may be a path_ of form '6'
        while (vis[v] != i && id[v] == -1 && v != rt) {
          vis[v] = i;
          v = pre[v];
        }
        if (id[v] == -1 && v != rt) {
          int u = v;
          do {
            id[u] = cnt;
            u = pre[u];
          } while (u != v);
          ++cnt;
        }
      }
    }
    if (cnt == 0) {
      break;
    }
    // update nodes and edges
    for (int i = 0; i < n; ++i) {
      if (id[i] == -1) {
        id[i] = cnt++;
      }
    }
    for (auto& [u, v, w] : e) {
      if (id[u] != id[v]) {
        w -= in[v];
      }
      u = id[u];
      v = id[v];
    }
    rt = id[rt];
    n = cnt;
  }
  return ans;
}

}  // namespace cuzperf
