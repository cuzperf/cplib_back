#pragma once

#include <map>
#include <vector>

#include "base/builtin.h"

template <typename T>
struct Fenwick {
  static inline int lowbit(int n) { return n & (-n); }
  int n_;
  std::vector<T> s_;
  Fenwick() {}
  Fenwick(int n) : n_(n), s_(n + 1) {}
  // https://codeforces.com/blog/entry/59305
  Fenwick(const std::vector<T>& a) : n_(a.size()), s_(n_ + 1) {
    for (int i = 0; i < n_; ++i) {
      s_[i + 1] = s_[i] + a[i];
    }
    for (int i = n_; i > 0; --i) {
      s_[i] -= s_[i - lowbit(i)];
    }
  }
  std::vector<T> getOrigin() const {
    auto a = s_;
    for (int i = 1; i <= n_; ++i) {
      a[i] += a[i - lowbit(i)];
    }
    std::vector<T> ans(n_);
    for (int i = n_ - 1; i >= 0; --i) {
      ans[i] = a[i + 1] - a[i];
    }
    return ans;
  }
  void add(int id, T p) {
    for (int i = id + 1; i <= n_; i += lowbit(i)) {
      s_[i] += p;
    }
  }
  // [0, id)
  T sum(int id) {
    T r = 0;
    for (int i = std::min(id, n_); i > 0; i -= lowbit(i)) {
      r += s_[i];
    }
    return r;
  }
  // [l, r)
  T sum(int l, int r) { return sum(r) - sum(l); }
  // find minimal index s.t. sum(id) >= x, sum must be increased
  int search(T val) {
    T sum = 0;
    int id = 0;
    for (int i = lg2_u32(s_.size()); ~i; --i) {
      if (id + (1 << i) <= n_ && sum + s_[id + (1 << i)] < val) {
        id += (1 << i);
        sum += s_[id];
      }
    }
    return ++id;
  }
};
// see https://codeforces.com/contest/1635/submission/147077087 for more use

template <typename T>
class FenwickPlus {
  int n_;
  // c[i] = a[i] - a[i - 1], b_i = (i - 1) * c_i
  // c[0] = a[0], b[0] = 0
  // c[i] = a[i] - a[i - 1], b[i] = i * c[i]
  // a[i] = c[0] + ... + c[i]
  // a[0] + ... + a[i - 1] = i c[0] + (i - 1) c[1] + ... + c[i - 1]
  //     = i * (c[0] + c[1] + ... c[i - 1]) - (b[0] + b[1] + b[i - 1])
  Fenwick<T> B, C;
  void add(int id, T p) {
    C.add(id, p);
    B.add(id, (id - 1) * p);
  }

 public:
  FenwickPlus() {}
  FenwickPlus(int n) : n_(n), B(n), C(n) {}
  void add(int l, int r, T p) {
    add(l, p);
    add(r + 1, -p);
  }
  T sum(int id) { return id * C.sum(id) - B.sum(id); }
  T sum(int l, int r) { return sum(r) - sum(l); }
  // find minimal index s.t. sum(id) >= x, sum must be increased
  int search(T val) {
    T sumB = 0, sumC = 0;
    int id = 0;
    for (int i = lg2_u32(n_); ~i; --i) {
      int idi = id + (1 << i);
      if (idi <= n_ && idi * (sumC + C.s[idi]) - B.s[idi] - sumB < val) {
        id = idi;
        sumB += B.s[id];
        sumC += C.s[id];
      }
    }
    return ++id;
  }
};

// 2D-Fenwick tree using map
struct Fenwick2M {
  static inline int lowbit(int n) { return n & (-n); }
  Fenwick2M() {}
  Fenwick2M(int n, int m) : n_(n), m_(m), mp_(n + 1) {}
  void add(int x, int y, int p) {
    for (int i = x; i <= n_; i += lowbit(i)) {
      auto& a = mp_[i];
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
      auto& a = mp_[i];
      if (a.empty()) {
        continue;
      }
      for (int j = y; j > 0; j -= lowbit(j)) {
        if (a.count(j)) {
          ans += a[j];
        }
      }
    }
    return ans;
  }
  int sum(int x1, int y1, int x2, int y2) {
    --x1;
    --y1;
    return sum(x2, y2) - sum(x2, y1) - sum(x1, y2) + sum(x1, y1);
  }
  int n_, m_;
  std::vector<std::map<int, int>> mp_;
};

// 2D-Fenwick tree using vector
struct Fenwick2V {
  static inline int lowbit(int n) { return n & (-n); }
  Fenwick2V() {}
  Fenwick2V(int n, int m) : n_(n), m_(m), mp_(n + 1, std::vector<int>(m + 1)) {}
  void add(int x, int y, int p) {
    for (int i = x; i <= n_; i += lowbit(i)) {
      auto& a = mp_[i];
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
      auto& a = mp_[i];
      for (int j = y; j > 0; j -= lowbit(j)) {
        ans += a[j];
      }
    }
    return ans;
  }
  int sum(int x1, int y1, int x2, int y2) {
    --x1;
    --y1;
    return sum(x2, y2) - sum(x2, y1) - sum(x1, y2) + sum(x1, y1);
  }
  int n_, m_;
  std::vector<std::vector<int>> mp_;
};
