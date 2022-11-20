#include "test/util.h"

#include "base/builtin.h"

namespace cuzperf {

TEST(BsTest, ctz) {
  for (int i = 0; i < 32; ++i) {
    unsigned x = (rnd() >> i);
    EXPECT_GE(ctz32(std::max(x, 1u) << i), i);
  }

  for (int i = 0; i < 64; ++i) {
    uint64_t x = (rnd64() >> i);
    EXPECT_GE(ctz64(std::max(x, static_cast<uint64_t>(1)) << i), i);
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

TEST(BsTest, bitCount) {
  f32(bitCount32);
  f64(bitCount64);

  f32(bitCountTable32);
  f64(bitCountTable64);
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

TEST(BsTest, parity) {
  g32(parity32);
  g64(parity64);

  g32(parityTable32);
  g64(parityTable64);

  g32(parityMIT32);
  g64(parityMIT64);
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
