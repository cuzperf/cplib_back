#include "math/poly/polyAll.h"
#include "test/util.h"

namespace cuzperf {

TEST(MathTest, PolyS) {
  std::vector<int> a{1, 2};         // a = 1 + 2x
  std::vector<int> b{1, 2, 1};      // b = 1 + 2x + x^2
  std::vector<int> ab{1, 4, 5, 2};  // a * b = 1 + 4x + 5x^2 + 2x^3

  PolyS A(a), B(b);
  std::vector<int> c = A * B;
  EXPECT_EQ(c, ab);
}

TEST(MathTest, Poly) {}

}  // namespace cuzperf
