#pragma once

#include <vector>
#include <functional>

namespace cuzperf {

class LCA {
  int n_;
  std::vector<int> fa_, dep_, sz_, son_, top_;
 public:
  LCA(std::vector<std::vector<int>>& e, int rt = 1) : n_(e.size()) {
    fa_.resize(n_);
    dep_.resize(n_);
    sz_.resize(n_);
    son_.resize(n_);
    fa_[rt] = rt;
    dep_[rt] = 0;
    std::function<int(int)> pdfs = [&](int u) -> int {
      sz_[u] = 1;
      for (auto v : e[u]) if (v != fa_[u]) {
        dep_[v] = dep_[u] + 1;
        fa_[v] = u;
        sz_[u] += pdfs(v);
        if (sz_[v] > sz_[son_[u]]) son_[u] = v;
      }
      return sz_[u];
    };
    top_.resize(n_);
    std::function<void(int, int)> dfs = [&](int u, int t) -> void {
      top_[u] = t;
      if (son_[u] == 0) return;
      dfs(son_[u], t);
      for (auto v : e[u]) if (v != fa_[u] && v != son_[u]) dfs(v, v);
    };
    pdfs(rt);
    dfs(rt, rt);
  }
  int lca(int u, int v) {
    while (top_[u] != top_[v]) {
      if (dep_[top_[u]] > dep_[top_[v]]) {
        u = fa_[top_[u]];
      } else {
        v = fa_[top_[v]];
      }
    }
    return dep_[u] < dep_[v] ? u : v;
  }
};

}  // namespace cuzperf
