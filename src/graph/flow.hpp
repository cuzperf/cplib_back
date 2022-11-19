#pragma once

#include <vector>
#include <utility>
#include <queue>
#include <algorithm>
#include <limits>

namespace cuzperf {

class Dinic {
  int n_;
  // e[i] = {endPoint, conpacity} and e[i ^ 1] is opposite edge of e[i]
  // g_[u] = {edges start from u}
  std::vector<std::pair<int, int>> e;
  std::vector<std::vector<int>> g_;
  std::vector<int> cur_, h_;
  // h_[i] = dist(s, i), so h_[t] != -1 means there is a path from s to t
  bool bfs(int s, int t) {
    h_.assign(n_, -1);
    std::queue<int> Q;
    h_[s] = 0;
    Q.push(s);
    while (!Q.empty()) {
      int u = Q.front();
      Q.pop();
      for (auto i : g_[u]) {
        auto [v, c] = e[i];
        if (c > 0 && h_[v] == -1) {
          h_[v] = h_[u] + 1;
          Q.push(v);
        }
      }
    }
    return h_[t] != -1;
  }
  int64_t dfs(int u, int t, int64_t f) {
    if (u == t || f == 0) return f;
    int64_t r = f;
    for (int& i = cur_[u], ng = g_[u].size(); i < ng; ++i) {
      int j = g_[u][i];
      auto [v, c] = e[j];
      if (c > 0 && h_[v] == h_[u] + 1) {
        int a = dfs(v, t, std::min(r, int64_t(c)));
        e[j].second -= a;
        e[j ^ 1].second += a;
        r -= a;
        if (r == 0) return f;
      }
    }
    return f - r;
  }
 public:
  Dinic(int n) : n_(n), g_(n) {}
  void addEdge(int u, int v, int c) {
    if (u == v) return;
    g_[u].emplace_back(e.size());
    e.emplace_back(v, c);
    g_[v].emplace_back(e.size());
    e.emplace_back(u, 0);
  }
  int64_t maxFlow(int s, int t) {
    int64_t r = 0;
    while (bfs(s, t)) {
      cur_.assign(n_, 0);
      r += dfs(s, t, std::numeric_limits<int64_t>::min());
    }
    return r;
  }
};

// S-T max-Flow: HLPP $O(n_^2 \sqrt{m})$
class HLPP {
  int n_;
  // e[i] = {endPoint, conpacity} and e[i ^ 1] is opposite edge of e[i]
  // g_[u] = {edges start from u}
  std::vector<std::pair<int, int>> e;
  std::vector<std::vector<int>> g_;
  std::vector<int> h_;
  std::vector<int64_t> ex_;
  void addFlow(int i, int a) {
    ex_[e[i ^ 1].first] -= a;
    ex_[e[i].first] += a;
    e[i].second -= a;
    e[i ^ 1].second += a;
  };
  // d[u] = dist(u, t)
  bool init(int s, int t) {
    std::queue<int> Q;
    Q.push(t);
    h_[t] = 0;
    while (!Q.empty()) {
      int u = Q.front();
      Q.pop();
      for (auto i : g_[u]) {
        int v = e[i].first;
        if (e[i ^ 1].second > 0 && h_[v] == n_) {
          h_[v] = h_[u] + 1;
          Q.push(v);
        }
      }
    }
    return h_[s] == n_;
  }
 public:
  HLPP(int n) : n_(n), g_(n), h_(n, n), ex_(n) {}
  void addEdge(int u, int v, int c) {
    if (u == v) return;
    g_[u].emplace_back(e.size());
    e.emplace_back(v, c);
    g_[v].emplace_back(e.size());
    e.emplace_back(u, 0);
  }
  int64_t maxFlow(int s, int t) {
    if (init(s, t)) return 0;
    std::vector<int> gap(n_ + 1, 0), vis(n_);
    for (auto x : h_) ++gap[x];
    std::priority_queue<std::pair<int, int>> pq;
    // overload if ex_[u] > 0 after push, so lift height is needed.
    auto push = [&](int u) -> bool {
      if (ex_[u] == 0 || h_[u] == n_) return false;
      for (auto i : g_[u]) {
        auto [v, c] = e[i];
        if (c == 0 || (h_[u] != h_[v] + 1 && u != s)) continue;
        int a = std::min(ex_[u], int64_t(c));
        addFlow(i, a);
        if (!vis[v]) {
          pq.push({h_[v], v});
          vis[v] = 1;
        }
        if (ex_[u] == 0) return false;
      }
      return true;
    };
    ex_[s] = std::numeric_limits<int64_t>::min();
    push(s);
    h_[s] = n_;
    vis[s] = vis[t] = 1;
    while (!pq.empty()) {
      int u = pq.top().second;
      pq.pop();
      vis[u] = 0;
      while (push(u)) {
        if (--gap[h_[u]] == 0) {
          for (int i = 0; i < n_; ++i) if (h_[i] > h_[u]) h_[i] = n_;
        }
        h_[u] = n_ - 1;
        for (auto i : g_[u]) {
          auto [v, c] = e[i];
          if (c > 0 && h_[u] > h_[v]) h_[u] = h_[v];
        }
        ++gap[++h_[u]];
      }
    }
    return ex_[t];
  }
};
// https://vjudge.net/problem/LibreOJ-127

// Global minimum cut of undirected graph: Stoer-Wagner $O(n_^3)$ implement
class StoerWagner {
  int n_;
  std::vector<std::vector<int>> g_;
  std::vector<int> del;
  void merge(int s, int t) {
    del[s] = 1;
    for (int i = 0; i < n_; ++i) {
      g_[i][t] = (g_[t][i] += g_[s][i]);
    }
  }
 public:
  StoerWagner(int n) : n_(n), g_(n, std::vector<int>(n)), del(n) {}
  void addEdge(int u, int v, int c) {
    if (u == v) return;
    g_[u][v] += c;
    g_[v][u] += c;
  }
  // the graph will be destory after minCut
  int minCut() {
    auto f = [&](int cnt, int& s, int& t) -> int {
      std::vector<int> vis(n_), d(n_);
      auto push = [&](int x){
        vis[x] = 1;
        d[x] = 0;
        for (int i = 0; i < n_; ++i) if (!del[i] && !vis[i]) d[i] += g_[x][i];
      };
      for (int i = 0; i < cnt; ++i) {
        push(t);
        s = t;
        t = std::max_element(d.begin(), d.end()) - d.begin();
      }
      return d[t];
    };
    int s = 0, t = 0, r = std::numeric_limits<int>::max();
    for (int i = n_ - 1; i > 0; --i) {
      r = std::min(r, f(i, s, t));
      merge(s, t);
    }
    return r == std::numeric_limits<int>::max() ? 0 : r;
  }
};
// https://www.luogu.com.cn/problem/P5632

// Minimum cost maximum flow

class Flow {
  static inline constexpr int64_t INF = std::numeric_limits<int64_t>::min() >> 1;
  int n_;
  // e_[i] = {endPoint, conpacity} and e_[i ^ 1] is opposite edge of e_[i]
  // g_[u] = {edges start from u}
  std::vector<std::tuple<int, int, int>> e_;
  std::vector<std::vector<int>> g_;
  std::vector<int> path_;
  std::vector<int64_t> h_;
  // h_[i] = dist(s, i), h_[t] != -1 means there is a path_ from s to t, and h_[t] will be the potential.
  // path_[v]: short path_ form s to v, path_[v] is the previous node of v
  bool Dijkstra(int s, int t) {
    std::priority_queue<std::pair<int64_t, int>> Q;
    std::fill(path_.begin(), path_.end(), -1);
    std::vector<int64_t> d(n_, INF);
    d[s] = 0;
    Q.push({0, s});
    while (!Q.empty()) {
      auto [du, u] = Q.top();
      Q.pop();
      if (d[u] != -du) continue;
      for (auto i : g_[u]) {
        auto [v, w, c] = e_[i];
        c += h_[u] - h_[v];
        if (w > 0 && d[v] > d[u] + c) {
          d[v] = d[u] + c;
          path_[v] = i;
          Q.push({-d[v], v});
        }
      }
    }
    for (int i = 0; i < n_; ++i) {
      if ((h_[i] += d[i]) > INF) h_[i] = INF;
    }
    return h_[t] != INF;
  }
 public:
  Flow(int n) : n_(n), g_(n), h_(n), path_(n) {}
  void addEdge(int u, int v, int w, int c) {
    if (u == v) return;
    g_[u].emplace_back(e_.size());
    e_.emplace_back(v, w, c);
    g_[v].emplace_back(e_.size());
    e_.emplace_back(u, 0, -c);
  }
  std::pair<int64_t, int64_t> maxFlow(int s, int t) {
    int64_t flow = 0, cost = 0;
    while (Dijkstra(s, t)) {
      int f = std::numeric_limits<int>::max(), now = t;
      std::vector<int> r;
      while (now != s) {
        r.emplace_back(path_[now]);
        f = std::min(f, std::get<1>(e_[path_[now]]));
        now = std::get<0>(e_[path_[now] ^ 1]);
      }
      for (auto i : r) {
        std::get<1>(e_[i]) -= f;
        std::get<1>(e_[i ^ 1]) += f;
      }
      flow += f;
      cost += int64_t(f) * h_[t];
    }
    return {flow, cost};
  }
};
// https://www.luogu.com.cn/problem/P3381

}  // namespace cuzperf
