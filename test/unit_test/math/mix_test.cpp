#include "test/util.h"

#include "math/mix.h"

namespace cuzperf {

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

TEST(MathTest, quickSortStable) {

}


}  // namespace cuzperf
