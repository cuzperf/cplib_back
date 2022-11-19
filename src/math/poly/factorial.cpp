#include "factorial.h"

#include "poly.hpp"
#include "polyFFT.hpp"
#include "polyMTT.hpp"

namespace cuzperf {

int factorialS(int n, int p) {
  if (n >= p) {
    return 0;
  }
  if (n <= 1) {
    return 1;
  }
  ModInt::setMod(p);
  if (n > p - 1 - n) {
    int ans = ModInt(factorialS(p - 1 - n, p)).inv();
    return (p - n) & 1 ? p - ans : ans;
  }
  ModInt::setMod(p);
  int sn = std::sqrt(n);
  auto A = PolyFFTDynamic::prod(sn);
  std::vector<ModInt> x;
  x.reserve(n / sn);
  for (int i = sn; i <= n; i += sn) {
    x.emplace_back(i - sn + 1);
  }
  auto y = A.evals(x);
  ModInt r(1);
  for (auto t : y) {
    r *= t;
  }
  for (int i = n / sn * sn + 1; i <= n; ++i) {
    r *= ModInt::raw(i);
  }
  return r;
}

int64_t factorial(int64_t n, int64_t p) {
  if (n >= p) {
    return 0;
  }
  if (n <= 1) {
    return 1;
  }
  int s = std::sqrt(n);
  PolyMFT::setMod(p, s + 1);
  if (n > p - 1 - n) {
    auto ans = ModLL(factorial(p - 1 - n, p)).inv();
    return (p - n) & 1 ? -ans : ans;
  }
  std::vector<ModLL> h{1, s + 1};
  for (int bit = lg32(s) - 1, d = 1; bit >= 0; --bit) {
    auto nh1 = PolyMFT::valToVal(h, ModLL(d + 1), d);
    auto nh2 = PolyMFT::valToVal(h, ModLL(s).inv() * ModLL(d), 2 * d + 1);
    h.insert(h.end(), nh1.begin(), nh1.end());
    d *= 2;
    for (int i = 0; i <= d; ++i) {
      h[i] *= nh2[i];
    }
    if (s >> bit & 1) {
      ++d;
      int64_t tmp = d;
      for (int i = 0; i < d; ++i, tmp += s) {
        h[i] *= ModLL::raw(tmp);
      }
      ModLL last(1), tj = ModLL(static_cast<int64_t>(s) * d);
      for (int i = 0; i < d; ++i) {
        ++tj, last *= tj;
      }
      h.emplace_back(last);
    }
  }
  ModLL ans = 1;
  for (int i = 0; i < s; ++i) {
    ans *= h[i];
  }
  for (int64_t i = 1LL * s * s + 1; i <= n; ++i) {
    ans *= ModLL::raw(i);
  }
  return ans;
}

}  // namespace cuzperf
