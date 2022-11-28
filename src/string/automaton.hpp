#pragma once

#include <array>
#include <queue>
#include <vector>
#include <string>

namespace cuzperf {

class Automaton {
  static inline constexpr int CHAR = 26;
  static inline int charToInt(char x) { return x - 'a'; }
  using Node = std::array<int, CHAR>;
  std::vector<Node> nxt_;
  std::vector<int> cnt_, fail_, last_;
  void addNode(int fa, int c) {
    nxt_[fa][c] = nxt_.size();
    nxt_.emplace_back(Node());
    cnt_.emplace_back(0);
    fail_.emplace_back(0);
    last_.emplace_back(0);
  }

 public:
  Automaton() : nxt_(1), cnt_(1), fail_(1), last_(1) {}
  void insert(std::string s) {
    int p = 0;
    for (auto x : s) {
      int c = charToInt(x);
      if (nxt_[p][c] == 0) {
        addNode(p, c);
      }
      p = nxt_[p][c];
    }
    ++cnt_[p];
  }
  void build() {
    std::queue<int> Q;
    for (int c = 0; c < CHAR; ++c) {
      if (nxt_[0][c]) {
        Q.push(nxt_[0][c]);
      }
    }
    while (!Q.empty()) {
      int p = Q.front();
      Q.pop();
      for (int c = 0; c < CHAR; ++c) {
        if (int& q = nxt_[p][c]; q != 0) {
          fail_[q] = nxt_[fail_[p]][c];
          Q.push(q);
          // match count optim
          last_[q] = cnt_[fail_[q]] ? fail_[q] : last_[fail_[q]];
        } else {
          q = nxt_[fail_[p]][c];
        }
      }
    }
  }
  // case-by-case analysis
  int query(std::string s) {
    int p = 0, r = 0;
    auto add = [&](int& x) {
      r += x;
      x = 0;
    };
    for (auto x : s) {
      int c = charToInt(x);
      p = nxt_[p][c];
      if (cnt_[p]) {
        add(cnt_[p]);
      }
      int q = p;
      while (last_[q]) {
        q = last_[q];
        if (cnt_[q]) {
          add(cnt_[q]);
        }
      }
    }
    return r;
  }
};

}  // namespace cuzperf
