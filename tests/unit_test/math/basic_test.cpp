#include "math/basic.h"
#include "tests/test_util.h"

namespace cuzperf {

constexpr static int M = 998244353;

TEST(MathTest, powMod) {
  EXPECT_EQ(powMod(0, 123456, 3), 0);

  for (int i = 0; i < 30; ++i) {
    EXPECT_EQ(powMod(2, i, M), 1 << i);
  }

  // a^{p - 1} mod p = 1
  for (int i = 0; i < RUN_CNT; ++i) {
    EXPECT_EQ(powMod(std::abs(static_cast<int>(rnd() % (M - 1))) + 1, M - 1, M), 1);
  }
}

}  // namespace cuzperf
