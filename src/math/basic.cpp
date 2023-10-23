#include <cassert>

namespace cuzperf {

int powMod(int x, int n, int M)
{
  assert(n >= 0);
  int r = 1;
  for (; n; n >>= 1) {
    if (n & 1) {
      r = 1LL * r * x % M;
    }
    x = 1LL * x * x % M;
  }
  return r;
}

}  // namespace cuzperf
