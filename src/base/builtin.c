#include "base/builtin.h"

#include <limits.h>
#include <math.h>

#if defined(_MSC_VER) && (defined(_M_X86) || defined(_M_X64))
#include <ammintrin.h>
#include <immintrin.h>
#endif

int64_t mulModi(int64_t a, int64_t b, int64_t m) {
#if defined(__GNUC__)
  return (__int128)a * b % m;
#elif defined(_MSC_VER) && defined(_M_X64)
  int64_t high;
  const int64_t low = _mul128(a, b, &high);
  int64_t rem;
  _div128(high, low, m, &rem);
  return rem;
#else
  assert(m <= INT64_MAX / 2 && m >= INT64_MIN / 2);
  if (b < 0) {
    b = -b;
    a = -a;
  }
  int64_t r = 0;
  for (; b; b >>= 1) {
    if (b & 1) {
      r = (r + a) % m;
    }
    a = (a + a) % m;
  }
  return r;
#endif
}

uint64_t mulModu(uint64_t a, uint64_t b, uint64_t m) {
#if defined(__GNUC__)
  return (unsigned __int128)a * b % m;
#elif defined(_MSC_VER) && defined(_M_X64)
  uint64_t high;
  const uint64_t low = _umul128(a, b, &high);
  uint64_t rem;
  _udiv128(high, low, m, &rem);
  return rem;
#else
  assert(m <= UINT64_MAX / 2);
  if (a < b) {
    std::swap(a, b);
  }
  uint64_t r = 0;
  for (; b; b >>= 1) {
    if (b & 1) {
      r += a;
      if (r >= m) {
        r -= m;
      }
    }
    a *= 2;
    if (a >= m) {
      a -= m;
    }
  }
  return r;
#endif
}

int IEE754_lg2_u32(unsigned x) {
  if (x == 0) {
    return 0;
  }
  union {
    float f;
    unsigned i;
  } v = {.f = x};
  return (v.i >> 23) - 127U;
}

int IEE754_lg2_u64(uint64_t x) {
  if (x == 0) {
    return 0;
  }
  union {
    double f;
    uint64_t i;
  } v = {.f = x};
  return (v.i >> 52) - 1023ULL;
}

int lg2_u32(unsigned x) {
#if defined(__GNUC__)
  return sizeof(uint64_t) * __CHAR_BIT__ - 1 - clz_u64(x);
#elif defined(_MSC_VER) && (defined(_M_X86) || defined(_M_X64))
  return sizeof(unsigned) * __CHAR_BIT__ - 1 - clz_u32(x);
#else
  return IEE754_lg2_u32(x);
#endif
}

int lg2_u64(uint64_t x) {
#if defined(__GNUC__)
  return sizeof(uint64_t) * __CHAR_BIT__ - 1 - clz_u64(x);
#elif defined(_MSC_VER) && (defined(_M_X86) || defined(_M_X64))
  return sizeof(uint64_t) * __CHAR_BIT__ - 1 - clz_u64(x);
#else
  return IEE754_lg2_u64(x);
#endif
}

int clz_u32(unsigned x) {
#if defined(__GNUC__)
  return __builtin_clz(x);
#elif defined(_MSC_VER) && (defined(_M_X86) || defined(_M_X64))
  return _lzcnt_u32(x)
#else
  return sizeof(unsigned) * __CHAR_BIT__ - 1 - lg2_u32(x);
#endif
}

int clz_u64(uint64_t x) {
#if defined(__GNUC__)
  return __builtin_clzll(x);
#elif defined(_MSC_VER) && (defined(_M_X86) || defined(_M_X64))
  return _lzcnt_u64(x);
#else
  return return sizeof(uint64_t) * __CHAR_BIT__ - 1 - lg2_u64(x);
#endif
}

