#pragma once

#include <set>
#include <stack>
#include <vector>

namespace cuzperf {
// dfs order of rooted tree
class DfsTour {
  int n_, cnt_;
  std::vector<int> l_, r_;
  std::vector<std::vector<int>> e_;

 public:
  DfsTour(int n) : n_(n), cnt_(0), l_(n), r_(n), e_(n) {}
  void addEdge(int u, int v) {
    if (u == v) {
      return;
    }
    e_[u].emplace_back(v);
    e_[v].emplace_back(u);
  }
  void dfs(int u, int fa) {
    l_[u] = ++cnt_;
    for (auto v : e_[u]) {
      if (v != fa) {
        dfs(v, u);
      }
    }
    r_[u] = cnt_;
  }
};

// Euler Tour: rooted by rt(length 2n - 1)
std::vector<int> EulerTour(std::vector<std::vector<int>>& e, int rt);

// Euler Path: return lexicographical smallest one or empty(use set if there is no multi-edge)
std::stack<int> EulerPathS(std::vector<std::multiset<int>> e);
// Euler Path start form rt: return lexicographical smallest one(assume existence and no multi-edge)
std::stack<int> EulerPath(std::vector<std::set<int>> e, int rt);
// https://www.luogu.com.cn/problem/P2731


// Topological sorting: Kahn algorithm for DAG. return lexicographical smallest one
std::vector<int> TopSort(std::vector<std::set<int>>& e);
// https://www.luogu.com.cn/problem/U107394


}  // namespace cuzperf
