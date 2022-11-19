#include "circleCount.h"

#include <algorithm>
#include <numeric>

namespace cuzperf {

int circle3count(const std::vector<std::pair<int, int>>& edge, int n) {
  std::vector<int> d(n), vis(n, -1);
  for (auto [u, v] : edge) {
    ++d[u], ++d[v];
  }
  std::vector<std::vector<int>> e(n);
  // Giving Orienting to Edge
  for (auto [u, v] : edge) {
    if (d[u] < d[v] || (d[u] == d[v] && u < v)) {
      e[u].emplace_back(v);
    } else {
      e[v].emplace_back(u);
    }
  }
  int ans = 0;
  for (int i = 0; i < n; ++i) {
    for (auto u : e[i]) {
      vis[u] = i;
    }
    for (auto u : e[i]) {
      for (auto v : e[u]) {
        if (vis[v] == i) {
          ++ans;
        }
      }
    }
  }
  return ans;
}

int64_t circle4count(const std::vector<std::pair<int, int>>& edge, int n) {
  std::vector<int> d(n), c(n, -1), id(n);
  for (auto [u, v] : edge) {
    ++d[u], ++d[v];
  }
  std::iota(id.begin(), id.end(), 0);
  std::sort(id.begin(), id.end(),
            [&](int i, int j) { return d[i] < d[j] || (d[i] == d[j] && i < j); });
  std::vector<std::vector<int>> e(n);
  for (auto [u, v] : edge) {
    e[u].emplace_back(v);
    e[v].emplace_back(u);
  }
  // x -> y -> z and x -> w -> z
  int64_t ans = 0;
  for (int i = 0; i < n; ++i) {
    for (auto u : e[i]) {
      if (id[i] < id[u]) {
        for (auto v : e[u]) {
          if (id[i] < id[v]) {
            ans += c[v]++;
          }
        }
      }
    }
    for (auto u : e[i]) {
      if (id[i] < id[u]) {
        for (auto v : e[u]) {
          if (id[i] < id[v]) {
            c[v] = 0;
          }
        }
      }
    }
  }
  return ans;
}

}  // namespace cuzperf
