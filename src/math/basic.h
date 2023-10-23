#pragma once

namespace cuzperf {

// @return $x^n \mod M$ in $O(\log n)$
// @param $n \geq 0$ and $M \geq 0$
int powMod(int x, int n, int M);

}  // namespace cuzperf
