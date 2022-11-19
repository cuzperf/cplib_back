#pragma once

#include <string>
#include <vector>

namespace cuzperf {

// s(i - ans[i], i + ans[i])  is longest Palindrome
std::vector<int> Manacher(std::string s);
// add a special char to deal with the case of even length

}  // namespace cuzperf
