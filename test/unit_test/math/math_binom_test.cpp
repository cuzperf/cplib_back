#include "math/binom.hpp"
#include "math/mod.hpp"
#include "math/numberTheory.h"
#include "test/util.h"

namespace cuzperf {

TEST(MathTest, binom) {
  auto& binom = Binom::Instance();

  EXPECT_EQ(binom(3, 0), 1);
  EXPECT_EQ(binom(3, 1), 3);
  EXPECT_EQ(binom(3, 2), 3);
  EXPECT_EQ(binom(3, 3), 1);

  EXPECT_EQ(binom(10, 2), 45);
  EXPECT_EQ(binom(10, 5), 252);
  EXPECT_EQ(binom(10, 8), 45);

  EXPECT_EQ(binom(64, 32), 1832624140942590534);
}

constexpr static int M = 998244353;
namespace {
using mod = MInt<M>;
}  // namespace

TEST(MathTest, binomModp) {
  constexpr int N = 1003;
  auto& binom = BinomModp<mod>::Instance(N);

  EXPECT_EQ(binom(3, 0), mod(1));
  EXPECT_EQ(binom(3, 1), mod(3));
  EXPECT_EQ(binom(3, 2), mod(3));
  EXPECT_EQ(binom(3, 3), mod(1));

  EXPECT_EQ(binom(10, 2), mod(45));
  EXPECT_EQ(binom(10, 5), mod(252));
  EXPECT_EQ(binom(10, 8), mod(45));

  EXPECT_EQ(binom(64, 32), mod(641954814));
  EXPECT_EQ(binom(1000, 500), mod(640488516));
}

TEST(MathTest, lagrange) {
  // x^2
  {
    std::vector<mod> f{0, 1, 4};
    EXPECT_EQ(Lagrange(f, 3), mod(9));
    EXPECT_EQ(Lagrange(f, 4), mod(16));
    EXPECT_EQ(Lagrange(f, 100), mod(10000));
    EXPECT_EQ(Lagrange(f, 100000), mod(17556470));
  }
  // x^3 + x^2 - x + 2
  {
    std::vector<mod> f{2, 3, 12, 35};
    EXPECT_EQ(Lagrange(f, 4), mod(78));
    EXPECT_EQ(Lagrange(f, 5), mod(147));
    EXPECT_EQ(Lagrange(f, 6), mod(248));
    EXPECT_EQ(Lagrange(f, 100), mod(1009902));
    EXPECT_EQ(Lagrange(f, 100000), mod(750883898));
  }
}

TEST(MathTest, powSum) {
  constexpr int N = 100005;
  auto sp = spf(N);

  EXPECT_EQ(powSum<mod>(4, 0, sp), mod(4));
  EXPECT_EQ(powSum<mod>(4, 1, sp), mod(10));
  EXPECT_EQ(powSum<mod>(4, 2, sp), mod(30));
  EXPECT_EQ(powSum<mod>(4, 3, sp), mod(100));
  EXPECT_EQ(powSum<mod>(4, 4, sp), mod(354));

  EXPECT_EQ(powSum<mod>(521, 0, sp), mod(521));
  EXPECT_EQ(powSum<mod>(521, 1, sp), mod(135981));
  EXPECT_EQ(powSum<mod>(521, 2, sp), mod(47276061));
  EXPECT_EQ(powSum<mod>(521, 3, sp), mod(522434007));
  EXPECT_EQ(powSum<mod>(521, 4, sp), mod(931690166));
  EXPECT_EQ(powSum<mod>(521, 5, sp), mod(878873376));
  EXPECT_EQ(powSum<mod>(521, 520, sp), mod(660915335));

  EXPECT_EQ(powSum<mod>(N - 1, 100, sp), mod(338524149));
}

}  // namespace cuzperf
