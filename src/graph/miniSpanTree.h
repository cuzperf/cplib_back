#pragma once

#include <stdint.h>
#include <vector>

namespace cuzperf {


using edge = std::vector<std::pair<int, int>>;
using Edge = std::tuple<int, int, int>;

// Minimum Spanning Tree
int64_t Prim(const std::vector<edge>& e);
// LiuZhu: Minimum tree diagram
int64_t LiuZhu(std::vector<Edge> e, int n, int rt); // e has no self-loop

}  // namespace cuzperf
