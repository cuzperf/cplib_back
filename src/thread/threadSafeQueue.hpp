#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace cuzperf {

template <typename T>
class ThreadSafeQueue {
  std::queue<T> Q_;
  mutable std::mutex mtx_;
  std::condition_variable cv_;

 public:
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
  bool pop(T& ans, int wait_ms = -1) {
    std::unique_lock<std::mutex> _(mtx_);
    bool wait_success = true;
    if (wait_ms < 0) {
      cv_.wait(_, [this]() { return !Q_.empty(); });
    } else {
      wait_success =
          cv_.wait_for(_, std::chrono::milliseconds(wait_ms), [this]() { return !Q_.empty(); });
    }
    if (wait_success) {
      ans = std::move(Q_.front());
      Q_.pop();
    }
    return wait_success;
  }
};
}  // namespace cuzperf
