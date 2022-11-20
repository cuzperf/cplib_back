#pragma once

#include <bits/stdc++.h>
#include "gtest/gtest.h"
using namespace std::literals::chrono_literals;

namespace cuzperf {

static std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());
static std::mt19937_64 rnd64(std::chrono::steady_clock::now().time_since_epoch().count());

static inline constexpr int RUN_TIMES = 123;
static inline constexpr auto WAIT_TIMES = 10ms;

}  // namespace cuzperf