#include <thread>

#include "test/util.h"
#include "thread/threadSafeQueue.hpp"

namespace cuzperf {

TEST(ThreadTest, threadSafeQueue) {
  ThreadSafeQueue<int> A;
  std::vector<std::thread> threads;
  threads.reserve(8);
  threads.emplace_back(std::thread([&] {
    std::this_thread::sleep_for(0ms);
    A.push(1);
  }));
  threads.emplace_back(std::thread([&] {
    std::this_thread::sleep_for(20ms);
    A.push(2);
  }));
  threads.emplace_back(std::thread([&] {
    std::this_thread::sleep_for(60ms);
    A.push(3);
  }));
  threads.emplace_back(std::thread([&] {
    // Note that the timer is not accurate
    std::this_thread::sleep_for(200ms);
    A.push(4);
  }));

  threads.emplace_back(std::thread([&] {
    std::this_thread::sleep_for(30ms);
    int x = -1;
    bool success = A.pop(x);
    EXPECT_TRUE(success);
    EXPECT_EQ(x, 1);
  }));
  threads.emplace_back(std::thread([&] {
    std::this_thread::sleep_for(50ms);
    int x = -1;
    bool success = A.pop(x);
    EXPECT_TRUE(success);
    EXPECT_EQ(x, 2);
  }));
  threads.emplace_back(std::thread([&] {
    std::this_thread::sleep_for(70ms);
    int x = -1;
    bool success = A.pop(x, 20);
    EXPECT_TRUE(success);
    EXPECT_EQ(x, 3);
  }));
  threads.emplace_back(std::thread([&] {
    std::this_thread::sleep_for(100ms);
    int x = -1;
    bool success = A.pop(x, 30);
    EXPECT_FALSE(success);
  }));

  for (auto& t : threads) {
    t.join();
  }
}

}  // namespace cuzperf
