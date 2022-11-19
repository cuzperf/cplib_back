#pragma once

#include <string>
#include <vector>

namespace cuzperf {

std::vector<int> prefixFunction(std::string s);

// KMP based on prefixFunction. return all match postion in t
std::vector<int> kmp(std::string s, std::string t);

// ans[i] = count(s[0, ..., i] occur in s)
std::vector<int> countPrefix(std::string s);

// ans[i] = count(s[0, ..., i] occur in t)
std::vector<int> countPrefix(std::string s, std::string t);
// https://codeforces.com/problemset/problem/432/D

// z[i] = longest common prefix of s and s[i,...,n - 1], z[0] = 0.
std::vector<int> zFunction(std::string s);

// KMP based on zFunction. return all match postion in t
std::vector<int> kmpZ(std::string s, std::string t);

}  // namespace cuzperf
