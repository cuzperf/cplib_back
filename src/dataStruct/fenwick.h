#pragma once

#include <vector>
#include <map>
#include "../math/builtin.h"

namespace cuzperf {

template<typename T>
struct Bitree {
  static int lowbit(int n) { return n & (-n); }
  std::vector<T> s_;
  int n_;
  Bitree() {}
  Bitree(int n) : n_(n), s_(n) {}
  // https://codeforces.com/blog/entry/59305
  Bitree(const std::vector<T>& a) : s_(a) {
    n_ = s_.size();
    for (int i = n_; i > 1; --i) s_[i - 1] -= s_[i - lowbit(i) - 1];
  }
  std::vector<T> getOrigin() const {
    auto a = s_;
    for (int i = 2; i <= n_; ++i) a[i - 1] += a[i - lowbit(i) - 1];
    for (int i = n_ - 1; i > 0; --i) a[i] -= a[i - 1];
    return a;
  }
  void add(int id, T p) {
    for (int i = id; i <= n_; i += lowbit(i)) {
      s_[i - 1] += p;
    }
  }
  // sum of [0, id)
  T sum(int id) {
    T r = 0;
    for (int i = id; i; i -= lowbit(i)) {
      r += s_[i - 1];
    }
    return r;
  }
  // sum of [l, r]
  T sum(int l, int r) { return sum(r) - sum(l); }
  T at(int id) { return sum(id, id + 1);}
  // find minimal index s.t. sum(id) >= x, sum must be increased
  int search(T val) {
    T sum = 0;
    int id = 0;
    for (int i = lg32(n_); ~i; --i) {
      if (id + (1 << i) <= n_ && sum + s_[id + (1 << i) - 1] < val) {
        id += (1 << i);
        sum += s_[id - 1];
      }
    }
    return ++id;
  }
};
// see https://codeforces.com/contest/1635/submission/147077087 for more use

template<typename T>
class BitreePlus {
  int n_;
  // c[i] = a[i] - a[i - 1], b_i = (i - 1) * c_i
  Bitree<T> B, C;
  void add(int id, T p) {
    C.add(id, p);
    B.add(id, (id - 1) * p);
  }
 public:
  BitreePlus() {}
  BitreePlus(int n) : n_(n), B(n), C(n) {}
  void add(int l, int r, T p) {
    add(l, p);
    add(r + 1, -p);
  }
  T sum(int id) { return id * C.sum(id) - B.sum(id); }
  T sum(int l, int r) { return sum(r) - sum(l - 1); }
  T at(int id) { return sum(id, id); }
  // val must monic increase
  int lower_bound(T x) {
    int l = 1, r = n_;
    while (l <= r) {
      int m = (l + r) / 2;
      if (at(m) >= x) r = m - 1;
      else l = m + 1;
    }
    return l;
  }
  // val must monic increase
  int upper_bound(T x) {
    int l = 1, r = n_;
    while (l <= r) {
      int m = (l + r) / 2;
      if (at(m) > x) r = m - 1;
      else l = m + 1;
    }
    return l;
  }
  // find minimal index s.t. sum(id) >= x, sum must be increased
  int search(T val) {
    T sumB = 0, sumC = 0;
    int id = 0;
    for (int i = lg32(n_); ~i; --i)
      if (int idi = id + (1 << i); idi <= n_) {
        if (idi * (sumC + C.s[idi]) - B.s[idi] - sumB < val) {
          id = idi;
          sumB += B.s[id];
          sumC += C.s[id];
        }
      }
    return ++id;
  }
};

struct Bitree2M {
  static inline int lowbit(int n) { return n & (-n); }
  Bitree2M() {}
  Bitree2M(int n, int m) : n_(n), m_(m), mp_(n + 1) {}
  void add(int x, int y, int p) {
    for (int i = x; i <= n_; i += lowbit(i)) {
      auto &a = mp_[i];
      for (int j = y; j <= m_; j += lowbit(j)) {
        a[j] += p;
      }
    }
  }
  int sum(int x, int y) {
    x = std::min(x, n_);
    y = std::min(y, m_);
    int ans = 0;
    for (int i = x; i > 0; i -= lowbit(i)) {
      auto &a = mp_[i];
      if (a.empty()) continue;
      for (int j = y; j > 0; j -= lowbit(j)) {
        if (a.count(j)) {
          ans += a[j];
        }
      }
    }
    return ans;
  }
  int sum(int x1, int y1, int x2, int y2) {
    --x1; --y1;
    return sum(x2, y2) - sum(x2, y1) - sum(x1, y2) + sum(x1, y1);
  }
  int n_, m_;
  std::vector<std::map<int, int>> mp_;
};

struct Bitree2V {
  static inline int lowbit(int n) { return n & (-n); }
  Bitree2V() {}
  Bitree2V(int n, int m) : n_(n), m_(m), mp_(n + 1, std::vector<int>(m + 1)) {}
  void add(int x, int y, int p) {
    for (int i = x; i <= n_; i += lowbit(i)) {
      auto &a = mp_[i];
      for (int j = y; j <= m_; j += lowbit(j)) {
        a[j] += p;
      }
    }
  }
  int sum(int x, int y) {
    x = std::min(x, n_);
    y = std::min(y, m_);
    int ans = 0;
    for (int i = x; i > 0; i -= lowbit(i)) {
      auto &a = mp_[i];
      for (int j = y; j > 0; j -= lowbit(j)) {
        ans += a[j];
      }
    }
    return ans;
  }
  int sum(int x1, int y1, int x2, int y2) {
    --x1; --y1;
    return sum(x2, y2) - sum(x2, y1) - sum(x1, y2) + sum(x1, y1);
  }
  int n_, m_;
  std::vector<std::vector<int>> mp_;
};
}  // namespace cuzperf
