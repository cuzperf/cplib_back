#pragma once

#include <complex>
#include <vector>

#include "base/builtin.h"

namespace cuzperf {
namespace FFT {

using C = std::complex<double>;
void dft(std::vector<C>& a);
void idft(std::vector<C>& a);

}  // namespace FFT
}  // namespace cuzperf
