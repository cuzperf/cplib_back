#ifndef __BUILTIN__
#define __BUILTIN__

#include <stdbool.h>

// slow than __builtin_ctz and __builtin_ctzll but funny, you may use -Wno-narrowing when complier
int ctz32(unsigned x);
int ctz64(unsigned long long x);
// https://xr1s.me/2018/08/23/gcc-builtin-implementation/

inline int lg32(unsigned x) {
  return sizeof(unsigned) * __CHAR_BIT__  - 1 - ctz32(x);
}
inline int lg64(unsigned long long x) {
  return sizeof(unsigned long long) * __CHAR_BIT__  - 1 - ctz64(x);
}

// MIT HAKMEM: about two times faster than __builtin_popcount()
int bitCount(unsigned n);
// MIT HAKMEM: about two times faster than __builtin_popcountll(), run with 64bit
int bitCountll(unsigned long long n);
// https://www.cnblogs.com/lukelouhao/archive/2012/06/12/2546267.html

// faster than bitCount
int bitCountTable(unsigned n);
// slow than bitCountll
int bitCountTableLL(unsigned long long n);
// https://www.cnblogs.com/graphics/archive/2010/06/21/1752421.html

// All below are sightly slow than __builtin_parity and __builtin_parityll
bool parity(unsigned n);
// slow than parityMIT
bool parityll(unsigned long long n);
bool parityTable(unsigned n);  // slow than __builtin_parity
bool parityTablell(unsigned long long n);  // slow than __builtin_parityll
bool parityMIT(unsigned n);  // slow than parity
bool parityMITll(unsigned long long n);

// Handbook of Mathematical Functions by M. Abramowitz and I.A. Stegun, Ed.
// Absolute error <= 6.7e-5
float acosFast(float x);
float asinFast(float x);
// https://developer.download.nvidia.cn/cg

#endif //  __BUILTIN__
