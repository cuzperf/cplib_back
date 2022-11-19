#include "cut.h"

#include <functional>

namespace cuzperf {

std::vector<int> cutVertex(std::vector<std::vector<int>>& e) {
  int n = (int)e.size(), cnt = 0;
  std::vector<int> dfs(n), low(n), flag(n), r;
  std::function<void(int, int)> Tarjan = [&](int u, int fa) -> void {
    low[u] = dfs[u] = ++cnt;
    int ch = 0;
    for (auto v : e[u]) {
      if (dfs[v] == 0) {
        ++ch;
        Tarjan(v, u);
        low[u] = std::min(low[u], low[v]);
        if (u != fa && low[v] >= dfs[u]) {
          flag[u] = 1;
        }
      } else if (v != fa) {
        low[u] = std::min(low[u], dfs[v]);
      }
    }
    if (u == fa && ch > 1) {
      flag[u] = 1;
    }
  };
  for (int i = 0; i < n; ++i) {
    if (dfs[i] == 0) {
      Tarjan(i, i);
    }
  }
  for (int i = 0; i < n; ++i) {
    if (flag[i]) {
      r.emplace_back(i);
    }
  }
  return r;
}

}  // namespace cuzperf
