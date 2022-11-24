#pragma once

#include <algorithm>
#include <vector>

#include "mod.hpp"
#include "base/builtin.h"

namespace cuzperf {

// assume M is NTT-friendly and 3 is a primitive root of N
template<int M>
class NTT {
  using mod = MInt<M>;
  std::vector<int> rev_;
  std::vector<mod> roots_{0, 1};
 public:
  static inline const mod g = 3;
  void dft(std::vector<mod>& a) {
    int n = (int)a.size();
    if ((int)rev_.size() != n) {
      int k = ctz_u32(n) - 1;
      rev_.resize(n);
      for (int i = 0; i < n; ++i) {
        rev_[i] = rev_[i >> 1] >> 1 | (i & 1) << k;
      }
    }
    if ((int)roots_.size() < n) {
      int k = ctz_u32(roots_.size());
      roots_.resize(n);
      while ((1 << k) < n) {
        auto e = pow(g, (M - 1) >> (k + 1));
        for (int i = 1 << (k - 1); i < (1 << k); ++i) {
          roots_[2 * i] = roots_[i];
          roots_[2 * i + 1] = roots_[i] * e;
        }
        ++k;
      }
    }
    for (int i = 0; i < n; ++i) if (rev_[i] < i) {
      std::swap(a[i], a[rev_[i]]);
    }
    for (int k = 1; k < n; k *= 2) {
      for (int i = 0; i < n; i += 2 * k) {
        for (int j = 0; j < k; ++j) {
          auto u = a[i + j], v = a[i + j + k] * roots_[k + j];
          a[i + j] = u + v;
          a[i + j + k] = u - v;
        }
      }
    }
  }
  void idft(std::vector<mod>& a) {
    int n = (int)a.size();
    std::reverse(a.begin() + 1, a.end());
    dft(a);
    // not that n is power of 2, and M = 1 + c 2^x
    auto inv = mod::raw(M - (M - 1) / n);
    for (auto& x : a) x *= inv;
  }
};

}  // namespace cuzperf
