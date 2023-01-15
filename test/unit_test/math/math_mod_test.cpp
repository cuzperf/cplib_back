#include "math/mod.hpp"
#include "test/util.h"

namespace cuzperf {

template <typename T>
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


TEST(MathTest, mInt) { fun1123<MInt<1123>>(); }

}  // namespace cuzperf
