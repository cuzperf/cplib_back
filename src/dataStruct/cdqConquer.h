#pragma once

#include <vector>

namespace cuzperf {

// CDQ divided and conquer for partial order of dimension 3
struct cdqNode {
  int x, y, z, id, w;
  bool operator<(const cdqNode& A) const {
    if (x == A.x) return y == A.y ? z < A.z : y < A.y;
    return x < A.x;
  }
};
// ans[i] is the number of element less or equal to a[i]
std::vector<int> cdq(std::vector<cdqNode>& a, int k);
// https://www.luogu.com.cn/problem/P3810

}  // namespace cuzperf
