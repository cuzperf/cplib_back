#include "ds/ds.h"

#include "test/util.h"

namespace cuzperf {

TEST(DsTest, fenwick) {
  {
    std::vector<int> a{1, 3, 9, 30};
    Fenwick<int> A(a);
    auto b = A.getOrigin();
    EXPECT_EQ(a, b);
    EXPECT_EQ(A.sum(0), 0);
    EXPECT_EQ(A.sum(1), 1);
    EXPECT_EQ(A.sum(2), 4);
    EXPECT_EQ(A.sum(3), 13);
    EXPECT_EQ(A.sum(4), 43);
    EXPECT_EQ(A.sum(10086), 43);
    A.add(2, 100);
    EXPECT_EQ(A.sum(2, 3), 109);
    EXPECT_EQ(A.sum(10010), 143);
  }
  {
    const int n = 1e5 + 2;
    std::vector<int64_t> a(n);
    std::iota(a.begin(), a.end(), 0LL);
    Fenwick<int64_t> A(a);
    for (int i = 1; i <= n; ++i) {
      EXPECT_EQ(A.sum(i), 1LL * i * (i - 1) / 2);
    }
  }
}

TEST(DS, fenwick_min) {}

}  // namespace cuzperf
