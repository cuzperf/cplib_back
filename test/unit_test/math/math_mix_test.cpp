#include "math/mix.h"

#include "test/util.h"

namespace cuzperf {

static int xorSubsetSum_test(std::vector<int> a, int mod) {
  int n = a.size();
  int ans = 0;
  for (int i = 0; i < (1 << n); ++i) {
    int x = 0;
    for (int j = 0; j < n; ++j) if (i & (1 << j)) {
      x ^= a[j];
    }
    ans = (ans + x) % mod;
  }
  return ans;
}

TEST(MathTest, xorSubsetSum) {
  {
    std::vector<int> a;
    EXPECT_EQ(xorSubsetSum(a, 123), 0);
  }
  {
    std::vector<int> a{1};
    EXPECT_EQ(xorSubsetSum(a, 123), 1);
  }
  {
    std::vector<int> a{1, 2};
    EXPECT_EQ(xorSubsetSum(a, 123), 6);
  }
  {
    std::vector<int> a{1, 3, 9};
    EXPECT_EQ(xorSubsetSum(a, 123), 44);
  }
  {
    constexpr int N = 10;
    constexpr int M = 998244353;
    std::vector<int> a(N);
    for (int i = 0; i < N; ++i) a[i] = rnd() % M;
    EXPECT_EQ(xorSubsetSum(a, M), xorSubsetSum_test(a, M));
  }
}

TEST(MathTest, quickSort) {
  {
    std::vector<int> a{1, 1, 1};
    quickSort(a);
    auto sa = a;
    EXPECT_EQ(a, sa);
  }
  {
    std::vector<int> a{3, 1, 4, 10, 2};
    quickSort(a);
    std::vector<int> sa{1, 2, 3, 4, 10};
    EXPECT_EQ(a, sa);
  }
  {
    std::vector<int> a{3, 1, 4, 10, 2};
    quickSort(a);
    std::vector<int> sa{1, 2, 3, 4, 10};
    EXPECT_EQ(a, sa);
  }
}

namespace {
struct Node {
  int x, y;
  bool operator<(const Node& rhs) const { return x < rhs.x; }
  bool operator==(const Node& rhs) const { return x == rhs.x && y == rhs.y; }
};
}  // namespace

TEST(MathTest, quickSortStable) {
  {
    std::vector<Node> a{{1, 2}, {1, 3}, {2, 2}, {1, 3}, {1, 4}};
    std::vector<Node> sa{{1, 2}, {1, 3}, {1, 3}, {1, 4}, {2, 2}};
    quickSortStable(a);
    EXPECT_EQ(a, sa);
  }
  {
    std::vector<Node> a{{3, 2}, {3, 3}, {2, 2}, {2, 3}, {1, 4}, {1, 1}};
    std::vector<Node> sa{{1, 4}, {1, 1}, {2, 2}, {2, 3}, {3, 2}, {3, 3}};
    quickSortStable(a);
    EXPECT_EQ(a, sa);
  }
}

}  // namespace cuzperf
