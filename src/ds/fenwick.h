#pragma once

#include <vector>
#include <map>
#include "../math/builtin.h"

namespace cuzperf {

template<typename T>
struct Fenwick {
  static int lowbit(int n) { return n & (-n); }
  std::vector<T> s_;
  int n_;
  Fenwick() {}
  Fenwick(int n) : n_(n), s_(n) {}
  // https://codeforces.com/blog/entry/59305
  Fenwick(const std::vector<T>& a) : s_(a) {
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
      if (int idi = id + (1 << i); idi <= n_ && sum + s_[idi - 1] < val) {
        id = idi;
        sum += s_[id - 1];
      }
    }
    return ++id;
  }
};
// see https://codeforces.com/contest/1635/submission/147077087 for more use

template<typename T>
class FenwickPlus {
  int n_;
  // c[0] = a[0], b[0] = 0
  // c[i] = a[i] - a[i - 1], b[i] = i * c[i]
  // a[i] = c[0] + ... + c[i]
  // a[0] + ... + a[i - 1] = i c[0] + (i - 1) c[1] + ... + c[i - 1]
  //     = i * (c[0] + c[1] + ... c[i - 1]) - (b[0] + b[1] + b[i - 1])
  Fenwick<T> B, C;
  void add(int id, T p) {
    C.add(id, p);
    B.add(id, id * p);
  }
 public:
  FenwickPlus() {}
  FenwickPlus(int n) : n_(n), B(n), C(n) {}
  void add(int l, int r, T p) {
    add(l, p);
    add(r, -p);
  }
  T sum(int id) { return id * C.sum(id) - B.sum(id); }
  T sum(int l, int r) { return sum(r) - sum(l); }
  // find minimal index s.t. sum(id) >= x, sum must be increased
  int search(T val) {
    T sumB = 0, sumC = 0;
    int id = 0;
    for (int i = lg32(n_); ~i; --i)
      if (int idi = id + (1 << i); idi <= n_ && idi * (sumC + C.s[idi - 1]) - B.s[idi] - sumB < val) {
        id = idi;
        sumB += B.s[id - 1];
        sumC += C.s[id - 1];
      }
    return ++id;
  }
};

// 2D-Fenwick tree using map
struct Fenwick2M {
  static inline int lowbit(int n) { return n & (-n); }
  Fenwick2M() {}
  Fenwick2M(int n, int m) : n_(n), m_(m), mp_(n) {}
  void add(int x, int y, int p) {
    for (int i = x; i <= n_; i += lowbit(i)) {
      auto &a = mp_[i - 1];
      for (int j = y; j <= m_; j += lowbit(j)) {
        a[j - 1] += p;
      }
    }
  }
  int sum(int x, int y) {
    x = std::min(x, n_);
    y = std::min(y, m_);
    int ans = 0;
    for (int i = x; i > 0; i -= lowbit(i)) {
      auto &a = mp_[i - 1];
      if (a.empty()) continue;
      for (int j = y; j > 0; j -= lowbit(j)) {
        if (a.count(j - 1)) {
          ans += a[j - 1];
        }
      }
    }
    return ans;
  }
  int sum(int x1, int y1, int x2, int y2) {
    return sum(x2, y2) - sum(x2, y1) - sum(x1, y2) + sum(x1, y1);
  }
  int n_, m_;
  std::vector<std::map<int, int>> mp_;
};

// 2D-Fenwick tree using vector
struct Fenwick2V {
  static inline int lowbit(int n) { return n & (-n); }
  Fenwick2V() {}
  Fenwick2V(int n, int m) : n_(n), m_(m), va_(n, std::vector<int>(m)) {}
  void add(int x, int y, int p) {
    for (int i = x; i <= n_; i += lowbit(i)) {
      auto &a = va_[i - 1];
      for (int j = y; j <= m_; j += lowbit(j)) {
        a[j - 1] += p;
      }
    }
  }
  int sum(int x, int y) {
    x = std::min(x, n_);
    y = std::min(y, m_);
    int ans = 0;
    for (int i = x; i > 0; i -= lowbit(i)) {
      auto &a = va_[i - 1];
      for (int j = y; j > 0; j -= lowbit(j)) {
        ans += a[j - 1];
      }
    }
    return ans;
  }
  int sum(int x1, int y1, int x2, int y2) {
    --x1; --y1;
    return sum(x2, y2) - sum(x2, y1) - sum(x1, y2) + sum(x1, y1);
  }
  int n_, m_;
  std::vector<std::vector<int>> va_;
};

}  // namespace cuzperf
