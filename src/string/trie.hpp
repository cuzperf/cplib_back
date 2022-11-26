#pragma once

#include <array>
#include <vector>

namespace cuzperf {

// Trie build by lowercase characters(change charToInt for other charset)
class Trie {
  using Node = std::array<int, 26>;
  std::vector<Node> nxt_;
  // 0: no point, 1: has point unvisited, 2: has point visited
  std::vector<int> tag_;
  void addNode(int fa, int c) {
    nxt_[fa][c] = nxt_.size();
    nxt_.emplace_back(Node());
    tag_.emplace_back(0);
  }
  int charToInt(char x) { return x - 'a'; }

 public:
  Trie() : nxt_(1), tag_(1) {}
  void insert(std::string s) {
    int p = 0;
    for (auto x : s) {
      int c = x - 'a';
      if (nxt_[p][c] == 0) {
        addNode(p, c);
      }
      p = nxt_[p][c];
    }
    tag_[p] = 1;
  }
  int find(std::string s) {
    int p = 0;
    for (auto x : s) {
      int c = charToInt(x);
      if (nxt_[p][c] == 0) {
        return 0;
      }
      p = nxt_[p][c];
    }
    if (tag_[p] != 1) {
      return tag_[p];
    }
    tag_[p] = 2;
    return 1;
  }
};
// https://www.luogu.com.cn/problem/P2580

// 01 Trie find maximal xor sum
class Trie01 {
  using Node = std::array<int, 2>;
  std::vector<Node> ch_;
  void addNode(int fa, int c) {
    ch_[fa][c] = ch_.size();
    ch_.emplace_back(Node());
  }

 public:
  Trie01() : ch_(1) {}
  void insert(int x) {
    for (int i = 30, p = 0; i >= 0; --i) {
      int c = (x >> i) & 1;
      if (ch_[p][c] == 0) {
        addNode(p, c);
      }
      p = ch_[p][c];
    }
  }
  int getMax(int x) {
    int r = 0;
    for (int i = 30, p = 0; i >= 0; --i) {
      int c = (x >> i) & 1;
      if (ch_[p][c ^ 1]) {
        p = ch_[p][c ^ 1];
        r |= (1 << i);
      } else {
        p = ch_[p][c];
      }
    }
    return r;
  }
  int getAns(const std::vector<int>& a) {
    int r = 0;
    for (auto x : a) {
      insert(x);
      r = std::max(r, getMax(x));
    }
    return r;
  }
};
// https://www.luogu.com.cn/problem/P4551

// 01-Trie (Fusion Tree) xor sum(support modifty, add 1 to all)
class FusionTree {
  using Node = std::array<int, 4>;
  std::vector<Node> ch_;
#define lsonFT ch_[p][0]
#define rsonFT ch_[p][1]
  // ch_[p][2] = size of subtree rooted by p
  // ch_[p][3] = xor sum of subtree rooted by p
  void addNode(int p, int c) {
    ch_[p][c] = ch_.size();
    ch_.emplace_back(Node());
  }
  void pushUp(int p) {
    ch_[p][3] = 0;
    if (lsonFT) {
      ch_[p][3] ^= (ch_[lsonFT][3] << 1);
    }
    if (rsonFT) {
      ch_[p][3] ^= (ch_[rsonFT][3] << 1) | (ch_[rsonFT][2] & 1);
    }
  }
  // note ch_[lson][2] = ch_[p][2] - ch_[rsonFT] is a lazy tag_
  void insert(int p, int x) {
    ++ch_[p][2];
    if (!x) {
      return;
    }
    if (!ch_[p][x & 1]) {
      addNode(p, x & 1);
    }
    insert(ch_[p][x & 1], x >> 1);
    pushUp(p);
  }
  void erase(int p, int x) {
    --ch_[p][2];
    if (!x) {
      return;
    }
    erase(ch_[p][x & 1], x >> 1);
    pushUp(p);
  }
  void addAll(int p) {
    if (!ch_[p][2]) {
      return;
    }
    if (rsonFT) {
      addAll(rsonFT);
    }
    if (!lsonFT) {
      addNode(p, 0);
    }
    ch_[lsonFT][2] = ch_[p][2] - (rsonFT ? ch_[rsonFT][2] : 0);
    std::swap(lsonFT, rsonFT);
    pushUp(p);
  }

 public:
  FusionTree() : ch_(1) {}
  void insert(int x) { insert(0, x); }
  void erase(int x) { erase(0, x); }
  void addAll() { addAll(0); }
  int getVal() { return ch_[0][3]; }
};
// https://www.luogu.com.cn/problem/P6018

}  // namespace cuzperf
