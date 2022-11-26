#pragma once

#include <iostream>

namespace cuzperf {

template <typename valT>
class Matrix {
  static inline constexpr int N = 1003;
  int n_;

 public:
  valT a_[N][N];
  Matrix() {}
  Matrix(int n, valT x = 0) : n_(n) {
    all(0);
    for (int i = 0; i < n_; ++i) {
      a_[i][i] = x;
    }
  }
  void all(valT x) {
    for (int i = 0; i < n_; ++i) {
      for (int j = 0; j < n_; ++j) {
        a_[i][j] = x;
      }
    }
  }
  Matrix& operator+=(const Matrix& rhs) {
    for (int i = 0; i < n_; ++i) {
      for (int j = 0; j < n_; ++j) {
        a_[i][j] += rhs.a_[i][j];
      }
    }
    return (*this);
  }
  Matrix operator+(const Matrix& rhs) const { return Matrix(this) += rhs; }
  Matrix& operator-=(const Matrix& rhs) {
    for (int i = 0; i < n_; ++i) {
      for (int j = 0; j < n_; ++j) {
        a_[i][j] -= rhs.a_[i][j];
      }
    }
    return (*this);
  }
  Matrix operator-(const Matrix& rhs) const { return Matrix(this) -= rhs; }
  Matrix operator*(const Matrix& rhs) const {
    Matrix R(n_);
    for (int i = 0; i < n_; ++i) {
      for (int k = 0; k < n_; ++k) {
        for (int j = 0; j < n_; ++j) {
          R.a_[i][j] += a_[i][k] * rhs.a_[k][j];
        }
      }
    }
    return R;
  }
  Matrix operator*=(const Matrix& rhs) { return (*this) = (*this) * rhs; }
  void print() {
    for (int i = 0; i < n_; ++i) {
      for (int j = 0; j < n_; ++j) {
        std::cout << a_[i][j] << ' ';
      }
      std::cout << '\n';
    }
  }
  friend Matrix pow(Matrix A, int n) {
    Matrix R(A.n_, valT(1));
    while (n) {
      if (n & 1) {
        R = R * A;
      }
      n >>= 1;
      A = A * A;
    }
    return R;
  }
};
}  // namespace cuzperf
