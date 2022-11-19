#pragma once

#include <vector>

namespace cuzperf {

// Kosaraju: Strongly Connected Components
class Scc {
  int n_, nScc_;
  std::vector<int> vis_, color_, order_;
  std::vector<std::vector<int>> e_, e2_;
  void dfs(int u) {
    vis_[u] = true;
    for (auto v : e_[u]) if (!vis_[v]) dfs(v);
    order_.emplace_back(u);
  }
  void dfs2(int u) {
    color_[u] = nScc_;
    for (auto v : e2_[u]) if (!color_[v]) dfs2(v);
  }
  void Kosaraju() {
    for (int i = 0; i < n_; ++i) if (!vis_[i]) dfs(i);
    for (auto it = order_.rbegin(); it != order_.rend(); ++it) if (!color_[*it]) {
      ++nScc_;
      dfs2(*it);
    }
  }
 public:
  Scc(int n) : n_(n) {
    nScc_ = 0;
    e_.resize(n_);
    e2_.resize(n_);
    vis_.resize(n_);
    color_.resize(n_);
  }
  void addEdge(int u, int v) {
    nScc_ = 0;
    e_[u].emplace_back(v);
    e2_[v].emplace_back(u);
  }
  int solve() {
    if (nScc_ == 0) Kosaraju();
    return nScc_;
  }
};

// n_ / 2 pairs (2i, 2i + 1)
struct twoSAT {
  int n_, nScc_;
  std::vector<int> vis_, color_, order_;
  std::vector<std::vector<int>> e_, e2_;
  twoSAT(int n) : n_(n * 2) {
    nScc_ = 0;
    e_.resize(n_);
    e2_.resize(n_);
    vis_.resize(n_);
    color_.resize(n_);
  }
  void addEdge(int u, int v) {
    e_[u].emplace_back(v);
    e2_[v].emplace_back(u);
  }
  void dfs(int u) {
    vis_[u] = true;
    for (auto v : e_[u]) if (!vis_[v]) dfs(v);
    order_.emplace_back(u);
  }
  void dfs2(int u) {
    color_[u] = nScc_;
    for (auto v : e2_[u]) if (!color_[v]) dfs2(v);
  }
  void Kosaraju() {
    for (int i = 0; i < n_; ++i) if (!vis_[i]) dfs(i);
    for (auto it = order_.rbegin(); it != order_.rend(); ++it) if (!color_[*it]) {
      ++nScc_;
      dfs2(*it);
    }
  }
  std::vector<int> solve() {
    Kosaraju();
    // choose component with max color_ number
    std::vector<int> choose(nScc_ + 1);
    for (int i = 0; i < n_; i += 2) {
      int c1 = color_[i], c2 = color_[i + 1];
      if (c1 == c2) return std::vector<int>();
      if (choose[c1] || choose[c2]) continue;
      choose[std::max(c1, c2)] = 1;
    }
    std::vector<int> r(n_ / 2);
    for (int i = 0; i * 2 < n_; ++i) r[i] = (choose[color_[i * 2]] ? 1 : -1);
    return r;
  }
};

}  // namespace cuzperf
