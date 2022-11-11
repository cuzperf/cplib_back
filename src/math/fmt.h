#pragma once

#include <vector>

namespace cuzperf {
namespace FMT {

std::vector<int> And(std::vector<int> a, std::vector<int> b);
std::vector<int> Or(std::vector<int> a, std::vector<int> b);
std::vector<int> Xor(std::vector<int> a, std::vector<int> b);
// i = j | k and j & k = 0
std::vector<int> OrAnd(std::vector<int> a, std::vector<int> b);

}  // namespace FMT
// https://www.luogu.com.cn/problem/P6097

}  // namespace cuzperf
