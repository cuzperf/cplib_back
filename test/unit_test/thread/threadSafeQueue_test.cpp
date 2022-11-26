#include "thread/threadSafeQueue.hpp"

#include <thread>

#include "test/util.h"

namespace cuzperf {

TEST(ThreadTest, threadSafeQueue) {
  ThreadSafeQueue<int> A(30);
  std::vector<std::thread> threads;
  threads.reserve(8);
  threads.emplace_back(std::thread([&] {
    std::this_thread::sleep_for(0ms);
    A.push(1);
  }));
  threads.emplace_back(std::thread([&] {
    std::this_thread::sleep_for(10ms);
    A.push(2);
  }));
  threads.emplace_back(std::thread([&] {
    std::this_thread::sleep_for(30ms);
    A.push(3);
  }));
  threads.emplace_back(std::thread([&] {
    std::this_thread::sleep_for(80ms);
    A.push(4);
  }));

  threads.emplace_back(std::thread([&] {
    std::this_thread::sleep_for(15ms);
    auto x = A.pop();
    EXPECT_EQ(x, 1);
  }));
  threads.emplace_back(std::thread([&] {
    std::this_thread::sleep_for(20ms);
    auto x = A.pop();
    EXPECT_EQ(x, 2);
  }));
  threads.emplace_back(std::thread([&] {
    std::this_thread::sleep_for(35ms);
    int x = -1;
    bool success = A.pop(x);
    EXPECT_TRUE(success);
    EXPECT_EQ(x, 3);
  }));
  threads.emplace_back(std::thread([&] {
    std::this_thread::sleep_for(40ms);
    int x = -1;
    bool success = A.pop(x);
    EXPECT_FALSE(success);
  }));

  for (auto& t : threads) {
    t.join();
  }
}

}  // namespace cuzperf
