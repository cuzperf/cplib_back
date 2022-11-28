#pragma once

#include <assert.h>
#include <stdint.h>

#include <vector>

namespace cuzperf {

class Binom {
  static inline constexpr int N = 65;
  int64_t C_[N][N];
  Binom() {
    for (int i = 0; i < N; ++i) {
      C_[i][0] = C_[i][i] = 1;
    }
    for (int i = 1; i < N; ++i) {
      for (int j = 1; j < i; ++j) {
        C_[i][j] = C_[i - 1][j] + C_[i - 1][j - 1];
      }
    }
  }

 public:
  Binom(const Binom&) = delete;
  static Binom& Instance() {
    static Binom instance_;
    return instance_;
  }
  int64_t operator()(int m, int n) const {
    assert(n < N && m < N);
    return C_[m][n];
  }
};

template <typename valT>
class BinomModp {
  static inline constexpr int N = 1e6 + 2;
  BinomModp() { fac_.reserve(N), ifac_.reserve(N), inv_.reserve(N); }
  void init(int n) {
    assert(n <= valT::mod());
    fac_[0] = 1;
    for (int i = 1; i < n; ++i) {
      fac_[i] = fac_[i - 1] * valT::raw(i);
    }
    ifac_[n - 1] = fac_[n - 1].inv();
    for (int i = n - 1; i > 0; --i) {
      ifac_[i - 1] = ifac_[i] * valT::raw(i);
    }
    for (int i = 1; i < n; ++i) {
      inv_[i] = ifac_[i] * fac_[i - 1];
    }
  }

 public:
  std::vector<valT> fac_, ifac_, inv_;
  BinomModp(const BinomModp&) = delete;
  static BinomModp& Instance(int n = 0) {
    static BinomModp instance_;
    if (n) {
      instance_.init(n);
    }
    return instance_;
  }
  valT operator()(int n, int k) const {
    if (n < 0 || n < k) {
      return valT(0);
    }
    return fac_[n] * ifac_[k] * ifac_[n - k];
  }
  // M is a small prime number in this case
  valT lucas(int n, int k) const {
    valT r(1);
    const int M = valT::mod();
    while (n && k) {
      int np = n % M, kp = k % M;
      if (np < kp) {
        return valT(0);
      }
      r *= operator()(np, kp);
      n /= M;
      k /= M;
    }
    return r;
  }
};

// Calculate f(m) where f is the Lagrange interpolation on $f(0), f(1), \cdots, f(n - 1)$
template <typename valT>
valT Lagrange(const std::vector<valT>& f, int m) {
  int n = static_cast<int>(f.size());
  if (m < n) {
    return f[m];
  }
  auto& B = BinomModp<valT>::Instance(n);
  std::vector<valT> AP(n), BP(n);
  AP[0] = BP[n - 1] = valT(1);
  for (int i = 1; i < n; ++i) {
    AP[i] = AP[i - 1] * valT::raw(m + 1 - i);
  }
  for (int i = n - 2; ~i; --i) {
    BP[i] = BP[i + 1] * valT::raw(m - 1 - i);
  }
  valT ans = 0;
  for (int i = 0; i < n; ++i) {
    valT x = f[i] * AP[i] * BP[i] * B.ifac_[i] * B.ifac_[n - 1 - i];
    ans += (n - 1 - i) & 1 ? -x : x;
  }
  return ans;
}
// Lagrange theorem $f(x) =  \sum_{i = 0}^{n - 1} f_i \prod_{j \neq i} \frac{x - j}{i - j}$
// Simplies $f(m) = \sum_{i = 0}^{n - 1} (-1)^{n - 1 - i} f_i \binom{m}{i} \binom{m - i - 1}{n - 1 -
// i}$

// Calculate powSum in $O(k)$ based on Lagrange interpolation
template <typename valT>
valT powSum(int n, int k, const std::vector<int>& sp) {
  if (k == 0) {
    return valT(n);
  }
  std::vector<valT> f(k + 2);
  f[1] = valT(1);
  for (int i = 2, nf = static_cast<int>(f.size()); i < nf; ++i) {
    if (sp[i] == i) {
      f[i] = pow(valT(i), k);
    } else {
      f[i] = f[sp[i]] * f[i / sp[i]];
    }
  }
  for (int i = 1, nf = static_cast<int>(f.size()); i < nf; ++i) {
    f[i] += f[i - 1];
  }
  return Lagrange(f, n);
}
// https://codeforces.com/problemset/problem/622/F

}  // namespace cuzperf
