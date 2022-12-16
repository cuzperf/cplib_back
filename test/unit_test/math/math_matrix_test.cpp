#include "math/matrix.hpp"

#include "test/util.h"

#include "math/mod.hpp"

namespace cuzperf {

static constexpr int M = 998255353;
using mod = MInt<M>;

static mod Fibonacci(int n) {
  if (n < 2) return n;
  std::vector<mod> f(n + 1);
  f[1] = 1;
  for (int i = 2; i <= n; ++i) {
    f[i] = f[i - 1] + f[i - 2];
  }
  return f[n];
}

static mod FibonacciMatrix(int n) {
  if (n < 2) return n;
  using matrixMod = Matrix<2, mod>;
  // (f_{n + 1}, f_n}) = (f1, f0) A^n
  matrixMod A(std::vector<std::vector<mod>>{{1, 1}, {1, 0}});
  return pow(A, n).a_[0][1];
}

TEST(MathTest, matrix) {
  using matrix3 = Matrix<3, int>;
  matrix3 A(std::vector<std::vector<int>>{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
  auto B = A;
  B *= A;
  auto a2 = std::vector<std::vector<int>>{{30, 36, 42}, {66, 81, 96}, {102, 126, 150}};
  EXPECT_EQ(B.getOrigin(), a2);

  B = pow(A, 5);
  auto a5 = std::vector<std::vector<int>>{{121824, 149688, 177552}, {275886, 338985, 402084}, {429948, 528282, 626616}};
  EXPECT_EQ(B.getOrigin(), a5);

  {
    for (int i = 0; i < RUN_CNT; ++i) {
      EXPECT_EQ(Fibonacci(i), FibonacciMatrix(i));
    }
    static constexpr int N = 1e5;
    EXPECT_EQ(Fibonacci(N), FibonacciMatrix(N));
  }
}

}  // namespace cuzperf
