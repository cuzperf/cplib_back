#include "string/kmp.h"

namespace cuzperf {

std::vector<int> prefixFunction(std::string s) {
  int n = static_cast<int>(s.size());
  std::vector<int> p(n);
  for (int i = 1; i < n; ++i) {
    int j = p[i - 1];
    while (j > 0 && s[i] != s[j]) {
      j = p[j - 1];
    }
    if (s[i] == s[j]) {
      ++j;
    }
    p[i] = j;
  }
  return p;
}

std::vector<int> kmp(std::string s, std::string t) {
  std::vector<int> ans;
  int n = static_cast<int>(s.size()), m = static_cast<int>(t.size());
  if (n > m) {
    return ans;
  }
  auto p = prefixFunction(s);
  for (int i = 0, j = 0; i < m; ++i) {
    while (j > 0 && s[j] != t[i]) {
      j = p[j - 1];
    }
    if (s[j] == t[i] && ++j == n) {
      ans.emplace_back(i - n + 1);
    }
  }
  return ans;
}

std::vector<int> countPrefix(std::string s) {
  auto p = prefixFunction(s);
  int n = static_cast<int>(s.size());
  std::vector<int> ans(n + 1);
  for (auto x : p) {
    ++ans[x];
  }
  for (int i = n - 1; i > 0; --i) {
    ans[p[i - 1]] += ans[i];
  }
  for (int i = 0; i <= n; ++i) {
    ++ans[i];
  }
  return ans;
}

std::vector<int> countPrefix(std::string s, std::string t) {
  auto p = prefixFunction(s);
  int n = static_cast<int>(s.size()), m = static_cast<int>(t.size());
  std::vector<int> ans(n + 1);
  for (int i = 0, j = 0; i < m; ++i) {
    while (j > 0 && t[i] != s[j]) {
      j = p[j - 1];
    }
    if (t[i] == s[j]) {
      ++j;
    }
    ++ans[j];
  }
  ++ans[0];
  for (int i = n; i > 0; --i) {
    ans[p[i - 1]] += ans[i];
  }
  return ans;
}

std::vector<int> zFunction(std::string s) {
  int n = static_cast<int>(s.size());
  std::vector<int> z(n);
  for (int i = 1, l = 0, r = 0; i < n; ++i) {
    if (i <= r && z[i - l] < r - i + 1) {
      z[i] = z[i - l];
    } else {
      int j = std::max(0, r - i + 1);
      while (i + j < n && s[j] == s[i + j]) {
        ++j;
      }
      z[i] = j;
      if (i + j - 1 > r) {
        l = i;
        r = i + j - 1;
      }
    }
  }
  return z;
}

std::vector<int> kmpZ(std::string s, std::string t) {
  std::vector<int> ans;
  int n = static_cast<int>(s.size()), m = static_cast<int>(t.size());
  if (n > m) {
    return ans;
  }
  auto z = zFunction(s);
  for (int i = 0, l = 0, r = -1; i < m; ++i) {
    if (i > r || z[i - l] >= r - i + 1) {
      int j = std::max(0, r - i + 1);
      while (j < n && i + j < m && s[j] == t[i + j]) {
        ++j;
      }
      if (j == n) {
        ans.emplace_back(i);
      }
      if (i + j - 1 > r) {
        l = i;
        r = i + j - 1;
      }
    }
  }
  return ans;
}

}  // namespace cuzperf
