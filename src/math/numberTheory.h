#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <map>
#include <numeric>
#include <random>
#include <unordered_map>
#include <vector>

#include "math/basic.h"

namespace cuzperf {

// note that p_[1] = 2 and p_[0] is meanless
class Prime {
  static inline constexpr int N = 5e6 + 2;
  bool isp_[N]{};
  std::vector<int> p_{0, 2}, pi_;
  // $O(N \log \log N)$ but faster when N < 1e9
  void initPrime() {
    p_.reserve(N);
    isp_[2] = true;
    for (int i = 3; i < N; i += 2) {
      isp_[i] = true;
    }
    int sq = int(std::sqrt(N + 0.1)) | 1;  // make sure it is odd number
    for (int i = 3; i <= sq; i += 2) {
      if (isp_[i]) {
        p_.emplace_back(i);
        for (int j = i * i; j < N; j += i << 1) {
          isp_[j] = false;
        }
      }
    }
    for (int i = sq + 2; i < N; i += 2) {
      if (isp_[i]) {
        p_.emplace_back(i);
      }
    }
  }
  // $O(N)$ but slower when N < 1e9
  std::vector<int> initPrimeS() {
    p_.reserve(N);
    isp_[2] = true;
    for (int i = 3; i < N; i += 2) {
      isp_[i] = true;
    }
    for (int i = 3; i < N; i += 2) {
      if (isp_[i]) {
        p_.emplace_back(i);
      }
      // use t to avoid overflow
      for (int j = 2, t = (N - 1) / i + 1, np = p_.size(); j < np && p_[j] < t; ++j) {
        isp_[i * p_[j]] = false;  // isp_[x]
        // It will be O(nloglogn) if we remove following code
        if (i % p_[j] == 0) {
          break;  // % is time-consuming
        }
      }
    }
    return p_;
  }
  static inline constexpr int M = 6;
  std::vector<int> phi_[M + 1];
  void initPi() {
    pi_[2] = 1;
    for (int i = 3; i < N; ++i) {
      pi_[i] = pi_[i - 1];
      if (isp_[i]) {
        ++pi_[i];
      }
    }
    std::vector<int> sz(M + 1, 1);
    for (int i = 1; i <= M; ++i) {
      sz[i] = p_[i] * sz[i - 1];
    }
    phi_[0] = {1};  // phi_[0] is meanless
    // optim since phi_[j][i] = phi_[j][i - 1] - phi_[j / p_[i]][i - 1]
    for (int i = 1; i <= M; ++i) {
      phi_[i].resize(sz[i]);
      for (int j = 0; j < p_[i]; ++j) {
        for (int k = 0, jsz = j * sz[i - 1]; k < sz[i - 1]; ++k) {
          phi_[i][jsz + k] = j * phi_[i - 1].back() + phi_[i - 1][k];
        }
      }
      for (int k = 0; k < sz[i - 1]; ++k) {
        for (int j = 0, kp = k * p_[i]; j < p_[i]; ++j) {
          phi_[i][kp + j] -= phi_[i - 1][k];
        }
      }
    }
  }
  // See if x \in (s - n, s] is prime assume that p_.back() * p_.back() >= s
  std::vector<int> seive(int64_t s, int n) {  // O(N log s)
    std::vector<int> isP(n, 1);               // isP[i] = 1 means s - i is prime
    for (int i = 1; 1LL * p_[i] * p_[i] <= s; ++i) {
      for (int j = s % p_[i]; j < n; j += p_[i]) {
        isP[j] = 0;
      }
    }
    return isP;
  }
  Prime() : pi_(N) {
    initPrime();
    initPi();
  }
  bool isPrimeS(int64_t n) {
    if (n == 2) {
      return true;
    }
    if (n == 1 || n % 2 == 0) {
      return false;
    }
    for (int64_t i = 3; i * i <= n; i += 2) {
      if (n % i == 0) {
        return false;
      }
    }
    return true;
  }

