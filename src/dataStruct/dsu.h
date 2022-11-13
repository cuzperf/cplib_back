#pragma once

#include <vector>
#include <numeric>

namespace cuzperf {

// Disjoint Set Union
class DSU {
  std::vector<int> p_;
  // std::vector<int> sz_;
 public:
  DSU(int n) : p_(n) { std::iota(p_.begin(), p_.end(), 0); }
  int find(int x) {
    return x == p_[x] ? x : p_[x] = find(p_[x]);
    // non recursive version
    // int now = x;
    // while (now != p[now]) now = p[now];
    // while (x != now) {
    //   auto px = p[x];
    //   p[x] = now;
    //   x = px;
    // }
    // return x;

    // other impl not recommend
    // while (x != p_[x]) x = p_[x] = p_[p_[x]];
    // return x;
  }
  bool merge(int x, int y) {
    int px = find(x), py = find(y);
    if (px == py) return false;
    // do something, small to big;
    return true;
  }
};

}  // namespace cuzperf
