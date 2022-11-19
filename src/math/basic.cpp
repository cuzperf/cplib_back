#include "basic.h"

#include <assert.h>
#include <algorithm>
#include <numeric>

#include "base/builtin.h"

namespace cuzperf {

int powMod(int x, int n, int M) {
  assert(n >= 0);
  int r = 1;
  for (; n; n >>= 1) {
    if (n & 1) { r = 1LL * r * x % M; }
    x = 1LL * x * x % M;
  }
  return r;
}

// (n - n % a) = (n / a) * a  ==>  a^{-1} = -(n / a) (n % a)^{-1}
int inv(int a, int n) {
  return a == 1 ? 1 : 1LL * (n - n / a) * inv(n % a, n) % n;
}

int64_t gcd_(int64_t a, int64_t b) {
  if (!a || !b) { return a | b; }
  unsigned shift = ctz64(a | b);
  a >>= ctz64(a);
  do {
    b >>= ctz64(b);
    if (a > b) std::swap(a, b);
    b -= a;
  } while (b);
  return a << shift;
}
int64_t gcd(int64_t a, int64_t b) {
  return gcd_(std::abs(a), std::abs(b));
}

// Chinese remainder theorem: x = ai mod mi, m_i > 0, 0 <= a_i < m_i
std::pair<int64_t, int64_t> crt_(int a1, int m1, int a2, int m2) {
  auto [d, t1, t2] = exGcd(m1, m2);
  // we assume that the solution always exists
  assert((a2 - a1) % d == 0);
  int64_t m = 1LL * (m1 / d) * m2;
  int64_t ans = (a1 + 1LL * ((a2 - a1) / d) * t1 % m2 * m1) % m;
  return {ans < 0 ? ans + m: ans, m};
}
std::pair<int64_t, int64_t> crt(int a1, int m1, int a2, int m2) {
  a1 %= m1; a2 %= m2;
  return crt_(a1 < 0 ? a1 + m1 : a1, m1, a2 < 0 ? a2 + m2 : a2, m2);
}
// https://www.luogu.com.cn/problem/P1495


}  // namespace cuzperf
