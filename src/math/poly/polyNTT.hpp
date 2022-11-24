#pragma once

#include "poly.hpp"
#include "math/mod.hpp"
#include "math/ntt.hpp"
#include "base/builtin.h"

namespace cuzperf {

template<int N>
class PolyBaseNTT : public PolyBase<MInt<N>> {
 protected:
  PolyBaseNTT mul(const PolyBaseNTT& rhs) const {
    int n = (int)this->size(), m = rhs.size(), tot = std::max(1, n + m - 1);
    int sz = 1 << lg2_u32(tot * 2 - 1);
    std::vector<MInt<N>> A = *this, B = rhs;
    A.resize(sz); B.resize(sz);
    ntt.dft(A); ntt.dft(B);
    for (int i = 0; i < sz; ++i) A[i] *= B[i];
    ntt.idft(A);
    A.resize(n + m - 1);
    return PolyBaseNTT(std::move(A));
  }
 public:
  static inline constexpr int M = N;
  static inline NTT<N> ntt;
  using PolyBase<MInt<N>>::PolyBase;
};

const constexpr int NTTM = 998244353;
using PolyNTT = Poly<PolyBaseNTT<NTTM>, MInt<NTTM>>;

}  // namespace cuzperf