 public:
  int operator[](int i) { return p_[i]; }
  int64_t primephi(int64_t x, int s) {
    if (s <= M) {
      return (x / phi_[s].size()) * phi_[s].back() + phi_[s][x % phi_[s].size()];
    }
    if (x / p_[s] <= p_[s]) {
      return primePi(x) - s + 1;
    }
    if (x / p_[s] / p_[s] <= p_[s] && x < N) {
      int s2x = pi_[static_cast<int>(std::sqrt(x + 0.2))];
      int64_t ans = pi_[x] - (s2x + s - 2) * (s2x - s + 1) / 2;
      for (int i = s + 1; i <= s2x; ++i) {
        ans += pi_[x / p_[i]];
      }
      return ans;
    }
    return primephi(x, s - 1) - primephi(x / p_[s], s - 1);
  }
  int64_t primePi(int64_t x) {
    if (x < N) {
      return pi_[x];
    }
    int ps2x = primePi(int(std::sqrt(x + 0.2)));
    int ps3x = primePi(int(std::cbrt(x + 0.2)));
    int64_t ans = primephi(x, ps3x) + 1LL * (ps2x + ps3x - 2) * (ps2x - ps3x + 1) / 2;
    for (int i = ps3x + 1, ed = ps2x; i <= ed; ++i) {
      ans -= primePi(x / p_[i]);
    }
    return ans;
  }
  bool isPrime(int64_t n) {
    if (n < N) {
      return isp_[n];
    }
    if (1LL * p_.back() * p_.back() > n) {
      return isPrimeS(n);
    }
    for (int i = 1; p_[i] * p_[i] <= n; ++i) {
      if (n % p_[i] == 0) {
        return false;
      }
    }
    return true;
  }
  // DynamicProgramming version O(\frac{n}{\log n}) with n < 10^12
  int64_t primePiS(int64_t n) {
    int rn = std::sqrt(n + 0.2);
    std::vector<int64_t> R(rn + 1);
    for (int i = 1; i <= rn; ++i) {
      R[i] = n / i - 1;
    }
    int ln = n / (rn + 1) + 1;
    std::vector<int64_t> L(ln + 1);
    for (int i = 1; i <= ln; ++i) {
      L[i] = i - 1;
    }
    for (int p = 2; p <= rn; ++p) {
      if (L[p] == L[p - 1]) {
        continue;
      }
      for (int i = 1, tn = std::min(n / p / p, int64_t(rn)); i <= tn; ++i) {
        R[i] -= (i <= rn / p ? R[i * p] : L[n / i / p]) - L[p - 1];
      }
      for (int i = ln; i / p >= p; --i) {
        L[i] -= L[i / p] - L[p - 1];
      }
    }
    return R[1];
  }
  int64_t nthPrime(int64_t n) {  // Newton method
    if (n < static_cast<int>(p_.size())) {
      return p_[n];
    }
    int64_t ans = n * log(n), err = log(n) / log(10);
    int64_t m = primePi(ans);
    while (m < n || m > n + err) {
      ans += (n - m) / (log(m) - 1) * log(m) * log(m);
      m = primePi(ans);
    }
    int sn = std::sqrt(N);
    while (1) {
      auto isP = seive(ans, sn);
      for (int i = 0; i < sn; ++i) {
        if (isP[i]) {
          if (m-- == n) {
            return ans - i;
          }
        }
      }
      ans -= sn;
    }
  }
  Prime(const Prime&) = delete;
  static Prime& Instance() {
    static Prime instance;
    return instance;
  }
};

class Euler {
  static inline constexpr int N = 5e6 + 2;
  std::vector<int> phi_, p_{0, 2};
  std::unordered_map<int, int64_t> mpPhi_;
  std::vector<int64_t> sumPhi_;
  void initPhi() {  // $O(N)$
    for (int i = 1; i < N; i += 2) {
      phi_[i] = i;
    }
    for (int i = 2; i < N; i += 2) {
      phi_[i] = i >> 1;
    }
    for (int i = 3; i < N; i += 2) {
      if (phi_[i] == i) {
        p_.emplace_back(i);
        --phi_[i];
      }
      for (int j = 2, t = (N - 1) / i + 1, np = p_.size(); j < np && p_[j] < t; ++j) {
        if (i % p_[j] == 0) {
          phi_[i * p_[j]] = phi_[i] * p_[j];
          break;
        }
        phi_[i * p_[j]] = phi_[i] * (p_[j] - 1);
      }
    }
    for (int i = 2; i < N; i += 4) {
      phi_[i] = phi_[i >> 1];
    }
    for (int i = 4; i < N; i += 4) {
      phi_[i] = phi_[i >> 1] << 1;
    }
  }
  int64_t getPhiS(int64_t n) {
    if (n % 2 == 0) {
      n /= 2;
    }
    int64_t r = n;
    while (n % 2 == 0) {
      n /= 2;
    }
    for (int64_t i = 3; i * i <= n; i += 2) {
      if (n % i == 0) {
        r = r / i * (i - 1);
        while (n % i == 0) {
          n /= i;
        }
      }
    }
    if (n > 1) {
      r = r / n * (n - 1);
    }
    return r;
  }
  Euler() : phi_(N), sumPhi_(N) {
    initPhi();
    for (int i = 1; i < N; ++i) {
      sumPhi_[i] = sumPhi_[i - 1] + phi_[i];
    }
  }

