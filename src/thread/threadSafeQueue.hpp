#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <chrono>

namespace cuzperf {

template<typename T>
class ThreadSafeQueue {
  std::queue<T> Q_;
  mutable std::mutex mtx_;
  std::condition_variable cv_;
  const int wait_ms_;
 public:
  ThreadSafeQueue(int wait_ms = -1) : wait_ms_(wait_ms) {}
  void push(const T& x) {
    {
      std::lock_guard _(mtx_);
      Q_.push(x);
    }
    cv_.notify_one();
  }
  void push(T&& x) {
    {
      std::lock_guard _(mtx_);
      Q_.push(std::move(x));
    }
    cv_.notify_one();
  }
  T pop() {
    std::unique_lock<std::mutex> _(mtx_);
    cv_.wait(_, [this]() { return !Q_.empty();});
    auto ans = std::move(Q_.front());
    Q_.pop();
    return ans;
  }
  bool pop(T& ans) {
    std::unique_lock<std::mutex> _(mtx_);
    bool wait_success = true;
    if (wait_ms_ < 0) {
      cv_.wait(_, [this]() { return !Q_.empty();});
    } else {
      wait_success = cv_.wait_for(_, std::chrono::milliseconds(wait_ms_), [this]() { return !Q_.empty();});
    }
    if (wait_success) {
      ans = std::move(Q_.front());
      Q_.pop();
    }
    return wait_success;
  }
};
}  // namespace cuzperf
