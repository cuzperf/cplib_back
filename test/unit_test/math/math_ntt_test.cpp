#include "math/ntt.hpp"

#include "test/util.h"

namespace cuzperf {

TEST(MathTest, ntt) {
  {
    static constexpr int M = 998244353;  // 1 + 2^23 * 7 * 17
    using ntt = NTT<M>;

    std::vector<MInt<M>> a{1, 2, 3, 4, 5, 6, 7, 8};
    auto b = a;
    ntt t;
    t.dft(b);
    std::vector<MInt<M>> fa{36, 894301004, 346334868, 201631260, 998244349, 796613085, 651909477, 103943341};
    EXPECT_EQ(b, fa);
    t.idft(b);
    EXPECT_EQ(a, b);
  }
  {
    static constexpr int M = 469762049;  // 1 +  2^26 * 7
    using ntt = NTT<M>;

    std::vector<MInt<M>> a{1, 2, 3, 4, 5, 6, 7, 8};
    auto b = a;
    ntt t;
    t.dft(b);
    std::vector<MInt<M>> fa{36, 135891481, 78440360, 448772802, 469762045, 20989239, 391321681, 333870560};
    EXPECT_EQ(b, fa);
    t.idft(b);
    EXPECT_EQ(a, b);
  }
  {
    static constexpr int M = 754974721;  // 1 +  2^24 * 3^2 * 5
    using ntt = NTT<M>;
    // you must set this since it is a 3 is not a primitive root if 754974721
    ntt::setPrimitiveRoot(11);

    std::vector<MInt<M>> a{1, 2, 3, 4, 5, 6, 7, 8};
    auto b = a;
    ntt t;
    t.dft(b);
    std::vector<MInt<M>> fa{36, 721760612, 214508730, 292743144, 754974717, 462231569, 540465983, 33214101};
    EXPECT_EQ(b, fa);
    t.idft(b);
    EXPECT_EQ(a, b);
  }
}

}  // namespace cuzperf
