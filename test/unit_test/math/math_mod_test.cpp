#include "math/mod.hpp"

#include "test/util.h"

namespace cuzperf {

template<typename T>
static void fun1123() {
  T A{123}, B{321};
  --A;
  EXPECT_EQ(A, T(122));
  A *= B;
  EXPECT_EQ(A, T(980));
  A += B;
  EXPECT_EQ(A, T(178));
  A /= B;
  EXPECT_EQ(A, T(123));

  EXPECT_EQ(T(2).inv(), T(562));
  EXPECT_EQ(T(3).inv(), T(749));
}

TEST(MathTest, modInt) {
  ModInt::setMod(1123);
  fun1123<ModInt>();
}

TEST(MathTest, modLL) {
  ModLL::setMod(1123);
  fun1123<ModLL>();
}


TEST(MathTest, mInt) {
  fun1123<MInt<1123>>();
  // mint1123 A{123}, B{321};
  // --A;
  // EXPECT_EQ(A, mint1123(122));
  // A *= B;
  // EXPECT_EQ(A, mint1123(980));
  // A += B;
  // EXPECT_EQ(A, mint1123(178));
  // A /= B;
  // EXPECT_EQ(A, mint1123(123));


  // EXPECT_EQ(mint1123(2).inv(), mint1123(562));
  // EXPECT_EQ(mint1123(3).inv(), mint1123(749));
}

}  // namespace cuzperf
