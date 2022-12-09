#pragma once

#include <assert.h>
#include <stdint.h>

#include <iostream>

#include "base/builtin.h"
#include "math/basic.h"

namespace cuzperf {

// You should setMod before use it
class ModInt {
  static inline int M = 998244353;
  int n_;
  static int inv(int a) {
    auto [d, x, y] = exGcd(a, M);
    // assert(d == 1);
    return x < 0 ? x + M : x;
  }
  // assum M is prime
  static int invP(int x) { return x == 1 ? x : 1LL * (M - M / x) * invP(M % x) % M; }

 public:
  template <typename T>
  operator T() const {
    return static_cast<T>(n_);
  }
  static void setMod(int m) { M = m; }
  static int mod() { return M; }
  // assume 0 <= x < M
  static ModInt raw(int x) {
    ModInt A;
    A.n_ = x;
    return A;
  }
  ModInt() { n_ = 0; }
  ModInt(const int& x) : n_(x % M) {
    if (n_ < 0) {
      n_ += M;
    }
  }
  ModInt(const int64_t& x) : n_(x % M) {
    if (n_ < 0) {
      n_ += M;
    }
  }
  ModInt operator-() const { return n_ == 0 ? *this : raw(M - n_); }
  ModInt& operator++() {
    if (++n_ == M) {
      n_ = 0;
    }
    return *this;
  }
  ModInt& operator--() {
    if (n_-- == 0) {
      n_ += M;
    }
    return *this;
  }
  ModInt& operator+=(const ModInt& A) {
    n_ += A.n_;
    if (n_ >= M) {
      n_ -= M;
    }
    return (*this);
  }
  ModInt& operator-=(const ModInt& A) {
    n_ -= A.n_;
    if (n_ < 0) {
      n_ += M;
    }
    return (*this);
  }
  ModInt& operator*=(const ModInt& A) {
    n_ = 1LL * n_ * A.n_ % M;
    return (*this);
  }
  ModInt& operator/=(const ModInt& A) { return (*this) *= A.inv(); }
  ModInt operator+(const ModInt& A) const { return ModInt(*this) += A; }
  ModInt operator-(const ModInt& A) const { return ModInt(*this) -= A; }
  ModInt operator*(const ModInt& A) const { return ModInt(*this) *= A; }
  ModInt operator/(const ModInt& A) const { return ModInt(*this) /= A; }
  ModInt operator<<(int x) const {
    static constexpr int bits = 32;
    int64_t r = n_;
    while (x > bits) {
      x -= bits;
      r <<= bits;
      r %= M;
    }
    r <<= x;
    return ModInt(r);
  }
  ModInt& operator<<=(int x) { return (*this) = (*this) << x; }
  bool operator==(const ModInt& A) const { return n_ == A.n_; }
  bool operator!=(const ModInt& A) const { return n_ != A.n_; }
  ModInt inv() const { return inv(n_); }
  ModInt invP() const { return invP(n_); }
  friend ModInt pow(ModInt A, int n) {
    ModInt R(1);
    while (n) {
      if (n & 1) {
        R *= A;
      }
      n >>= 1;
      A *= A;
    }
    return R;
  }
  friend std::istream& operator>>(std::istream& in, ModInt& A) {
    int64_t x;
    in >> x;
    A = ModInt(x);
    return in;
  }
  friend std::ostream& operator<<(std::ostream& out, const ModInt& A) {
    out << A.n_;
    return out;
  }
};

// You should setMod before use it
class ModLL {
  static inline int64_t M = 998244353;
  int64_t n_;
  static int64_t inv(int64_t a) {
    auto [d, x, y] = exGcd(a, M);
    // assert(d == 1);
    return x < 0 ? x + M : x;
  }
  // assum M is prime
  static int64_t invP(int64_t x) { return x == 1 ? x : mulModi(M - M / x, invP(M % x), M); }

