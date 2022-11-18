#include "mix.h"

namespace cuzperf {

constexpr int MAXN = 100000;
// constexpr int MAXN = 12345678;
// -fconstexpr-loop-limit (default: 1<<18=262144),
// -fconstexpr-ops-limit  (default: 1<<25=33554432)
// -fconstexpr-depth      (default:  1<<9=512)
// -ftemplate-depth       (default: 900)
// g++ main.cpp -std=c++17 -fconstexpr-loop-limit=12345678 -fconstexpr-ops-limit=1234567890 -fconstexpr-depth=100000 -ftemplate-depth=100000
bool fast_is_prime(int n) {
  static constexpr Sieve<MAXN> s;
  return s.isP[n];
}
constexpr bool isPrime(int n) {
  if (n < 2) return false;
  if (n < 4) return true;
  if (n % 2 == 0) return false;
  for (int i = 3; i * i <= n; i += 2) {
    if (n % i == 0) return false;
  }
  return true;
};
// https://codeforces.com/blog/entry/79941

constexpr int fastSqrt(int v) {
  unsigned temp = 0, nHat = 0, b = 0x8000, bshft = 15;
  do {
    if (v >= (temp = (((nHat << 1) + b) << bshft--))) {
      nHat += b;
      v -= temp;
    }
  } while (b >>= 1);
  return nHat;
}

constexpr bool isPrimeR(int n, int c) {
  return c * c > n ? true : n % c == 0 ? false : isPrimeR(n, c + 2);
}
constexpr bool isPrimeConstexpr(int n) {
  return n < 2 ? false : n < 4 || (n % 2 == 1 && isPrimeR(n, 3));
}

}  // namespace cuzperf
