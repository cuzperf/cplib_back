#pragma once

#include <stdint.h>

#include <algorithm>
#include <vector>

#include "base/builtin.h"

namespace cuzperf {
static constexpr int G = 3, M = 998244353;  // 1 +  2^23 * 7 * 17

// 这里为了方便，用一维数组而非二维数组
// $roots[2^k + j] = e^{2\pi \mathrm{i} \frac{j}{2^k}}, 0 \leq j < 2^k$
static std::vector<int> roots_{0};
// $rev[2^k + j] = bitReverse(j)$ of length $k$, $0 \leq j < 2^k$
static std::vector<int> rev_{0, 0};

static unsigned powMod(unsigned x, unsigned n) {
  static const unsigned m = 998244353U;
  static const unsigned mr = 998244351U;
  static const unsigned m1 = 301989884U;
  static const unsigned m1inv = 232013824U;
  unsigned xx = (uint64_t(x) << 32) % m, rr = m1;
  while (n) {
    if (n & 1) {
      uint64_t t = uint64_t(rr) * xx;
      rr = (t + uint64_t(unsigned(t) * mr) * m) >> 32;
    }
    uint64_t t = uint64_t(xx) * xx;
    xx = (t + uint64_t(unsigned(t) * mr) * m) >> 32;
    n >>= 1;
  }
  return uint64_t(rr) * m1inv % m;
}
void init(int n) {
  int k = static_cast<int>(roots_.size());
  if (k < n) {
    roots_.resize(n);
    int e = powMod(G, (M - 1) / n);
    int n2 = n / 2;
    roots_[n2] = 1;
    for (int i = n2 + 1; i < n; ++i) {
      roots_[i] = 1LL * roots_[i - 1] * e % M;
    }
    for (int i = n2 - 1; i >= k; --i) {
      roots_[i] = roots_[i << 1];
    }
    rev_.resize(n * 2);
    for (k *= 2; k <= n; k *= 2) {
      for (int i = 0; i < k; i += 2) {
        rev_[k + i] = rev_[k + (i >> 1)] >> 1;
        rev_[k + i + 1] = (rev_[k + (i >> 1)] | k) >> 1;
      }
    }
  }
}

void dft(std::vector<int>& a) {
  int n = static_cast<int>(a.size());
  init(n);
  for (int i = 0; i < n; ++i) {
    if (rev_[n + i] < i) {
      std::swap(a[i], a[rev_[n + i]]);
    }
  }
  for (int k = 1; k < n; k *= 2) {
    for (int i = 0; i < n; i += k * 2) {
      for (int j = 0; j < k; ++j) {
        int u = a[i + j];
        int v = 1LL * a[i + j + k] * roots_[j + k] % M;
        int x = u + v, y = u - v;
        a[i + j] = x >= M ? x - M : x;
        a[i + j + k] = y < 0 ? y + M : y;
      }
    }
  }
}
void idft(std::vector<int>& a) {
  std::reverse(a.begin() + 1, a.end());
  dft(a);
  // note that n is power of 2, and M = 1 + c 2^x
  const int invN = M - (M - 1) / static_cast<int>(a.size());
  for (auto &x : a) {
    x = 1LL * x * invN % M;
  }
}

class PolyS : public std::vector<int> {
  void standard() {
    while (!empty() && !back()) {
      pop_back();
    }
  }
  void reverse() {
    std::reverse(begin(), end());
    standard();
  }

