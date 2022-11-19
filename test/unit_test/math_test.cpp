#include "math/math.h"

#include <bits/stdc++.h>

#include "gtest/gtest.h"

namespace cuzperf {

constexpr static int TIME = 10;
static std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());
static std::mt19937_64 rnd64(std::chrono::steady_clock::now().time_since_epoch().count());
constexpr static int M = 998244353;
constexpr static int M7 = 1e7 + 7;
constexpr static int M9 = 1e9 + 9;

TEST(MATH, powMod) {
  EXPECT_EQ(powMod(0, 123456, 3), 0);
  EXPECT_EQ(powMod(2, 30, M), 1 << 30);

  // a^{p - 1} mod p = 1
  for (int i = 0; i < TIME; ++i) {
    EXPECT_EQ(powMod(std::abs(static_cast<int>(rnd() % (M - 1))) + 1, M - 1, M), 1);
  }
}

TEST(MATH, inv) {
  EXPECT_EQ(inv(1, 2), 1);
  EXPECT_EQ(inv(2, 3), 2);

  for (int i = 0; i < TIME; ++i) {
    int x = std::abs(static_cast<int>(rnd() % (M - 1)));
    EXPECT_EQ(inv(x, M), powMod(x, M - 2, M));
  }
}

TEST(MATH, gcd) {
  EXPECT_EQ(gcd(2, 3), 1);
  EXPECT_EQ(gcd(6, 8), 2);

  for (int i = 0; i < TIME; ++i) {
    auto x = rnd64(), y = rnd64();
    EXPECT_EQ(gcd(x, y), std::gcd(x, y));
  }
}

TEST(MATH, crt) {
  {
    auto [x, y] = crt(1, 3, 2, 6);
    EXPECT_EQ(x, 2);
    EXPECT_EQ(y, 6);
  }
  {
    auto [x, y] = crt(1, 3, 1, 7);
    EXPECT_EQ(x, 1);
    EXPECT_EQ(y, 21);
  }
  {
    auto [x, y] = crt(2, 6, 6, 14);
    EXPECT_EQ(x, 20);
    EXPECT_EQ(y, 42);
  }

  const auto MM = static_cast<int64_t>(M7) * M9;
  for (int i = 0; i < TIME; ++i) {
    int64_t xx = rnd64();
    xx = std::abs(xx % M);
    auto [x, y] = crt(xx % M7, M7, xx % M9, M9);
    EXPECT_EQ(xx, x);
    EXPECT_EQ(y, MM);
  }
}

}  // namespace cuzperf
