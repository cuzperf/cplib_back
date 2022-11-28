#include "numberTheory.h"

#include <assert.h>
#include <random>

#include "base/builtin.h"

namespace cuzperf {

std::vector<int> spfS(int N) {
  std::vector<int> sp(N);
  std::iota(sp.begin(), sp.end(), 0);
  for (int i = 2; i * i < N; ++i) {
    if (sp[i] == i) {
      for (int j = i * i; j < N; j += i) {
        if (sp[j] == j) {
          sp[j] = i;
        }
      }
    }
  }
  return sp;
}

std::vector<int> spf(int N) {
  std::vector<int> sp(N), p{0, 2};
  p.reserve(N);
  for (int i = 2; i < N; i += 2) {
    sp[i] = 2;
  }
  for (int i = 1; i < N; i += 2) {
    sp[i] = i;
  }
  for (int i = 3; i < N; i += 2) {
    if (sp[i] == i) {
      p.emplace_back(i);
    }
    for (int j = 2, np = static_cast<int>(p.size()); j < np && p[j] <= sp[i] && i * p[j] < N; ++j) {
      sp[i * p[j]] = p[j];  // Note that sp[x] is assigned only once foreach x
    }
  }
  return sp;
}

std::vector<int> nsf(int N) {
  std::vector<int> ans(N);
  std::iota(ans.begin(), ans.end(), 0);
  for (int i = 1; i < N; ++i) {
    if (ans[i] == i) {
      for (int j = 2; i * j * j < N; ++j) {
        ans[i * j * j] = i;
      }
    }
  }
  return ans;
}

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

std::pair<std::vector<int>, std::vector<int>> npf(int N) {
  std::vector<int> np(N, 1), nps(N, 1), p{0, 2};
  p.reserve(N);
  nps[0] = nps[1] = 0;
  np[0] = np[1] = 0;
  for (int i = 3; i < N; i += 2) {
    if (nps[i] == 1) {
      p.emplace_back(i);
    }
    for (int j = 2, t, pSize = p.size(); j < pSize && (t = i * p[j]) < N; ++j) {
      nps[t] = nps[i] + 1;
      np[t] = np[i];
      if (i % p[j] == 0) {
        break;
      }
      ++np[t];
    }
  }
  for (int i = 2; i < N; i += 4) {
    np[i] = np[i >> 1] + 1;
  }
  for (int i = 4; i < N; i += 4) {
    np[i] = np[i >> 1];
  }
  for (int i = 2; i < N; i += 2) {
    nps[i] = nps[i >> 1] + 1;
  }
  return {np, nps};
}

std::vector<int> factor(int n, const std::vector<int>& sp) {
  std::vector<int> ans;
  while (n > 1) {
    int pn = sp[n];
    ans.emplace_back(pn);
    while (n % pn == 0) {
      n /= pn;
    }
  }
  return ans;
}

std::vector<std::pair<int, int>> Factor(int n, const std::vector<int>& sp) {
  std::vector<std::pair<int, int>> ans;
  while (n > 1) {
    int pn = sp[n], cnt = 0;
    while (n % pn == 0) {
      n /= pn;
      ++cnt;
    }
    ans.emplace_back(pn, cnt);
  }
  return ans;
}

int primitiveRoot(int n, const std::vector<int>& sp) {
  if (n < 2) {
    return 0;
  }
  if (n == 2 || n == 4) {
    return n - 1;
  }
  if (n % 4 == 0) {
    return 0;
  }
  int n2 = n % 2 == 0 ? n / 2 : n;
  int pn = sp[n2];
  while (n2 % pn == 0) {
    n2 /= pn;
  }
  if (n2 != 1) {
    return 0;
  }
  auto fp = factor(pn - 1, sp);
  auto check = [&](int i) {
    for (auto x : fp) {
      if (powMod(i, (pn - 1) / x, pn) == 1) {
        return false;
      }
    }
    return true;
  };
  int ans = 2;
  while (!check(ans)) {
    ++ans;
  }
  n2 = n % 2 == 0 ? n / 2 : n;
  if (n2 != pn) {
    int m = n2 / pn * (pn - 1);
    auto fm = factor(m, sp);
    for (auto x : fp) {
      if (powMod(ans, m / x, m) == 1) {
        ans += pn;
        break;
      }
    }
  }
  if (n2 != n && (ans % 2 == 0)) {
    ans += n2;
  }
  return ans;
}

std::vector<int> primitiveRootAllS(int n, const std::vector<int>& sp) {
  int g = primitiveRoot(n, sp);
  if (g == 0) {
    return {};
  }
  if (n == 2 || n == 4) {
    return {n - 1};
  }
  int n2 = n & 1 ? n : n / 2;
  int pn = sp[n2], m = n2 / pn * (pn - 1), now = g;
  std::vector<int> ans{g};
  for (int i = 2; i < m; ++i) {
    now = 1LL * now * g % n;
    if (std::gcd(i, m) == 1) {
      ans.emplace_back(now);
    }
  }
  std::sort(ans.begin(), ans.end());
  return ans;
}

std::vector<int> primitiveRootAll(int n, const std::vector<int>& sp) {
  if (n < 2) {
    return {};
  }
  if (n == 2 || n == 4) {
    return {n - 1};
  }
  if (n % 4 == 0) {
    return {};
  }
  int n2 = n % 2 == 0 ? n / 2 : n, pn = sp[n2];
  while (n2 % pn == 0) {
    n2 /= pn;
  }
  if (n2 != 1) {
    return {};
  }
  int m = (n & 1 ? n : n / 2) / pn * (pn - 1);
  std::vector<int> vis(n, -1), ans;
  for (int i = 2; i < n; ++i) {
    if (vis[i] == -1 && std::gcd(i, n) == 1) {
      bool flag = true;
      int now = 1;
      for (int j = 1; j < m; ++j) {
        now = 1LL * now * i % n;
        if (now == 1) {
          flag = false;
          break;
        }
        if (std::gcd(j, m) == 1) {
          vis[now] = i;
        } else {
          vis[now] = 0;
        }
      }
      if (flag) {
        for (int j = 0; j < n; ++j) {
          if (vis[j] == i) {
            ans.emplace_back(j);
          }
        }
        return ans;
      }
    }
  }
  return {};
}

namespace PollardRho {
static std::mt19937_64 rnd64(std::chrono::steady_clock::now().time_since_epoch().count());
int64_t powModll(int64_t x, int64_t n, int64_t p) {
  assert(n >= 0);
  int64_t r = 1;
  for (; n; n >>= 1) {
    if (n & 1) {
      r = mulModi(r, x, p);
    }
    x = mulModi(x, x, p);
  }
  return r;
}

bool witness(int64_t a, int64_t n, int64_t m, int t) {
  int64_t x = powModll(a, m, n);
  if (x == 1 || x == n - 1) {
    return false;
  }
  while (t--) {
    x = mulModi(x, x, n);
    if (x == n - 1) {
      return false;
    }
  }
  return true;
}
static constexpr int TIMES = 52;
bool rabin(int64_t n) {
  if (n < 2) {
    return false;
  }
  if (n == 2) {
    return true;
  }
  if (n % 2 == 0) {
    return false;
  }
  int64_t m = n - 1;
  int t = ctz_u64(m);
  m >>= t;
  for (int cnt = 0; cnt < TIMES; ++cnt) {
    int64_t a = rnd64() % (n - 1) + 1;
    if (witness(a, n, m, t)) {
      return false;
    }
  }
  return true;
}
int64_t pollardrho(int64_t n) {
  int64_t x = 0, y = 0, z = 1, i = 1, k = 2, c = rnd64() % (n - 1) + 1;
  while (true) {
    x = mulModi(x, x, n);
    x += c;
    if (x >= n) {
      x -= n;
    }
    z = mulModi(y - x + n, z, n);
    // optim times of compute gcd
    if (++i == k) {
      int64_t d = std::gcd(z, n);
      if (d > 1) {
        return d;
      }
      y = x;
      if (k > n) {
        return n;
      }
      k <<= 1;
    }
  }
}
int64_t spf(int64_t n) {
  if (rabin(n) || n == 1) {
    return n;
  }
  int64_t d = n;
  while (d == n) {
    d = pollardrho(n);
  }
  return std::min(spf(d), spf(n / d));
}
int64_t gpf(int64_t n, int64_t mxf) {
  if (rabin(n)) {
    return n;
  }
  if (n <= mxf) {
    return 1;
  }
  int64_t d = n;
  while (d == n) {
    d = pollardrho(n);
  }
  int64_t res = gpf(d, mxf);
  return std::max(res, gpf(n / d, std::max(res, mxf)));
}
}  // namespace PollardRho


int babyStepGiantStep(int a, int b, int p) {
  a %= p;
  b %= p;
  if (p == 1 || b == 1) {
    return 0;
  }
  int cnt = 0, t = 1;
  for (int g = std::gcd(a, p); g != 1; g = std::gcd(a, p)) {
    if (b % g) {
      return -1;
    }
    p /= g;
    ++cnt;
    b /= g;
    t = 1LL * t * (a / g) % p;
    if (b == t) {
      return cnt;
    }
  }
  std::map<int, int> mp;
  int m = std::ceil(std::sqrt(p));
  int base = b;
  for (int i = 0; i != m; ++i) {
    mp[base] = i;
    base = 1LL * base * a % p;
  }
  base = powMod(a, m, p);
  for (int i = 1; i <= m; ++i) {
    t = 1LL * t * base % p;
    if (mp.count(t)) {
      return (1LL * i * m - mp[t]) % p + cnt;
    }
  }
  return -1;
}

int sqrtModpS(int a, int p) {
  if (a == 0 || p == 2) {
    return a;
  }
  auto power = [p](int x, int n) {
    int r = 1;
    while (n) {
      if (n & 1) {
        r = 1LL * r * x % p;
      }
      n >>= 1;
      x = 1LL * x * x % p;
    }
    return r;
  };
  int q = (p - 1) / 2;
  if (power(a, q) != 1) {
    return -1;
  }
  if (q & 1) {
    return power(a, (q + 1) / 2);
  }
  int b;  // find a non-quadratic residue
  std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());
  while (power(b = rnd() % (p - 1) + 1, q) == 1)
    ;
  int c = ctz_u32(q);
  q >>= c;  // p - 1 = q << (c + 1)
  b = power(b, q);
  int x = power(a, (q + 1) / 2), t = power(a, q);
  // Keep x^2 = a t, t^{2^c} = 1, b^{2^c} = -1
  while (t != 1) {
    // return smallest r s.t. u^{2^r} = -1
    int cc = [p](int u) {
      int r = 0;
      while ((u = 1LL * u * u % p) != 1) {
        ++r;
      }
      return r;
    }(t);
    int d = power(b, 1 << (c - cc - 1));  // d^{2^{cc + 1}} = -1
    // update reason: (xd)^2 = a t d^2, (t d^2)^{2^{cc}} = 1, (d^2)^{2^cc} = -1
    x = 1LL * x * d % p;
    b = 1LL * d * d % p;
    t = 1LL * t * b % p;
    c = cc;
  }
  return x;
}