 public:
  PolyS() {}
  PolyS(const int& x) : std::vector<int>{(x % M + M) % M} { standard(); }
  PolyS(const std::vector<int>& a) : std::vector<int>{a} {
    standard();
    for (auto &x : *this) x = (x % M + M) % M;
  }
  PolyS(std::vector<int>&& a) : std::vector<int>(std::move(a)) { standard(); }
  int at(int id) const {
    if (id < 0 || id >= static_cast<int>(size())) {
      return 0;
    }
    return (*this)[id];
  }
  PolyS operator-() const {
    auto A = (*this);
    for (auto& x : A) {
      x = (x == 0 ? 0 : M - x);
    }
    return A;
  }
  PolyS mulXn(int n) const {
    auto A = *this;
    if (!A.empty()) {
      A.insert(A.begin(), n, 0);
    }
    return A;
  }
  PolyS modXn(int n) const {
    if (n >= static_cast<int>(size())) {
      return *this;
    }
    return PolyS({begin(), begin() + n});
  }
  PolyS modXnR(int n) {
    this->resize(n);
    return PolyS(std::move(*this));
  }
  PolyS divXn(int n) const {
    if (static_cast<int>(size()) <= n) {
      return PolyS();
    }
    return PolyS({begin() + n, end()});
  }
  PolyS& operator+=(const PolyS& rhs) {
    if (static_cast<int>(size()) < static_cast<int>(rhs.size())) {
      resize(rhs.size());
    }
    for (int i = 0, rs = rhs.size(); i < rs; ++i) {
      if (((*this)[i] += rhs[i]) >= M) {
        (*this)[i] -= M;
      }
    }
    standard();
    return *this;
  }
  PolyS& operator-=(const PolyS& rhs) {
    if (size() < rhs.size()) {
      resize(rhs.size());
    }
    for (int i = 0, rs = rhs.size(); i < rs; ++i) {
      if (((*this)[i] -= rhs[i]) < 0) {
        (*this)[i] += M;
      }
    }
    standard();
    return *this;
  }
  PolyS& operator*=(PolyS&& rhs) {
    int n = static_cast<int>(size()), m = rhs.size(), tot = std::max(1, n + m - 1);
    int sz = 1 << std::__lg(tot * 2 - 1);
    resize(sz);
    rhs.resize(sz);
    dft(*this);
    dft(rhs);
    for (int i = 0; i < sz; ++i) {
      (*this)[i] = 1LL * (*this)[i] * rhs[i] % M;
    }
    idft(*this);
    standard();
    return *this;
  }
  PolyS& operator/=(PolyS&& rhs) {
    int n = static_cast<int>(size()), m = rhs.size();
    if (n < m) {
      return (*this) = PolyS();
    }
    reverse();
    rhs.reverse();
    (*this) *= rhs.inv(n - m + 1);
    resize(n - m + 1);
    reverse();
    return *this;
  }
  PolyS& operator*=(const PolyS& rhs) { return (*this) *= PolyS(rhs); }
  PolyS& operator/=(const PolyS& rhs) { return (*this) /= PolyS(rhs); }
  PolyS& operator%=(const PolyS& rhs) { return (*this) -= (*this) / rhs * rhs; }
  PolyS operator+(const PolyS& rhs) const { return PolyS(*this) += rhs; }
  PolyS operator-(const PolyS& rhs) const { return PolyS(*this) -= rhs; }
  PolyS operator*(const PolyS& rhs) const { return PolyS(*this) *= rhs; }
  PolyS operator/(const PolyS& rhs) const { return PolyS(*this) /= rhs; }
  PolyS operator%(const PolyS& rhs) const { return PolyS(*this) %= rhs; }
  PolyS powModPoly(int n, PolyS p) const {
    PolyS r(1), x(*this);
    while (n) {
      if (n & 1) {
        (r *= x) %= p;
      }
      n >>= 1;
      (x *= x) %= p;
    }
    return r;
  }
  int inner(const PolyS& rhs) const {
    int r = 0, n = std::min(size(), rhs.size());
    for (int i = 0; i < n; ++i) {
      r = (r + 1LL * (*this)[i] * rhs[i]) % M;
    }
    return r;
  }
  PolyS derivation() const {
    if (empty()) {
      return PolyS();
    }
    int n = static_cast<int>(size());
    std::vector<int> r(n - 1);
    for (int i = 1; i < n; ++i) {
      r[i - 1] = 1LL * (*this)[i] * i % M;
    }
    return PolyS(r);
  }
  PolyS integral() const {
    if (empty()) {
      return PolyS();
    }
    int n = static_cast<int>(size());
    std::vector<int> r(n + 1), inv(n + 1, 1);
    for (int i = 2; i <= n; ++i) {
      inv[i] = 1LL * (M - M / i) * inv[M % i] % M;
    }
    for (int i = 0; i < n; ++i) {
      r[i + 1] = 1LL * (*this)[i] * inv[i + 1] % M;
    }
    return PolyS(r);
  }
  PolyS inv(int n) const {
    // assert((*this)[0] != 0);
    PolyS x(powMod((*this)[0], M - 2));
    int k = 1;
    while (k < n) {
      k *= 2;
      x *= (PolyS(2) - this->modXn(k) * x).modXnR(k);
    }
    return x.modXnR(n);
  }
  // assume a[0] = 1
  PolyS log(int n) const { return (derivation() * inv(n)).integral().modXnR(n); }
  // assume a[0] = 0
  PolyS exp(int n) const {
    PolyS x(1);
    int k = 1;
    while (k < n) {
      k *= 2;
      x = (x * (PolyS(1) - x.log(k) + this->modXn(k))).modXnR(k);
    }
    return x.modXnR(n);
  }
  // assume a[0] = 1;
  PolyS sqrt(int n) const {
    const static int inv2 = (M + 1) / 2;
    PolyS x(1);
    int k = 1;
    while (k < n) {
      k *= 2;
      x += this->modXn(k) * x.inv(k);
      x = x.modXnR(k) * inv2;
    }
    return x.modXnR(n);
  }
  int eval(int x) const {
    int r = 0, t = 1;
    for (int i = 0, n = static_cast<int>(size()); i < n; ++i) {
      r = (r + 1LL * (*this)[i] * t) % M;
      t = 1LL * t * x % M;
    }
    return r;
  }
  // transpose convolution {\rm MULT}(F(x),G(x))=\sum_{i\ge0}(\sum_{j\ge 0}f_{i+j}g_j)x^i
  PolyS mulT(PolyS&& rhs) const {
    if (rhs.size() == 0) {
      return PolyS();
    }
    int n = rhs.size();
    std::reverse(rhs.begin(), rhs.end());
    return ((*this) * rhs).divXn(n - 1);
  }
  // multi-evaluation(new tech)
  std::vector<int> evals(const std::vector<int>& x) const {
    if (size() == 0) {
      return std::vector<int>(x.size());
    }
    int n = static_cast<int>(x.size());
    std::vector<int> ans(n);
    std::vector<PolyS> g(4 * n);
    std::function<void(int, int, int)> build = [&](int l, int r, int p) {
      if (r - l == 1) {
        // g[p] = std::vector<int>{1, x[i] ? M - x[l] : 0};
        g[p] = PolyS({1, x[l] ? M - x[l] : 0});
      } else {
        int m = (l + r) / 2;
        build(l, m, 2 * p);
        build(m, r, 2 * p + 1);
        g[p] = g[2 * p] * g[2 * p + 1];
      }
    };
    build(0, n, 1);
    std::function<void(int, int, int, const PolyS&)> solve = [&](int l, int r, int p,
                                                                 const PolyS& f) {
      if (r - l == 1) {
        ans[l] = f.at(0);
      } else {
        int m = (l + r) / 2;
        solve(l, m, 2 * p, f.mulT(std::move(g[2 * p + 1])).modXnR(m - l));
        solve(m, r, 2 * p + 1, f.mulT(std::move(g[2 * p])).modXnR(r - m));
      }
    };
    solve(0, n, 1, mulT(g[1].inv(size())).modXnR(n));
    return ans;
  }  // https://www.luogu.com.cn/problem/P5050

