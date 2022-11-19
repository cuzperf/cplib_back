#pragma once

#include <stdint.h>
#include <utility>
#include <vector>

namespace cuzperf {

namespace Geomerty {
using Point = std::pair<double, double>;
double cross(const Point& op, const Point& sp, const Point& ep);
bool crossLeft(const Point& op, const Point& sp, const Point& ep);
double dist2(const Point& p, const Point& q);
double dist(const Point& p, const Point& q);

std::vector<Point> convexHull(std::vector<Point> p);
double diameter(std::vector<Point> p);
// float version: https://www.luogu.com.cn/problem/P6247
// Int version: https://www.luogu.com.cn/problem/P1452

double minDist(std::vector<Point> a);
// https://www.luogu.com.cn/problem/P1429
} // namespace Geomerty

// a is k * n matrix: has n k-dimension vectors, return r[i]: number of vector less than i
std::vector<int> partialOrder(std::vector<std::vector<int>>& a);
// http://cogs.pro:8081/cogs/problem/problem.php?pid=vSJzQVejP

namespace rectangleUnion {
class SegTree {
  static inline const int INF = 1e9 + 2;
  struct Node {
    int l = 0, r = 0, val = 0, mn = 0;
  };
  std::vector<Node> tree;
  int newNode() {
    int sz = tree.size();
    tree.emplace_back(Node());
    return sz;
  }
  void pushUp(int p) {
    tree[p].mn = std::min(tree[tree[p].l].mn, tree[tree[p].r].mn);
  }
  void updateNode(int p, int x) {
    tree[p].val += x;
    tree[p].mn += x;
  }
  void pushDown(int p) {
    if (tree[p].l) updateNode(tree[p].l, tree[p].val);
    if (tree[p].r) updateNode(tree[p].r, tree[p].val);
    tree[p].val = 0;
  }
  void add(int L, int R, int val, int l, int r, int p) {
    if (L <= l && r <= R) {
      updateNode(p, val);
      return;
    }
    auto m = (l + r) / 2;
    if (m > L) {
      if (tree[p].l == 0) tree[p].l = newNode();
      add(L, R, val, l, m, tree[p].l);
    }
    if (m < R) {
      if (tree[p].r == 0) tree[p].r = newNode();
      add(L, R, val, m, r, tree[p].r);
    }
  }
  int query(int l, int r, int p) {
    if (tree[p].mn) return r - l;
    auto m = (l + r) / 2;
    int ans = 0;
    if (tree[p].l) ans += query(l, m, tree[p].l);
    if (tree[p].r) ans += query(m, r, tree[p].r);
    return ans;
  }
 public:
  SegTree() {
    newNode();
  }
  void add(int L, int R, int val) {
    add(L, R, val, -INF, INF, 0);
  }
  int query() {
    return query(-INF, INF, 0);
  }
};

struct Edge {
  int x, l, r, val;
  bool operator<(const Edge& A) const {
    return x < A.x;
  }
};
int64_t rectangleUnion(const std::vector<std::tuple<int, int, int, int>>& rectangle);
// https://www.luogu.com.cn/problem/T110664

} // namespace rectangleUnion

}  // namespace cuzperf
