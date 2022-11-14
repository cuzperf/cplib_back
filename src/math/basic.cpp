#include "basic.h"

#include <assert.h>
#include <algorithm>
#include <numeric>

#include "builtin.h"

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


// O(N \log N) smalleset prime factor(may be faster)
std::vector<int> spfS(int N) {
  std::vector<int> sp(N);
  std::iota(sp.begin(), sp.end(), 0);
  for(int i = 2; i * i < N; ++i) if(sp[i] == i) {
    for(int j = i * i; j < N; j += i) if(sp[j] == j) {
      sp[j] = i;
    }
  }
  return sp;
}

// $O(N)$ smallest prime factor
std::vector<int> spf(int N) {
  std::vector<int> sp(N), p{0, 2};
  p.reserve(N);
  for (int i = 2; i < N; i += 2) sp[i] = 2;
  for (int i = 1; i < N; i += 2) sp[i] = i;
  for (int i = 3; i < N; i += 2) {
    if (sp[i] == i) p.emplace_back(i);
    for (int j = 2, np = (int)p.size(); j < np && p[j] <= sp[i] && i * p[j] < N; ++j) {
      sp[i * p[j]] = p[j]; // Note that sp[x] is assigned only once foreach x
    }
  }
  return sp;
}

// O(N) none square factor
std::vector<int> nsf(int N) {
  std::vector<int> ans(N);
  std::iota(ans.begin(), ans.end(), 0);
  for (int i = 1; i < N; ++i) if (ans[i] == i) {
    for (int j = 2; i * j * j < N; ++j) {
      ans[i * j * j] = i;
    }
  }
  return ans;
}

// O(N) none square factor
std::vector<int> nsfS(int N) {
  auto sp = spf(N);
  std::vector<int> ans(N);
  ans[1] = 1;
  for (int i = 2; i < N; ++i) {
    int si = i / sp[i];
    ans[i] = si % sp[i] == 0 ? ans[si / sp[i]] : ans[si] * sp[i];
  }
  return ans;
}

}  // namespace cuzperf
