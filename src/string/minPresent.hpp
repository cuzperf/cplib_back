#pragma once

#include <vector>

namespace cuzperf {

template<typename T>
int minPresent(std::vector<T>& a) {
  int k = 0, i = 0, j = 1, n = (int)a.size();
  while (k < n && i < n && j < n) {
    if (a[(i + k) % n] == a[(j + k) % n]) {
      ++k;
    } else {
      a[(i + k) % n] > a[(j + k) % n] ? i += k + 1 : j += k + 1;
      if (i == j) ++i;
      k = 0;
    }
  }
  return std::min(i, j);
}

}  // namespace cuzperf
