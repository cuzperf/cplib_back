#include "base/builtin.h"

#include "test/util.h"

namespace cuzperf {

TEST(BaseTest, mulMod) {
  {
    constexpr uint64_t a = 123456789012345678;
    constexpr uint64_t b = 876543210987654321;
    constexpr uint64_t m = 2718281828456;
    constexpr uint64_t abm = 1858573242646;
    EXPECT_EQ(mulModu(a, b, m), abm);
  }
  {
    constexpr int64_t a = -123456789012345678;
    constexpr int64_t b = 876543210987654321;
    constexpr int64_t m = 2718281828456;
    constexpr int64_t abm = -1858573242646;
    EXPECT_EQ(mulModi(a, b, m), abm);
  }
}

TEST(BaseTest, lg2) {
  std::vector<int> a{0, 0, 1, 1, 2, 2, 2, 2, 3, 3};
  for (int i = 0, n = a.size(); i < n; ++i) {
    EXPECT_EQ(IEE754_lg2_u32(i), a[i]);
    EXPECT_EQ(lg2_u32(i), a[i]);

    EXPECT_EQ(IEE754_lg2_u64(i), a[i]);
    EXPECT_EQ(lg2_u64(i), a[i]);
  }
  {
    unsigned M = 998244353;
    int lgM = 29;

    EXPECT_EQ(IEE754_lg2_u32(M), lgM);
    EXPECT_EQ(lg2_u32(M), lgM);

    EXPECT_EQ(IEE754_lg2_u64(M), lgM);
    EXPECT_EQ(lg2_u64(M), lgM);
  }
  {
    uint64_t M = 2718281828456;
    int lgM = 41;
    EXPECT_EQ(IEE754_lg2_u64(M), lgM);
    EXPECT_EQ(lg2_u64(M), lgM);
  }
  {
    unsigned M = (1 << 25) - 1;
    int lgM = 24;

    EXPECT_NE(M, unsigned(float(M)));

    EXPECT_EQ(IEE754_lg2_u32(M), lgM);
    EXPECT_EQ(lg2_u32(M), lgM);

    EXPECT_EQ(IEE754_lg2_u64(M), lgM);
    EXPECT_EQ(lg2_u64(M), lgM);
  }
  {
    uint64_t M = (1ll << 55) - 1;
    int lgM = 54;

    EXPECT_NE(M, uint64_t(double(M)));

    EXPECT_EQ((int)std::__lg(M), lgM);
    EXPECT_EQ(IEE754_lg2_u64(M), lgM);
    EXPECT_EQ(lg2_u64(M), lgM);
  }
}

TEST(BaseTest, clz) {
  std::vector<int> a{31, 31, 30, 30, 29, 29, 29, 29, 28, 28};
  for (int i = 0, n = a.size(); i < n; ++i) {
    EXPECT_EQ(clz_u32(i), a[i]);
    EXPECT_EQ(clz_u64(i), 32 + a[i]);
  }
  {
    unsigned M = 998244353;
    int lM = 2;

    EXPECT_EQ(clz_u32(M), lM);
    EXPECT_EQ(clz_u64(M), 32 + lM);
  }
  {
    uint64_t M = 2718281828456;
    int lM = 22;
    EXPECT_EQ(clz_u64(M), lM);
  }
}

TEST(BaseTest, ctz) {
  for (int i = 0; i < 32; ++i) {
    unsigned x = (rnd() >> i);
    EXPECT_GE(ctz_u32(std::max(x, 1u) << i), i);
  }

  for (int i = 0; i < 64; ++i) {
    uint64_t x = (rnd64() >> i);
    EXPECT_GE(ctz_u64(std::max(x, static_cast<uint64_t>(1)) << i), i);
  }
}

static void f32(int (*bc32)(unsigned)) {
  std::vector<int> bitCounts{0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
  for (int i = 0; i < 16; ++i) {
    EXPECT_EQ(bc32(i), bitCounts[i]);
  }
  EXPECT_EQ(bc32(0x12345678), 13);
  EXPECT_EQ(bc32(0xffffffff), 32);
  EXPECT_EQ(bc32(0xfffffffe), 31);
}

static void f64(int (*bc64)(uint64_t)) {
  std::vector<int> bitCounts{0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
  for (int i = 0; i < 16; ++i) {
    EXPECT_EQ(bc64(i), bitCounts[i]);
  }
  EXPECT_EQ(bc64(0x123456789abcdef0ull), 32);
  EXPECT_EQ(bc64(0xffffffffffffffffull), 64);
  EXPECT_EQ(bc64(0xfffffffffffffffeull), 63);
}

TEST(BaseTest, bitCount) {
  f32(bitCount_u32);
  f64(bitCount_u64);

  f32(bitCountTable_u32);
  f64(bitCountTable_u64);
}

static void g32(bool (*p32)(unsigned)) {
  std::vector<int> parities{0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0};
  for (int i = 0; i < 16; ++i) {
    EXPECT_EQ(p32(i), parities[i]);
  }
  EXPECT_TRUE(p32(0x12345678));
  EXPECT_FALSE(p32(0xffffffff));
  EXPECT_TRUE(p32(0xfffffffe));
}

static void g64(bool (*p64)(uint64_t)) {
  std::vector<int> parities{0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0};
  for (int i = 0; i < 16; ++i) {
    EXPECT_EQ(p64(i), parities[i]);
  }
  EXPECT_FALSE(p64(0x123456789abcdef0ull));
  EXPECT_FALSE(p64(0xffffffffffffffffull));
  EXPECT_TRUE(p64(0xfffffffffffffffeull));
}

TEST(BaseTest, parity) {
  g32(parity_u32);
  g64(parity_u64);

  g32(parityTable_u32);
  g64(parityTable_u64);

  g32(parityMIT_u32);
  g64(parityMIT_u64);
}

static constexpr float eps = 6.7e-5;

TEST(BsFastTest, acosFast) {
  std::uniform_real_distribution<float> u(-1, 1);
  std::default_random_engine e(time(nullptr));
  for (int i = 0; i < RUN_TIMES; ++i) {
    float x = u(e);
    EXPECT_LE(fabs(acosFast(x) - acos(x)), eps);
  }
}

TEST(BsFastTest, asinFast) {
  std::uniform_real_distribution<float> u(-1, 1);
  std::default_random_engine e(time(nullptr));
  for (int i = 0; i < RUN_TIMES; ++i) {
    float x = u(e);
    EXPECT_LE(fabs(asinFast(x) - asin(x)), eps);
  }
}

}  // namespace cuzperf