int ctz_u32(unsigned x) {
#ifdef __GNUC__
  return __builtin_ctz(x);
#elif defined(_MSC_VER) && (defined(_M_X86) || defined(_M_X64))
  return _tzcnt_u32(x);
#else
  return IEE754_lg2_u32(x & -x);
#endif
}
int ctz_u64(uint64_t x) {
#ifdef __GNUC__
  return __builtin_ctzll(x);
#elif defined(_MSC_VER) && (defined(_M_X86) || defined(_M_X64))
  return _tzcnt_u64(x);
#else
  return IEE754_lg2_u64(x & -x);
#endif
}

int bitCount_u32(unsigned n) {
  unsigned tmp = n - ((n >> 1) & 033333333333U) - ((n >> 2) & 011111111111U);
  return ((tmp + (tmp >> 3)) & 030707070707U) % 63U;
}

int bitCount_u64(uint64_t n) {
  uint64_t tmp = n - ((n >> 1) & 0x7777777777777777ULL) - ((n >> 2) & 0x3333333333333333ULL) -
                 ((n >> 3) & 0x1111111111111111ULL);
  return ((tmp + (tmp >> 4)) & 0x0f0f0f0f0f0f0f0fULL) % 255ULL;
}

int bitCountTable_u32(unsigned n) {
  static const int table[256] = {
      0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3,
      4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4,
      4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4,
      5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5,
      4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2,
      3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5,
      5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4,
      5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6,
      4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
  };
  return table[n & 0xffUL] + table[(n >> 8) & 0xffUL] + table[(n >> 16) & 0xffUL] + table[n >> 24];
}

int bitCountTable_u64(uint64_t n) {
  return bitCountTable_u32(n >> 32) + bitCountTable_u32(n & 0xffffffffULL);
}

bool parity_u32(unsigned n) {
  n = n ^ n >> 16;
  n = n ^ n >> 8;
  n = n ^ n >> 4;
  n = n ^ n >> 2;
  return (n ^ n >> 1) & 1U;
}
bool parity_u64(uint64_t n) {
  n = n ^ n >> 32;
  n = n ^ n >> 16;
  n = n ^ n >> 8;
  n = n ^ n >> 4;
  n = n ^ n >> 2;
  return (n ^ n >> 1) & 1U;
}
bool parityTable_u32(unsigned n) {
  static const bool table[256] = {
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1,
      0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0,
      0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0,
      1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1,
      0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0,
      1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1,
      1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0,
      1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
      0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
  };
  n = n ^ n >> 16;
  return table[(n ^ n >> 8) & 0xffU];
}
bool parityTable_u64(uint64_t n) {
  static const bool table[256] = {
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1,
      0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0,
      0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0,
      1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1,
      0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0,
      1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1,
      1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0,
      1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
      0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
  };
  n = n ^ n >> 32;
  n = n ^ n >> 16;
  return table[(n ^ n >> 8) & 0xffULL];
}
bool parityMIT_u32(unsigned n) {
  n = (n ^ n >> 1) & 0x55555555U;
  return (((n ^ n >> 2) & 0x11111111U) % 15U) & 1U;
}
bool parityMIT_u64(uint64_t n) {
  n = (n ^ n >> 1 ^ n >> 2) & 01111111111111111111111ULL;
  return (((n ^ n >> 3) & 0101010101010101010101ULL) % 63ULL) & 1U;
}

float acosFast(float x) {
  bool flag = (x < 0);
  x = fabs(x);
  float now = sqrt(1.0 - x) * (((0.0742610f - 0.0187293f * x) * x - 0.2121144f) * x + 1.5707288f);
  return flag ? 3.14159265358979f - now : now;
}
float asinFast(float x) {
  bool flag = (x < 0);
  x = fabs(x);
  float now = sqrt(1.0 - x) * (((0.0742610f - 0.0187293f * x) * x - 0.2121144f) * x + 1.5707288f);
  return flag ? now - 1.5707963267949f : 1.5707963267949f - now;
}
