#pragma once

#include <assert.h>
#include <stdint.h>

#include <algorithm>
#include <numeric>
#include <queue>
#include <stack>
#include <vector>

namespace cuzperf {

// a will becomes next lexicographical order of a, satisfies $-1 < a_0 < a_1 < \cdots, a_{n - 1} <
// mx$
bool nextBinom(std::vector<int>& a, int mx);

// total number binom{mx}{n}
void bruteForceBinom(int n, int mx);

// length of longest increasing subsquence
int LIS(std::vector<int>& a);
// length of longest non-decreasing subsquence
int LNDS(std::vector<int>& a);

// longest non-decreasing subsquence, return choosen index
std::stack<int> LISP(const std::vector<int>& a);

// monicDeque: index of every max element of SubInterval of length m
std::vector<int> monicDequeMax(std::vector<int>& a, int m);

// f is index of a such that $a_{f_0} < a_{f_1} < a_{f_m}$
std::vector<int> monicStack(const std::vector<int>& a);
// https://www.luogu.com.cn/problem/P5788

// Cartesian Tree
struct cNode {
  int id, val, par, ch[2];
  void init(int _id, int _val, int _par) { id = _id, val = _val, par = _par, ch[0] = ch[1] = 0; }
};
int cartesian_build(std::vector<cNode>& tree, int n);
// https://codeforces.com/contest/1490/problem/D

// we can only use the ideal of merge sort
int64_t inverseOrderCount(std::vector<int> a);
// https://codeforces.com/contest/1602/problem/E


// Error Correction Code: O(n_ m_ + k_^k_ n_)
class ECC {
  std::vector<std::vector<int>> a_;    // origin data: n rows, m cols.
  int k_;                              // Maximum number of differences allowed
  std::vector<std::vector<int>> bad_;  // difference with current answer
  int n_, m_, mxId_;
  void updateMxId(int i) {
    if (bad_[i].size() > bad_[mxId_].size()) {
      mxId_ = i;
    }
  }
  bool dfs(int c) {
    auto bd = bad_[mxId_];
    if ((int)bd.size() <= k_) {
      return true;
    }
    if ((int)bd.size() - k_ > c) {
      return false;
    }
    // Note that bd is O(k_) instead of O(m_)
    std::vector<int> f(bd.size() - k_);
    std::iota(f.begin(), f.end(), 0);
    int tMxId = mxId_;
    do {
      mxId_ = tMxId;
      std::queue<int> tmp;
      for (auto x : f) {
        tmp.push(r_[bd[x]]);
        for (int i = 0; i < n_; ++i) {
          if (a_[i][bd[x]] == r_[bd[x]]) {
            bad_[i].emplace_back(bd[x]);
          }
          if (a_[i][bd[x]] == a_[mxId_][bd[x]]) {
            bad_[i].erase(std::find(bad_[i].begin(), bad_[i].end(), bd[x]));
          }
        }
        r_[bd[x]] = a_[mxId_][bd[x]];
      }
      for (int i = 0; i < n_; ++i) {
        updateMxId(i);
      }
      if (dfs(c - f.size())) {
        return true;
      }
      for (auto x : f) {
        for (int i = 0; i < n_; ++i) {
          if (a_[i][bd[x]] == r_[bd[x]]) {
            bad_[i].emplace_back(bd[x]);
          }
          if (a_[i][bd[x]] == tmp.front()) {
            bad_[i].erase(std::find(bad_[i].begin(), bad_[i].end(), bd[x]));
          }
        }
        r_[bd[x]] = tmp.front();
        tmp.pop();
      }
    } while (nextBinom(f, bd.size()));
    return false;
  }

 public:
  std::vector<int> r_;  // m_ cols vector, current answer
  ECC(const std::vector<std::vector<int>>& a) : a_(a) {
    n_ = a_.size();
    assert(n_ > 0);
    r_ = a_[0];
    m_ = r_.size();
    bad_.resize(n_);
    mxId_ = 0;
    for (int i = 0; i < n_; ++i) {
      for (int j = 0; j < m_; ++j) {
        if (a_[i][j] != r_[j]) {
          bad_[i].emplace_back(j);
        }
      }
      updateMxId(i);
    }
  }
};

}  // namespace cuzperf
