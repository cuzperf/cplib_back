#include "mix.h"

#include <numeric>
#include <iostream>
#include <vector>

#include "basic.hpp"
#include "fenwick.hpp"

namespace cuzperf {

// a will becomes next lexicographical order of a, satisfies $-1 < a_0 < a_1 < \cdots, a_{n - 1} < mx$
bool nextBinom(std::vector<int>& a, int mx) {
  int n = (int)a.size(), i = 1;
  while (i <= n && a[n - i] == mx - i) ++i;
  if (i > n) return false;
  ++a[n - i];
  for (int j = 1; j < i; ++j) {
    a[n - j] = a[n - i] + (i - j);
  }
  return true;
}

// total number binom{mx}{n}
void bruteForceBinom(int n, int mx) {
  std::vector<int> a(n);
  std::iota(a.begin(), a.end(), 0);
  do {
    // do something
    for (auto x : a) std::cout << x << ' ';
    std::cout << '\n';
  } while (nextBinom(a, mx));
}

// length of longest increasing subsquence
int LIS(std::vector<int>& a) {
  std::vector<int> b;
  for (auto x : a) {
    if (auto it = std::lower_bound(b.begin(), b.end(), x); it == b.end()) {
      b.emplace_back(x);
    } else {
      *it = x;
    }
  }
  return b.size();
}
// length of longest non-decreasing subsquence
int LNDS(std::vector<int>& a) {
  std::vector<int> b;
  for (auto x : a) {
    if (auto it = std::upper_bound(b.begin(), b.end(), x); it == b.end()) {
      b.emplace_back(x);
    } else {
      *it = x;
    }
  }
  return b.size();
}

std::stack<int> LISP(const std::vector<int>& a) {
  int n = a.size();
  std::vector<int> b;
  b.reserve(n);
  // p[i] means the preview number of choosen i
  std::vector<int> p(n);
  std::iota(p.begin(), p.end(), 0);
  // q[i] means current b[i] is a[q[i]]
  std::vector<int> q;
  q.reserve(n);
  for (int i = 0; i < n; ++i) {
    auto it = std::upper_bound(b.begin(), b.end(), a[i]);
    if (it == b.end()) {
      if (!q.empty()) p[i] = q.back();
      b.emplace_back(a[i]);
      q.emplace_back(i);
    } else {
      *it = a[i];
      auto t = it - b.begin();
      q[t] = i;
      if (t > 0) p[i] = q[t - 1];
    }
  }
  std::stack<int> c;
  int now = q.back();
  c.push(now);
  while (now != p[now]) {
    now = p[now];
    c.push(now);
  }
  return c;
}

// monicDeque: index of every max element of SubInterval of length m
std::vector<int> monicDequeMax(std::vector<int>& a, int m) {
  std::vector<int> r;
  std::deque<int> Q;
  for (int i = 0, na = (int)a.size(); i < na; ++i) {
    if (!Q.empty() && i - Q.front() >= m) Q.pop_front();
    // change > to < if you want monicDequeMin
    while (!Q.empty() && a[i] > a[Q.back()]) Q.pop_back();
    Q.push_back(i);
    if (i >= m - 1) r.emplace_back(Q.front());
  }
  return r;
}

// f is index of a such that $a_{f_0} < a_{f_1} < a_{f_m}$
std::vector<int> monicStack(const std::vector<int>& a) {
  int n = (int)a.size();
  std::vector<int> f(n);
  std::stack<int> S;
  for (int i = 0; i < n; ++i) {
    while (!S.empty() && a[S.top()] < a[i]) {
      f[S.top()] = i;
      S.pop();
    }
    S.push(i);
  }
  return f;
}
// https://www.luogu.com.cn/problem/P5788

int cartesian_build(std::vector<cNode>& tree, int n) {
  for (int i = 1; i <= n; ++i) {
    int k = i - 1;
    while (tree[k].val < tree[i].val) k = tree[k].par;
    tree[i].ch[0] = tree[k].ch[1];
    tree[k].ch[1] = i;
    tree[i].par = k;
    tree[tree[i].ch[0]].par = i;
  }
  return tree[0].ch[1];
}
// https://codeforces.com/contest/1490/problem/D

// we can only use the ideal of merge sort
int64_t inverseOrderCount(std::vector<int> a) {
  discrete(a);
  Fenwick<int> A(*std::max_element(a.begin(), a.end()) + 1);
  int64_t ans = 0;
  for (int i = (int)a.size() - 1; i >= 0; --i) {
    ans += A.sum(a[i]);
    A.add(a[i] + 1, 1);
  }
  return ans;
}
// https://codeforces.com/contest/1602/problem/E

}  // namespace cuzperf
