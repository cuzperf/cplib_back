#pragma once

#include <stdint.h>

#include <tuple>
#include <utility>
#include <vector>

namespace cuzperf {

namespace Floyd {

void Floyd(int n);
std::vector<int> getPath(int x, int y);

}  // namespace Floyd

using edge = std::vector<std::pair<int, int>>;
using Edge = std::tuple<int, int, int>;

std::vector<int64_t> Dijkstra(const std::vector<edge>& e, int s);

bool BellmanFord(std::vector<Edge>& e, int n, int s = 0);

bool spfa(std::vector<edge>& e, int s = 0);

}  // namespace cuzperf
