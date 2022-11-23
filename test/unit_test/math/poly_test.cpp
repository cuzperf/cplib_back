#include "test/util.h"

#include "math/poly/polyAll.h"

namespace cuzperf {

TEST(MathTest, PolyS) {
  std::vector<int> a{1, 2};
  std::vector<int> b{1, 2, 1};
  std::vector<int> ab{1, 4, 5, 2};
  PolyS A(a), B(b);
  std::vector<int> c = A * B;
  EXPECT_EQ(c, ab);
}

}  // namespace cuzperf
