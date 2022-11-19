#include "tour.h"

#include <functional>
#include <queue>

namespace cuzperf {

std::vector<int> EulerTour(std::vector<std::vector<int>>& e, int rt) {
  std::vector<int> r;
  std::function<void(int, int)> dfs = [&](int u, int fa) -> void {
    r.emplace_back(u);
    for (auto v : e[u]) {
      if (v != fa) {
        dfs(v, u);
        r.emplace_back(u);
      }
    }
  };
  dfs(rt, rt);
  return r;
}

std::stack<int> EulerPathS(std::vector<std::multiset<int>> e) {
  int cnt = std::count_if(e.begin(), e.end(), [](auto x) { return x.size() & 1; });
  if (cnt > 2) {
    return std::stack<int>();
  }
  std::stack<int> ans;
  std::function<void(int)> Hierholzer = [&](int u) {
    while (!e[u].empty()) {
      int v = *e[u].begin();
      e[u].erase(e[u].begin());
      e[v].erase(e[v].find(u));
      Hierholzer(v);
    }
    ans.push(u);
  };
  for (int i = 0, ne = (int)e.size(); i < ne; ++i) {
    if (!e[i].empty() && ((e[i].size() & 1) || (cnt == 0))) {
      Hierholzer(i);
      break;
    }
  }
  return ans;
}

std::stack<int> EulerPath(std::vector<std::set<int>> e, int rt) {
  std::stack<int> ans;
  std::function<void(int)> Hierholzer = [&](int u) {
    while (!e[u].empty()) {
      int v = *e[u].begin();
      e[u].erase(e[u].begin());
      e[v].erase(e[v].find(u));
      Hierholzer(v);
    }
    ans.push(u);
  };
  Hierholzer(rt);
  return ans;
}

std::vector<int> TopSort(std::vector<std::set<int>>& e) {
  std::vector<int> r;
  std::priority_queue<int> Q;
  int n = (int)e.size();
  std::vector<int> in(n);
  for (auto& x : e) {
    for (auto i : x) {
      ++in[i];
    }
  }
  for (int i = 0; i < n; ++i) {
    if (in[i] == 0) {
      Q.push(-i);
    }
  }
  while (!Q.empty()) {
    int u = -Q.top();
    r.emplace_back(u);
    Q.pop();
    for (auto v : e[u]) {
      if (--in[v] == 0) {
        Q.push(-v);
      }
    }
  }
  return r;
}

}  // namespace cuzperf
