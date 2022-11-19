#pragma once

#include <string>
#include <vector>

namespace cuzperf {

// suffix array: SA-IS in O(N)
// assume a.back() = 0 and other elements are small postiove integral
std::vector<int> SAIS(std::vector<int> a);

// the lexicographical smallest suffix is s[ans[0],...]
std::vector<int> SAIS(const std::string& s);
// https://www.luogu.com.cn/problem/P3809

std::vector<int> getHeight(const std::string& s);

int64_t diffSubstringCount(const std::string& s);
// https://www.luogu.com.cn/problem/P2408

}  // namespace cuzperf
