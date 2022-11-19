#pragma once

#include <stdint.h>

#include <utility>
#include <vector>

namespace cuzperf {

// $O(m \sqrt{m})$, we will get TLE if the answer greater than std::numeric_limits<int>::max()
int circle3count(const std::vector<std::pair<int, int>>& edge, int n);
// https://www.luogu.com.cn/problem/P1989

// $O(m \sqrt{m})$
int64_t circle4count(const std::vector<std::pair<int, int>>& edge, int n);
// https://www.luogu.com.cn/blog/221955/san-yuan-huan-si-yuan-huan-ji-shuo

}  // namespace cuzperf
