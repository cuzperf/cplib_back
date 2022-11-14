#pragma once

#include <utility>
#include <vector>
#include <tuple>

namespace cuzperf {

template<typename T>
T floor(T a, T n) {
  if (n < 0) {
    n = -n;
    a = -a;
  }
  return a < 0 ? (a - n + 1) / n : a / n;
}
template<typename T>
T ceil(T a, T n) {
  if (n < 0) {
    n = -n;
    a = -a;
  }
  return a < 0 ? a / n : (a + n - 1) / n;
}

// never mixed it with cin and cout, useful for int128
template<typename T>
class FastIntIO {
 public:
  static T read(){
    T x = 0;
    bool negative = false;
    // you may use buffer instead for speed
    char ch = getchar();
    while (ch < '0' || ch > '9'){
      if (ch == '-') negative = true;
      ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
      x = x * 10 + ch - '0';
      ch = getchar();
    }
    return negative ?  -x : x;
  }
  static void print(T x){
    if (x < 0) {
      putchar('-');
      x = -x;
    }
    printCore(x);
  }
 private:
  static void printCore(T x){
    if (x > 9) printCore(x / 10);
    putchar(x % 10 + '0');
  }
};

// return [d, x, y] such that d = ax + by = gcd(a, b) with |x| <= b and |y| <= a if a, b > 0
namespace {
template<typename T>
std::tuple<T, T, T> exGcd_(T a, T b) {
  if (b == 0) return {a, 1, 0};
  auto [d, y, x] = exGcd_(b, a % b);
  return {d, x, y - a / b * x};
}
}
template<typename T>
std::tuple<T, T, T> exGcd(T a, T b) {
  auto [d, x, y] = exGcd_(std::abs(a), std::abs(b));
  if (a < 0) { x = -x; }
  if (b < 0) { y = -y; }
  return {d, x, y};
}

// @return $x^n \mod M$ in $O(\log n)$
// @param $n \geq 0$ and $M \geq 0$
int powMod(int x, int n, int M);

// @return x such that $1 \equiv ax \mod n$
// @param gcd(a, n) = 1
int inv(int a, int n);

// slightly faster than std::gcd, but not recommend
int64_t gcd(int64_t a, int64_t b);
// https://cp-algorithms.com/algebra/euclid-algorithm.html

// Chinese remainder theorem: $x = ai \mod mi, m_i > 0$
std::pair<int64_t, int64_t> crt(int a1, int m1, int a2, int m2);

// $O(N \log N)$ smalleset prime factor(may be faster)
std::vector<int> spfS(int N);
// $O(N)$ smallest prime factor
std::vector<int> spf(int N);

// $O(N)$ none square factor
std::vector<int> nsf(int N);

// $O(N)$ none square factor using spf
std::vector<int> nsfS(int N);

}  // namespace cuzperf
