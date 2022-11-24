#include "util.h"

#include "ds/listVector.hpp"

namespace cuzperf {

TEST(DsTest, Vector2List) {
  std::vector<int> a{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto root = Vector2List(a);
  auto cur = root;
  for (int i = 0, n = a.size(); i < n; ++i) {
    EXPECT_TRUE(cur);
    EXPECT_EQ(cur->val, a[i]);
    cur = cur->next;
  }
  clearList(root);
}

TEST(DsTest, List2Vector) {
  ListNode<int>* root = nullptr;
  for (int i = 0; i < 10; ++i) {
    root = new ListNode(9 - i, root);
  }
  std::vector<int> a(10);
  auto b = List2Vector(root);
  std::iota(a.begin(), a.end(), 0);
  EXPECT_EQ(a, b);
  clearList(root);
}

TEST(DsTest, VectorList) {
  constexpr int N = 1e4;
  std::vector<int> a(N);
  for (int i = 0; i < N; ++i) a[i] = rnd();
  auto root = Vector2List(a);
  auto b = List2Vector(root);
  EXPECT_EQ(a, b);
  clearList(root);
}

}  // namespace cuzperf