 public:
  template <typename T>
  operator T() const {
    return static_cast<T>(n_);
  }
  static void setMod(int64_t m) { M = m; }
  static int64_t mod() { return M; }
  // assume 0 <= x < M
  static ModLL raw(int64_t x) {
    ModLL A;
    A.n_ = x;
    return A;
  }
  ModLL() { n_ = 0; }
  ModLL(const int& x) : n_(x % M) {
    if (n_ < 0) {
      n_ += M;
    }
  }
  ModLL(const int64_t& x) : n_(x % M) {
    if (n_ < 0) {
      n_ += M;
    }
  }
#ifdef __GNUC__
  ModLL(const __int128_t& x) : n_(x % M) {
    if (n_ < 0) {
      n_ += M;
    }
  }
#endif
  ModLL operator-() const { return n_ == 0 ? *this : raw(M - n_); }
  ModLL& operator++() {
    if (++n_ == M) {
      n_ = 0;
    }
    return *this;
  }
  ModLL& operator--() {
    if (n_-- == 0) {
      n_ += M;
    }
    return *this;
  }
  ModLL& operator+=(const ModLL& A) {
    n_ += A.n_;
    if (n_ >= M) {
      n_ -= M;
    }
    return (*this);
  }
  ModLL& operator-=(const ModLL& A) {
    n_ -= A.n_;
    if (n_ < 0) {
      n_ += M;
    }
    return (*this);
  }
  ModLL& operator*=(const ModLL& A) {
    n_ = mulModi(n_, A.n_, M);
    return (*this);
  }
  ModLL& operator/=(const ModLL& A) { return (*this) *= A.inv(); }
  ModLL operator+(const ModLL& A) const { return ModLL(*this) += A; }
  ModLL operator-(const ModLL& A) const { return ModLL(*this) -= A; }
  ModLL operator*(const ModLL& A) const { return ModLL(*this) *= A; }
  ModLL operator/(const ModLL& A) const { return ModLL(*this) /= A; }
  ModLL operator<<(int x) const {
    static constexpr int bits = 62;
    int64_t r = n_;
    while (x > bits) {
      x -= bits;
      r = mulModi(r, 1ULL << 62, M);
    }
    r = mulModi(r, 1ULL << x, M);
    return r;
  }
  ModLL& operator<<=(int x) { return (*this) = (*this) << x; }
  bool operator==(const ModLL& A) const { return n_ == A.n_; }
  bool operator!=(const ModLL& A) const { return n_ != A.n_; }
  ModLL inv() const { return inv(n_); }
  ModLL invP() const { return invP(n_); }
  friend ModLL pow(ModLL A, int64_t n) {
    ModLL R(1);
    while (n) {
      if (n & 1) {
        R *= A;
      }
      n >>= 1;
      A *= A;
    }
    return R;
  }
  friend std::istream& operator>>(std::istream& in, ModLL& A) {
    int64_t x;
    in >> x;
    A = ModLL(x);
    return in;
  }
  friend std::ostream& operator<<(std::ostream& out, const ModLL& A) {
    out << A.n_;
    return out;
  }
};


template <int N>
class MInt {
  static inline constexpr int M = N;
  int n_;
  static int inv(int a) {
    auto [d, x, y] = exGcd(a, M);
    // assert(d == 1);
    return x < 0 ? x + M : x;
  }
  // assum M is prime
  static int invP(int x) { return x == 1 ? x : 1LL * (M - M / x) * invP(M % x) % M; }

 public:
  template <typename T>
  operator T() const {
    return static_cast<T>(n_);
  }
  static void setMod(int m [[maybe_unused]]) { assert(M == m); }
  static constexpr int mod() { return M; }
  // assume 0 <= x < M
  static MInt raw(int x) {
    MInt A;
    A.n_ = x;
    return A;
  }
  MInt() { n_ = 0; }
  MInt(const int& x) : n_(x % M) {
    if (n_ < 0) {
      n_ += M;
    }
  }
  MInt(const int64_t& x) : n_(x % M) {
    if (n_ < 0) {
      n_ += M;
    }
  }
  MInt operator-() const { return n_ == 0 ? *this : raw(M - n_); }
  MInt& operator++() {
    if (++n_ == M) {
      n_ = 0;
    }
    return *this;
  }
  MInt& operator--() {
    if (n_-- == 0) {
      n_ += M;
    }
    return *this;
  }
  MInt& operator+=(const MInt& A) {
    n_ += A.n_;
    if (n_ >= M) {
      n_ -= M;
    }
    return (*this);
  }
  MInt& operator-=(const MInt& A) {
    n_ -= A.n_;
    if (n_ < 0) {
      n_ += M;
    }
    return (*this);
  }
  MInt& operator*=(const MInt& A) {
    n_ = 1LL * n_ * A.n_ % M;
    return (*this);
  }
  MInt& operator/=(const MInt& A) { return (*this) *= A.inv(); }
  MInt operator+(const MInt& A) const { return MInt(*this) += A; }
  MInt operator-(const MInt& A) const { return MInt(*this) -= A; }
  MInt operator*(const MInt& A) const { return MInt(*this) *= A; }
  MInt operator/(const MInt& A) const { return MInt(*this) /= A; }
  MInt operator<<(int x) const {
    static constexpr int bits = 32;
    int64_t r = n_;
    while (x > bits) {
      x -= bits;
      r <<= bits;
      r %= M;
    }
    r <<= x;
    return MInt(r);
  }
  MInt& operator<<=(int x) { return (*this) = (*this) << x; }
  bool operator==(const MInt& A) const { return n_ == A.n_; }
  bool operator!=(const MInt& A) const { return n_ != A.n_; }
  MInt inv() const { return inv(n_); }
  MInt invP() const { return invP(n_); }
  friend MInt pow(MInt A, int n) {
    MInt R(1);
    while (n) {
      if (n & 1) {
        R *= A;
      }
      n >>= 1;
      A *= A;
    }
    return R;
  }
  friend std::istream& operator>>(std::istream& in, MInt& A) {
    int64_t x;
    in >> x;
    A = MInt(x);
    return in;
  }
  friend std::ostream& operator<<(std::ostream& out, const MInt& A) {
    out << A.n_;
    return out;
  }
};

// valT
template <class T>
struct is_MInt : std::false_type {};

template <int M>
struct is_MInt<MInt<M>> : std::true_type {};

template <class T>
inline constexpr bool is_mint_v = is_MInt<T>::value;

template <typename T>
using ModT =
    std::enable_if_t<std::is_same_v<ModLL, T> || std::is_same_v<ModInt, T> || is_mint_v<T>>;

}  // namespace cuzperf
