#pragma once

#include <vector>
#include <functional>

namespace cuzperf {

// You must change mid to be random one or you will be hack
template<typename T> // don't use it
void quickSort(std::vector<T>& a) {
  std::function<void(int, int)> qSort = [&](int l, int r) {
    int i = l, j = r;
    auto mid = a[(l + r) / 2];
    while (i <= j) {
      while (i <= j && a[i] < mid) ++i;
      while (j >= i && a[j] > mid) --j;
      if (i <= j) {
        std::swap(a[i], a[j]);
        ++i;
        --j;
      }
    }
    if (i < r) qSort(i, r);
    if (l < j) qSort(l, j);
  };
  qSort(0, a.size() - 1);
}

// Shortest recursive relational formula: https://cmwqf.github.io/2020/07/18/%E6%B5%85%E8%B0%88Berlekamp-Massey%E7%AE%97%E6%B3%95/
template<typename valT>
static std::vector<valT> BerlekampMassey(const std::vector<valT>& a) {
  std::vector<valT> ans, lst;
  valT delta = 0;
  for (int i = 0, w = -1, n = (int)a.size(); i < n; ++i) {
    valT t = 0;
    for (int j = 0, na = ans.size(); j < na; ++j) {
      t += ans[j] * a[i - 1 - j];
    }
    if (t == a[i]) continue;
    // first time ans fail
    if (w == -1) {
      w = i; delta = a[i];
      ans.emplace_back(0);
      continue;
    }
    auto now = ans;
    auto mul = (a[i] - t) / delta;
    if (i - w + lst.size() > ans.size()) ans.resize(i - w + lst.size());
    ans[i - w - 1] += mul;
    for (int j = 0, lj = lst.size(); j < lj; ++j) ans[i - w + j] -= mul * lst[j];
    if ((int)now.size() - i < (int)lst.size() - w) {
      w = i; delta = a[i] - t; std::swap(now, lst);
    }
  }
  return ans;
}
// https://www.luogu.com.cn/problem/P5487

// it used in seq number for network that determines the order of number
// for instance if T = uint16_t, 1 < 2, 2 < 32768, 32768 < 1
template<typename T>
bool uless(T x, T y) {
  static_assert(std::is_unsigned_v<T>, "T must be unsigned");
  static constexpr T HalfT = T(1) << (std::numeric_limits<T>::digits - 1);
  return T(x - y) > HalfT;
}

template<int N>
struct Sieve {
  bool isP[N];
  // O(n log log n)
  constexpr Sieve() : isP() {
    isP[2] = true;
    for (int i = 3; i < N; i += 2) isP[i] = true;
    for (int i = 3, j = 9; j < N; i += 2, j = i * i) if (isP[i]) {
      while (j < N) isP[j] = false, j += i * 2;
    }
  }
};
bool fast_is_prime(int n);
constexpr bool isPrime(int n);
// https://codeforces.com/blog/entry/79941

namespace v0 {
// assmue that i  is odd, i * i > n twice is needed, don't need 1LL * i * i > n
template <int n, int i>
struct PrimeChecker {
  using type =
      std::conditional_t<(i * i > n), std::true_type,
          std::conditional_t<(n % i == 0), std::false_type,
              typename PrimeChecker<n, (i * i > n) ? -1 : i + 2>::type>>;
};
template <int n>
struct PrimeChecker<n, -1> {
  using type = void;
};

template<int n>
struct IsPrime {
  using type =
      std::conditional_t<(n < 2 || n % 2 == 0), std::false_type,
          typename PrimeChecker<n, 3>::type>;
};
template<>
struct IsPrime<2> : public std::true_type {};
}  // namespace v0
// https://stackoverflow.com/q/18303632/17276415

constexpr int fastSqrt(int v);
// http://www.azillionmonkeys.com/qed/ulerysqroot.pdf

namespace {
// assmue that i  is odd
template <int n, int i>
struct PrimeChecker {
  using type =
      std::conditional_t<(i == 1), std::true_type,
          std::conditional_t<(n % i == 0), std::false_type,
              typename PrimeChecker<n, i - 2>::type>>;
};
template <int n>
struct PrimeChecker<n, 1> : public std::true_type {};

template<int n>
struct IsPrime {
  using type = std::conditional_t<(n < 2 || n % 2 == 0), std::false_type,
              typename PrimeChecker<n, fastSqrt(n) | 1>::type>;
};
template<>
struct IsPrime<2> : public std::true_type {};
}  // namespace

// too slow and may have complier error in gcc11 for big n
constexpr bool isPrimeR(int n, int c);
constexpr bool isPrimeConstexpr(int n);
// https://stackoverflow.com/questions/18303632/compile-time-prime-checking

}  // namespace cuzperf
