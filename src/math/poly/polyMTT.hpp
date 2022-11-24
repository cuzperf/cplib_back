#pragma once

#include "math/mod.hpp"
#include "math/ntt.hpp"
#include "poly.hpp"

#include "base/builtin.h"

#ifdef __GNUC__

namespace cuzperf {

class PolyBaseMFT4 : public PolyBase<ModLL> {
 public:
  static inline constexpr int M0 = 595591169, M1 = 645922817, M2 = 897581057, M3 = 998244353;
  static inline constexpr int64_t M01 = 1LL * M0 * M1, M23 = 1LL * M2 * M3;
  static inline constexpr __int128_t M0123 = __int128_t(M01) * M23;
  static inline constexpr int t01 = 538269027, t23 = 415935157;
  static inline constexpr int64_t t0123 = 341204425684314487LL;
  static inline NTT<M0> ntt0;
  static inline NTT<M1> ntt1;
  static inline NTT<M2> ntt2;
  static inline NTT<M3> ntt3;
  using PolyBase<ModLL>::PolyBase;
 protected:
  static ModLL crt(int a0, int a1, int a2, int a3) {
    int64_t ans1 = a0 + int64_t(a1 - a0) * t01 % M1 * M0;
    int64_t ans2 = a2 + int64_t(a3 - a2) * t23 % M3 * M2;
    __int128_t ans = ans1 + __int128_t(ans2 - ans1) * t0123 % M23 * M01;
    if (ans < 0) ans += M0123;
    return ModLL(ans);
  }
  PolyBaseMFT4 mul(const PolyBaseMFT4& rhs) const {
    int tot = std::max(1, int(this->size() + rhs.size() - 1));
    int sz = 1 << lg2_u32(tot * 2 - 1);
    std::vector<MInt<M0>> a0(sz), b0(sz);
    std::vector<MInt<M1>> a1(sz), b1(sz);
    std::vector<MInt<M2>> a2(sz), b2(sz);
    std::vector<MInt<M3>> a3(sz), b3(sz);
    for (int i = 0, ns = (int)this->size(); i < ns; ++i) {
      int64_t tmp = (*this)[i];
      a0[i] = tmp; a1[i] = tmp; a2[i] = tmp; a3[i] = tmp;
    }
    for (int i = 0, ns = rhs.size(); i < ns; ++i) {
      int64_t tmp = rhs[i];
      b0[i] = tmp; b1[i] = tmp; b2[i] = tmp; b3[i] = tmp;
    }
    ntt0.dft(a0); ntt0.dft(b0);
    ntt1.dft(a1); ntt1.dft(b1);
    ntt2.dft(a2); ntt2.dft(b2);
    ntt3.dft(a3); ntt3.dft(b3);
    for (int i = 0; i < sz; ++i) {
      a0[i] *= b0[i]; a1[i] *= b1[i]; a2[i] *= b2[i]; a3[i] *= b3[i];
    }
    ntt0.idft(a0); ntt1.idft(a1); ntt2.idft(a2); ntt3.idft(a3);
    std::vector<ModLL> ans(tot);
    for (int i = 0; i < tot; ++i) ans[i] = crt(a0[i], a1[i], a2[i], a3[i]);
    return PolyBaseMFT4(std::move(ans));
  }
};
// 4-module NTT, Module can be up to 1e14 since N < 1e6 in general
using PolyMFT = Poly<PolyBaseMFT4, ModLL>;

}  // namespace cuzperf

#endif
