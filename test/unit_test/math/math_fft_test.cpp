#include "math/fft.h"
#include "test/util.h"

namespace cuzperf {

static constexpr double eps = 1e-6;

TEST(MathTest, fft) {
  {
    std::vector<FFT::C> a{1, 2, 3, 4};
    int n = a.size();
    auto b = a;
    FFT::dft(b);
    std::vector<FFT::C> ffta{10, FFT::C(-2, 2), -2, FFT::C(-2, -2)};
    for (int i = 0; i < n; ++i) {
      EXPECT_LE(abs(ffta[i] - b[i]), eps);
    }
    FFT::idft(b);
    for (int i = 0; i < n; ++i) {
      EXPECT_LE(abs(a[i] - b[i]), eps);
    }
  }
  {
    std::vector<FFT::C> a{
        1, 2, 3, 4, FFT::C(1, 0.5), FFT::C(2, 0.3), FFT::C(0, 0.5), FFT::C(2.1, 3.4)};
    auto b = a;
    FFT::dft(b);
    FFT::idft(b);
    int n = a.size();
    for (int i = 0; i < n; ++i) {
      EXPECT_LE(abs(a[i] - b[i]), eps);
    }
  }
}

}  // namespace cuzperf