 public:
  int operator[](int i) { return phi_[i]; }
  int64_t getPhi(int64_t n) {
    if (n < static_cast<int>(phi_.size())) {
      return phi_[n];
    }
    if (1LL * p_.back() * p_.back() > n) {
      return getPhiS(n);
    }
    int64_t r = n;
    for (int i = 1; 1LL * p_[i] * p_[i] <= n; ++i) {
      if (n % p_[i] == 0) {
        r = r / p_[i] * (p_[i] - 1);
        while (n % p_[i] == 0) {
          n /= p_[i];
        }
      }
    }
    if (n > 1) {
      r = r / n * (n - 1);
    }
    return r;
  }
  // min_25 $O(n^{\frac{2}{3}})$
  int64_t getSumPhi(int n) {
    if (n < N) {
      return sumPhi_[n];
    }
    if (mpPhi_.count(n)) {
      return mpPhi_[n];
    }
    int64_t r = 1LL * (n + 1) * n / 2;
    for (int i = 2, j; i <= n; i = j + 1) {
      j = n / (n / i);
      r -= (j - i + 1) * getSumPhi(n / i);
    }
    return mpPhi_[n] = r;
  }
  Euler(const Euler&) = delete;
  static Euler& Instance() {
    static Euler instance;
    return instance;
  }
};
// https://www.luogu.com.cn/problem/P4213

class Mobius {
  static inline constexpr int N = 5e6 + 2;
  std::vector<int> mu_, sumMu_, p_{0, 2};
  std::unordered_map<int, int> mpMu_;
  int getMuS(int64_t n) {
    if (n % 4 == 0) {
      return 0;
    }
    int r = (n % 2 ? 1 : -1);
    if (n % 2 == 0) {
      n /= 2;
    }
    for (int64_t i = 3; i * i <= n; i += 2) {
      if (n % i == 0) {
        n /= i;
        if (n % i == 0) {
          return 0;
        }
        r = -r;
      }
    }
    return n > 1 ? -r : r;
  }
  void initMuS() {  // $O(n log n)$
    mu_[1] = 1;
    for (int i = 1; i < N; ++i) {
      for (int j = i * 2; j < N; j += i) {
        mu_[j] -= mu_[i];
      }
    }
  }
  void initMu() {
    p_.reserve(N);
    for (int i = 1; i < N; i += 2) {
      mu_[i] = i;
    }
    for (int i = 3; i < N; i += 2) {
      if (mu_[i] == i) {
        mu_[i] = -1;
        p_.emplace_back(i);
      }
      for (int j = 2, t = (N - 1) / i + 1, np = p_.size(); j < np && p_[j] < t; ++j) {
        if (i % p_[j] == 0) {
          mu_[i * p_[j]] = 0;
          break;
        }
        mu_[i * p_[j]] = -mu_[i];
      }
    }
    for (int i = 2; i < N; i += 4) {
      mu_[i] = -mu_[i >> 1];
    }
  }
  Mobius() : mu_(N), sumMu_(N) {
    initMu();
    for (int i = 1; i < N; ++i) {
      sumMu_[i] = sumMu_[i - 1] + mu_[i];
    }
  }

