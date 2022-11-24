#pragma once

#include "math/fft.h"
#include "math/mod.hpp"
#include "poly.hpp"

#include <cmath>

#include "base/builtin.h"

namespace cuzperf {

// do not use it if T = ModLL
template<typename T>
class PolyBaseFFT : public PolyBase<T> {
 protected:
  PolyBaseFFT mul(const PolyBaseFFT& rhs) const {
    int tot = std::max(1, int((int)this->size() + rhs.size() - 1));
    int sz = 1 << lg2_u32(tot * 2 - 1);
    // Must be split to ensure accuracy (or use skill '3 times to 2 times')
    auto A1(*this), A2(*this), B1(rhs), B2(rhs);
    static constexpr int bit = 15, msk = (1LL << bit) - 1;
    for (auto& x : A1) x = int(x) & msk;
    for (auto& x : A2) x = int(x) >> bit;
    for (auto& x : B1) x = int(x) & msk;
    for (auto& x : B2) x = int(x) >> bit;
    std::vector<std::complex<double>> A(sz), B(sz), C(sz);
    for (int i = 0, tSize = (int)this->size(); i < tSize; ++i) {
      A[i] = std::complex<double>((double)A1[i], (double)A2[i]);
    }
    for (int i = 0, rSize = rhs.size(); i < rSize; ++i) {
      B[i] = std::complex<double>((double)B1[i], (double)B2[i]);
    }
    FFT::dft(A); FFT::dft(B);
    C[0] = conj(B[0]);
    for (int i = 1; i < sz; ++i) C[i] = conj(B[sz - i]);
    for (int i = 0; i < sz; ++i) B[i] *= A[i];
    for (int i = 0; i < sz; ++i) C[i] *= A[i];
    FFT::idft(B); FFT::idft(C);
    std::vector<T> ans(tot), A1B2(tot), A1B1(tot);
    // It will lose a lot of precision
    for (int i = 0; i < tot; ++i) {
      A1B2[i] = static_cast<int64_t>(std::llround(B[i].imag()));
      A1B1[i] = static_cast<int64_t>(std::llround(B[i].real() * 0.5 + C[i].real() * 0.5));
      ans[i] = static_cast<int64_t>(std::llround(C[i].real() * 0.5 - B[i].real() * 0.5));
    }
    for (auto& x : ans) x <<= bit;
    for (int i = 0; i < tot; ++i) ans[i] += A1B2[i];
    for (auto& x : ans) x <<= bit;
    for (int i = 0; i < tot; ++i) ans[i] += A1B1[i];
    return PolyBaseFFT(std::move(ans));
  }
 public:
  using PolyBase<T>::PolyBase;
};
const constexpr int FFTM = 1e9 + 7;
using PolyFFT = Poly<PolyBaseFFT<MInt<FFTM>>, MInt<FFTM>>;
using PolyFFTDynamic = Poly<PolyBaseFFT<ModInt>, ModInt>;
// The following are not recommented
using PolyFFTLL = Poly<PolyBaseFFT<ModLL>, ModLL>;

}  // namespace cuzperf