  // $a_n = \sum_{i = 1}^{k} f_i a_{n - i}$: https://oi-wiki.org/math/linear-recurrence/
  // find n-th term of The recursive formula for the constant coefficient of order k in $O(k \log k
  // \log n)$
  static int linearRecursion(const std::vector<int>& a, std::vector<int> f, int64_t n) {
    if (n < static_cast<int>(a.size())) {
      return (a[n] % M + M) % M;
    }
    int m = static_cast<int>(f.size());
    std::reverse(f.begin(), f.end());
    std::vector<int> g(m);
    g.emplace_back(1);
    PolyS A = PolyS({0, 1}), p = PolyS(std::move(g)) - PolyS(std::move(f));
    PolyS R = A.powModPoly(n, p);
    return R.inner(a);
  }  // https://www.luogu.com.cn/problem/P4723
  //
  static std::vector<int> MPFFT(std::vector<int> a, std::vector<int> b) {
    // assert(b.size() == 2 * (a.size() - 1));
    int d = a.size() - 1;
    int sz = 1 << std::__lg(d * 2 - 1);
    std::reverse(a.begin(), a.end());
    a.resize(sz);
    b.resize(sz);
    dft(a);
    idft(b);
    for (int i = 0; i < sz; ++i) a[i] = 1LL * a[i] * b[i] % M;
    a.resize(d);
    const int invN = M - (M - 1) / sz;
    for (auto &x : a) {
      x = 1LL * x * invN % M;
    }
    return a;
  }
};   // https://www.luogu.com.cn/training/3015#information

}  // namespace cuzperf
