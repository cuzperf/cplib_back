#include "duval.h"

namespace cuzperf {

std::vector<std::string> duval(const std::string& s) {
  std::vector<std::string> r;
  int n = (int)s.size(), i = 0;
  while (i < n) {
    int j = i + 1, k = i;
    while (j < n && s[k] <= s[j]) {
      if (s[k] < s[j]) k = i;
      else ++k;
      ++j;
    }
    while (i <= k) {
      r.emplace_back(s.substr(i, j - k));
      i += j - k;
    }
  }
  return r;
}

}  // namespace cuzperf
