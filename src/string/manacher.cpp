#include "string/manacher.h"

#include <algorithm>

namespace cuzperf {

std::vector<int> Manacher(std::string s) {
  int n = static_cast<int>(s.size());  // assume n is odd
  std::vector<int> d(n);
  for (int i = 0, l = 0, r = -1; i < n; ++i) {
    int k = i > r ? 1 : std::min(d[l + r - i], r - i);
    while (k <= i && i + k < n && s[i - k] == s[i + k]) {
      ++k;
    }
    d[i] = k--;
    if (i + k > r) {
      l = i - k;
      r = i + k;
    }
  }
  return d;
}

}  // namespace cuzperf
