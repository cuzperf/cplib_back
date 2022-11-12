#pragma once

#include <stdint.h>
#include <vector>
#include <algorithm>
#include <stack>

namespace cuzperf {

// a will becomes next lexicographical order of a, satisfies $-1 < a_0 < a_1 < \cdots, a_{n - 1} < mx$
bool nextBinom(std::vector<int>& a, int mx);

// total number binom{mx}{n}
void bruteForceBinom(int n, int mx);

// Error Correction Code: O(n_ m_ + k_^k_ n_)
class ECC {
  std::vector<std::vector<int>> a_;   // origin data: n rows, m cols.
  int k_;                             // Maximum number of differences allowed
  std::vector<std::vector<int>> bad_; // difference with current answer
  int n_, m_, mxId_;
  void updateMxId(int i) {
    if (bad_[i].size() > bad_[mxId_].size()) mxId_ = i;
  }
  bool dfs(int c);
 public:
  std::vector<int> r_;   // m_ cols vector, current answer
  ECC(const std::vector<std::vector<int>>& a);
};


// length of longest increasing subsquence
int LIS(std::vector<int>& a);
// length of longest non-decreasing subsquence
int LNDS(std::vector<int>& a);

// longest non-decreasing subsquence, return choosen index
template<typename T>
std::stack<int> LISP(const std::vector<T>& a);

// monicDeque: index of every max element of SubInterval of length m
std::vector<int> monicDequeMax(std::vector<int>& a, int m);

// f is index of a such that $a_{f_0} < a_{f_1} < a_{f_m}$
std::vector<int> monicStack(const std::vector<int>& a);
// https://www.luogu.com.cn/problem/P5788

// Cartesian Tree
struct cNode {
  int id, val, par, ch[2];
  void init(int _id, int _val, int _par) {
    id = _id, val = _val, par = _par, ch[0] = ch[1] = 0;
  }
};
int cartesian_build(std::vector<cNode>& tree, int n);
// https://codeforces.com/contest/1490/problem/D

// we can only use the ideal of merge sort
int64_t inverseOrderCount(std::vector<int> a);
// https://codeforces.com/contest/1602/problem/E

}  // namespace cuzperf
