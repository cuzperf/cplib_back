// g++ blocks_trans_test.cpp -std=c++17 -O2 -I../.. && ./a.out

// L1 cache test, result deponeds on Machine

#include <bits/stdc++.h>

#include "src/timer.hpp"

static std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());

const int N = 4321;
const int M = 12340;
int a[N][M];
int b[M][N];

void tran90block(int li, int ri, int lj, int rj) {
  for (int i = li; i < ri; ++i) {
    for (int j = lj; j < rj; ++j) {
      b[j][i] = a[N - 1 - i][j];
    }
  }
}

void tran90_2d(int blocki, int blockj) {
  int mb = M / blockj, nb = N / blocki;
  int lj = 0;
  for (int j = 0; j < mb; ++j) {
    int rj = lj + blockj;
    int li = 0;
    for (int i = 0; i < nb; ++i) {
      int ri = li + blocki;
      tran90block(li, ri, lj, rj);
      li = ri;
    }
    tran90block(li, N, lj, rj);
    lj = rj;
  }
  int li = 0;
  for (int i = 0; i < nb; ++i) {
    int ri = li + blocki;
    tran90block(li, ri, lj, M);
    li = ri;
  }
  tran90block(li, N, lj, M);
}

void tran90(int blocks) {
  int mb = M / blocks, bj = 0;
  for (int _ = 0; _ < mb; ++_) {
    int nbj = bj + blocks;
    tran90block(0, N, bj, nbj);
    bj = nbj;
  }
  tran90block(0, N, bj, M);
}

int main() {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < M; ++j) {
      a[i][j] = rnd() % 100;
    }
  }
  {
    auto start = cuzperf::Timer::now_ms();
    tran90(M);
    std::cout << "[time] origin: " << cuzperf::Timer::now_ms() - start << "ms\n";
    // for (int i = 0; i < 10; ++i) {
    //   for (int j = 0; j < 10; ++j) {
    //     std::cout << b[i][j] << ' ';
    //   }
    //   std::cout << '\n';
    // }
  }
  constexpr int MaxB = 20;
  {
    std::vector<uint64_t> times(MaxB + 1, INT64_MAX);
    for (int block = 2; block <= MaxB; ++block) {
      auto start = cuzperf::Timer::now_ms();
      tran90(block);
      times[block] = cuzperf::Timer::now_ms() - start;
      // for (int i = 0; i < 10; ++i) {
      //   for (int j = 0; j < 10; ++j) {
      //     std::cout << b[i][j] << ' ';
      //   }
      //   std::cout << '\n';
      // }
    }
    auto mn = std::min_element(times.begin(), times.end());
    std::cout << "[time] 1-dim " << mn - times.begin() << " blocks: " << *mn << "ms\n";
  }
  {
    uint64_t mn = INT64_MAX;
    int i, j;
    for (int blocki = 2; blocki <= MaxB; ++blocki) {
      for (int blockj = 2; blockj <= MaxB; ++blockj) {
        auto start = cuzperf::Timer::now_ms();
        tran90_2d(blocki, blockj);
        auto time = cuzperf::Timer::now_ms() - start;
        if (time < mn) {
          i = blocki;
          j = blockj;
          mn = time;
        }
        // for (int i = 0; i < 10; ++i) {
        //   for (int j = 0; j < 10; ++j) {
        //     std::cout << b[i][j] << ' ';
        //   }
        //   std::cout << '\n';
        // }
      }
    }
    std::cout << "[time] 2-dim " << i << ' ' << j << " blocks: " << mn << "ms\n";
  }
  return 0;
}
