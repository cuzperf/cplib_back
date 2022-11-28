#include "math/mod.hpp"

#include "test/util.h"

namespace cuzperf {

TEST(MathTest, modInt) {

}

TEST(MathTest, modLL) {

}


TEST(MathTest, mInt) {
  using mint1123 = MInt<1123>;
  mint1123 A{123}, B{321};
  --A;
  EXPECT_EQ(A, mint1123(122));
  A *= B;
  EXPECT_EQ(A, mint1123(980));
  A += B;
  EXPECT_EQ(A, mint1123(178));
  A /= B;
  EXPECT_EQ(A, mint1123(123));


  EXPECT_EQ(mint1123(2).inv(), mint1123(562));
  EXPECT_EQ(mint1123(3).inv(), mint1123(749));
}

}  // namespace cuzperf
