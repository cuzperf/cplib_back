#include "test/util.h"

#include "math/poly/polyAll.h"

namespace cuzperf {

TEST(MathTest, PolyS) {
  std::vector<int> a{18, 2, 6, 17, 7, 19, 17, 6, 2, 12, 14};
  std::vector<int> b{4, 15, 5, 20, 2, 6, 20, 12, 16, 5};
  PolyS A(a), B(b);
  auto C = A * B;

}

}  // namespace cuzperf
