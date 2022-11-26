#pragma once

#include <vector>

namespace cuzperf {

template <typename T>
struct ListNode {
  T val;
  ListNode* next;
  ListNode(ListNode* nxt = nullptr) : next(nxt) {}
  ListNode(const T& v, ListNode* nxt = nullptr) : val(v), next(nxt) {}
  ListNode(T&& v, ListNode* nxt = nullptr) : val(std::move(v)), next(nxt) {}
};

template <typename T>
std::vector<T> List2Vector(ListNode<T>* root) {
  std::vector<T> a;
  while (root) {
    a.emplace_back(root->val);
    root = root->next;
  }
  return a;
}

template <typename T>
ListNode<T>* Vector2List(const std::vector<T>& a) {
  ListNode<T>* root = nullptr;
  auto tail = root;
  for (const auto& x : a) {
    if (tail) {
      tail->next = new ListNode(x);
      tail = tail->next;
    } else {
      tail = root = new ListNode(x);
    }
  }
  return root;
}

template <typename T>
void clearList(ListNode<T>* root) {
  while (root) {
    auto nxt = root->next;
    delete root;
    root = nxt;
  }
}

}  // namespace cuzperf