 public:
  int operator[](int i) { return mu_[i]; }  // assmue i < N
  int getMu(int64_t n) {
    if (n < static_cast<int>(mu_.size())) {
      return mu_[n];
    }
    if (1LL * p_.back() * p_.back() > n) {
      return getMuS(n);
    }
    int r = 1;
    for (int i = 1; 1LL * p_[i] * p_[i] <= n; ++i) {
      if (n % p_[i] == 0) {
        n /= p_[i];
        if (n % p_[i] == 0) {
          return 0;
        }
        r = -r;
      }
    }
    return n > 1 ? -r : r;
  }
  int64_t getAbsSum(int64_t n) {  // Q(n) = Q(n-1) + |mu_(n)|
    int64_t r = 0;
    for (int64_t i = 1; i * i < n; ++i) {
      r += mu_[i] * (n / i / i);
    }
    return r;
  }
  // min_25 $O(n^{\frac{2}{3}})$
  int getSumMu(int n) {
    if (n < N) {
      return sumMu_[n];
    }
    if (mpMu_.count(n)) {
      return mpMu_[n];
    }
    int r = 1;
    for (int i = 2, j; i <= n; i = j + 1) {
      j = n / (n / i);
      r -= 1LL * (j - i + 1) * getSumMu(n / i);
    }
    return mpMu_[n] = r;
  }
  Mobius(const Mobius&) = delete;
  static Mobius& Instance() {
    static Mobius instance;
    return instance;
  }
};
// https://www.luogu.com.cn/problem/P4213

// $O(N \log N)$ smalleset prime factor(may be faster)
std::vector<int> spfS(int N);
// $O(N)$ smallest prime factor
std::vector<int> spf(int N);

// $O(N)$ smallest prime factor of n
int spfator(int n);

// $O(N)$ none square factor
std::vector<int> nsf(int N);

// $O(N)$ none square factor using spf
std::vector<int> nsfS(int N);

// init numbers of (multi) prime factors less than N in $O(N)$
std::pair<std::vector<int>, std::vector<int>> npf(int N);

// list of different prime factors of n
std::vector<int> factor(int n);

// list of different prime factors of n
std::vector<int> factor(int n, const std::vector<int>& sp);

// list of prime factors of n
std::vector<std::pair<int, int>> Factor(int n, const std::vector<int>& sp);

// smallest primitive root or 0
int primitiveRoot(int n);
// list of all primitive roots or empty
std::vector<int> primitiveRootAllS(int n);
// list of all primitive roots or empty
std::vector<int> primitiveRootAll(int n);
// https://www.luogu.com.cn/problem/P6091

// Probabilistic Method: Miller-Rabin prime test and PollardRho big number Decomposition
namespace PollardRho {
int64_t powModll(int64_t x, int64_t n, int64_t p);

// m - 1 = m * 2 ^ t, return false if test is invaild
bool witness(int64_t a, int64_t n, int64_t m, int t);
bool rabin(int64_t n);
int64_t pollardrho(int64_t n);
int64_t spf(int64_t n);
int64_t gpf(int64_t n, int64_t mxf = 1);
}  // namespace PollardRho


// find smallest non-negative $x$ s.t. $a^x = b \mod p$, or $-1$(assume $0^0 = 1$)
int babyStepGiantStep(int a, int b, int p);
// https://www.luogu.com.cn/problem/P4195

// find $x$ s.t. $x^2 = a \mod p$, or $-1$ in $O(\log^2 p)$ Tonelli-Shanks
int sqrtModpS(int a, int p);  // 0 <= a < p < std::numeric_limits<int>::max()

// find $x$ s.t. $x^2 = a \mod p$, or $-1$ in $O(\log p)$ Cipolla
int sqrtModp(int a, int p);
// https://www.luogu.com.cn/problem/P5491

// return all pair $(i, j, lcm(i, j)$  with lcm(i, j) <= n, $O(n \log^2 n)$
std::vector<std::tuple<int, int, int>> lcmPair(int n);
// https://ac.nowcoder.com/acm/contest/view-submission?submissionId=48024221&returnHomeType=1&uid=437678882

// $O(n \log n)$
template <typename T>
std::vector<T> DirichletProduct(const std::vector<T>& a, const std::vector<T>& b, int n) {
  std::vector<T> c(n + 1);
  for (int i = 1; i <= n; ++i) {
    for (int j = 1, ij = i; ij <= n; ij += i, ++j) {
      c[ij] += a[i] * b[j];
    }
  }
  return c;
}
// $O(n \log \log n)$, $new_a[n] = \sum_{d | n} old_a[d]$
template <typename T>
void mobiousTran(std::vector<T>& a, int n) {
  auto& p = Prime::Instance();
  for (int i = 1; p[i] <= n; ++i) {
    for (int j = 1; j * p[i] <= n; ++j) {
      a[j * p[i]] += a[j];
    }
  }
}
// $O(n \log \log n)$, $old_a[n] = \sum_{d | n} new_a[d]$
template <typename T>
void mobiousTranInv(std::vector<T>& a, int n) {
  auto& p = Prime::Instance();
  for (int i = p.primePi(n); i; --i) {
    for (int j = n / p[i]; j; --j) {
      a[j * p[i]] -= a[j];
    }
  }
}
// It is perfect simple and fast in $O(n \log n)$
template <typename T>
void mobiousTranInvS(std::vector<T>& a, int n) {
  for (int i = 1; i <= n; ++i) {
    for (int j = i * 2; j <= n; j += i) {
      a[j] -= a[i];
    }
  }
}
// $O(n \log n)$
template <typename T>
std::vector<T> DirichletRevProduct(const std::vector<T>& a, const std::vector<T>& b, int n) {
  std::vector<T> c(n + 1);
  for (int i = 1; i <= n; ++i) {
    for (int j = 1, ij = i; ij <= n; ij += i, ++j) {
      c[i] += a[ij] * b[j];
    }
  }
  return c;
}
// $O(n \log \log n)$, $new_a[d] = \sum_{d | n} old_a[n]$
template <typename T>
void mobiousRevTran(std::vector<T>& a, int n) {
  auto& p = Prime::Instance();
  for (int i = 1; p[i] <= n; ++i) {
    for (int j = n / p[i]; j; --j) {
      a[j] += a[j * p[i]];
    }
  }
}
// $O(n \log \log n)$, $old_a[d] = \sum_{d | n} new_a[n]$
template <typename T>
void mobiousRevTranInv(std::vector<T>& a, int n) {
  auto& p = Prime::Instance();
  for (int i = 1; p[i] <= n; ++i) {
    for (int j = 1; j * p[i] <= n; ++j) {
      a[j] -= a[j * p[i]];
    }
  }
}
// reference: https://www.cnblogs.com/ivorysi/p/8889154.html
// application: https://www.cnblogs.com/ImagineC/p/10557379.html

}  // namespace cuzperf
