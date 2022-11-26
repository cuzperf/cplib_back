#pragma once

#include <array>
#include <map>
#include <vector>

#include "base/builtin.h"

namespace cuzperf {

class MexS {
  static inline const int B = 64;  // submit use 64bit
  std::vector<int64_t> cnt_;
  std::vector<std::vector<uint64_t>> a_;
  int ans_;

 public:
  // the answer is at most n
  MexS(int n) : cnt_(n + 1), ans_(-1) {
    int x = cnt_.size();
    while (x > B) {
      a_.emplace_back(std::vector<uint64_t>((x + B - 1) / B, -1ULL));
      x /= B;
    }
    a_.emplace_back(std::vector<uint64_t>{-1ULL});
  }
  void insert(int id) {
    if (id < 0 || id >= static_cast<int>(cnt_.size()) || cnt_[id]++) {
      return;
    }
    if (id == ans_) {
      ans_ = -1;
    }
    for (auto& x : a_) {
      int tid = id / B;
      x[tid] ^= 1ULL << (id - tid * B);
      if (x[tid]) {
        return;
      }
      id = tid;
    }
  }
  void erase(int id) {  // make sure there is an element in this set
    if (id < 0 || id >= static_cast<int>(cnt_.size()) || --cnt_[id]) {
      return;
    }
    if (id <= ans_) {
      ans_ = id;
    }
    for (auto& x : a_) {
      int tid = id / B;
      x[tid] ^= 1ULL << (id - tid * B);
      if (x[tid] != -1ULL) {
        return;
      }
      id = tid;
    }
  }
  int solve() {
    if (ans_ == -1) {
      ans_ = 0;
      for (auto it = a_.crbegin(); it != a_.crend(); ++it) {
        ans_ = ans_ * B + ctz_u64((*it)[ans_]);
      }
    }
    return ans_;
  }
};

class MEX {
  // B may need to be bigger
  static inline constexpr int B = 20;
  std::array<std::map<int, int>, B> mp;
  std::map<int, int> S;

 public:
  void insert(int x) {
    if (S[x]++) {
      return;
    }
    int mask = 0;
    for (int i = B - 1; i >= 0; --i) {
      mask |= 1 << i;
      ++mp[i][x & mask];
    }
  }
  void erase(int x) {
    if (--S[x]) {
      return;
    }
    S.erase(x);
    int mask = 0;
    for (int i = B - 1; i >= 0; --i) {
      mask |= 1 << i;
      --mp[i][x & mask];
    }
  }
  // find mex(a_i ^ x)
  int solve(int x = 0) {
    int mask = 0, r = 0;
    for (int i = B - 1; i >= 0; --i) {
      mask |= x & (1 << i);
      if (mp[i][mask] == (1 << i)) {
        mask ^= 1 << i;
        r |= 1 << i;
      }
    }
    return r;
  }
};

}  // namespace cuzperf
