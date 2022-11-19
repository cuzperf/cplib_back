#pragma once

#include <vector>

namespace cuzperf {

std::vector<int> cutVertex(std::vector<std::vector<int>>& e);
// https://www.luogu.com.cn/problem/P3388

class CutEdge {
  int n_, cnt_;
  std::vector<std::vector<int>> g_;
  std::vector<int> e, flag, dfs, low;
  void Tarjan(int u, int inEdgeNum) {
    low[u] = dfs[u] = ++cnt_;
    for (auto i : g_[u]) {
      int v = e[i];
      if (dfs[v] == 0) {
        Tarjan(v, i);
        low[u] = std::min(low[u], low[v]);
        if (low[v] > dfs[u]) {
          flag[i] = flag[i ^ 1] = 1;
        }
      } else if ((i ^ 1) != inEdgeNum) {
        low[u] = std::min(low[u], dfs[v]);
      }
    }
  }

 public:
  CutEdge(int n) : n_(n), cnt_(0), g_(n), dfs(n), low(n) {}
  void addEdge(int u, int v) {
    if (u == v) {
      return;
    }
    g_[u].emplace_back(e.size());
    e.emplace_back(v);
    flag.emplace_back(0);
    g_[v].emplace_back(e.size());
    e.emplace_back(u);
    flag.emplace_back(0);
  }
  int solve() {
    for (int i = 0; i < n_; ++i) {
      if (dfs[i] == 0) {
        Tarjan(i, -1);
      }
    }
    int r = 0;
    for (auto x : flag) {
      r += x;
    }
    return r / 2;
  }
};
// https://www.luogu.com.cn/problem/T103481

}  // namespace cuzperf
