#pragma once

#include <stdint.h>

namespace cuzperf {

// $O(\sqrt{n} \log^2 n)$ base on multi-evaluation
int factorialS(int n, int p);

// https://vjudge.net/problem/SPOJ-FACTMODP
int64_t factorial(int64_t n, int64_t p);

}  // namespace cuzperf
