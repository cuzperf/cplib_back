#ifndef __BUILTIN__
#define __BUILTIN__

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// return (a * b) % m
int64_t mulModi(int64_t a, int64_t b, int64_t m);
uint64_t mulModu(uint64_t a, uint64_t b, uint64_t m);

// base on IEE754 for float: 1 + 8 + 23
int IEE754_lg2_u32(unsigned x);
// base on IEE754 for double: 1 + 11 + 52
int IEE754_lg2_u64(uint64_t x);

// return log2(x)
int lg2_u32(unsigned x);
int lg2_u64(uint64_t x);
// http://graphics.stanford.edu/~seander/bithacks.html

int clz_u32(unsigned x);
int clz_u64(uint64_t x);

// slow than __builtin_ctz and __builtin_ctzll but funny, you may use -Wno-narrowing when complier
int ctz_u32(unsigned x);
int ctz_u64(uint64_t x);
// https://xr1s.me/2018/08/23/gcc-builtin-implementation/

// MIT HAKMEM: about two times faster than __builtin_popcount()
int bitCount_u32(unsigned n);
// MIT HAKMEM: about two times faster than __builtin_popcountll(), run with 64bit
int bitCount_u64(uint64_t n);
// https://www.cnblogs.com/lukelouhao/archive/2012/06/12/2546267.html

// faster than bitCount_u32
int bitCountTable_u32(unsigned n);
// slow than bitCount_u64
int bitCountTable_u64(uint64_t n);
// https://www.cnblogs.com/graphics/archive/2010/06/21/1752421.html

// All below are sightly slow than __builtin_parity and __builtin_parityll
bool parity_u32(unsigned n);
// slow than parityMIT_u32
bool parity_u64(uint64_t n);
bool parityTable_u32(unsigned n);  // slow than __builtin_parity
bool parityTable_u64(uint64_t n);  // slow than __builtin_parityll
bool parityMIT_u32(unsigned n);    // slow than parity_u32
bool parityMIT_u64(uint64_t n);

// Handbook of Mathematical Functions by M. Abramowitz and I.A. Stegun, Ed.
// Absolute error <= 6.7e-5
float acosFast(float x);
float asinFast(float x);
// https://developer.download.nvidia.cn/cg

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  //  __BUILTIN__