struct pseudoComplex {
  int x, y;
  static inline int p, m;
  static void setMod(int _p, int _m) { p = _p; m = _m; }
  pseudoComplex(int _x = 0, int _y = 0) : x(_x), y(_y) {}
  pseudoComplex operator*(const pseudoComplex& A) const {
    return pseudoComplex((1LL * x * A.x + 1LL * y * A.y % p * m) % p,
                         (1LL * x * A.y + 1LL * y * A.x) % p);
  }
};

int sqrtModp(int a, int p) {
  if (a == 0 || p == 2) {
    return a;
  }
  auto power = [p](int x, int n) {
    int r = 1;
    while (n) {
      if (n & 1) {
        r = 1LL * r * x % p;
      }
      n >>= 1;
      x = 1LL * x * x % p;
    }
    return r;
  };
  int q = (p - 1) / 2;
  if (power(a, q) != 1) {
    return -1;
  }
  if (q & 1) {
    return power(a, (q + 1) / 2);
  }
  std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());
  int b, m;  // find a non-quadratic residue
  do {
    b = rnd() % p;
    m = (1LL * b * b - a) % p;
    if (m < 0) {
      m += p;
    }
  } while (power(m, q) == 1);
  int n = (p + 1) / 2;
  pseudoComplex::setMod(p, m);
  pseudoComplex R(1, 0), A(b, 1);
  while (n) {
    if (n & 1) {
      R = R * A;
    }
    n >>= 1;
    A = A * A;
  }
  return R.x;
}

std::vector<std::tuple<int, int, int>> lcmPair(int n) {
  std::vector<std::pair<int, int>> ed;
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j < i && i * j <= n; ++j) {
      if (std::gcd(i, j) == 1) {
        ed.emplace_back(j, i);
      }
    }
  }
  std::vector<std::tuple<int, int, int>> edge;
  for (auto [u, v] : ed) {
    int uv = u * v;
    for (int i = u, j = v, d = uv; d <= n; i += u, j += v, d += uv) {
      edge.emplace_back(i, j, d);
    }
  }
  return edge;
}

}  // namespace cuzperf
