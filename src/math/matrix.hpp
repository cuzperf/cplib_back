#pragma once

#include <assert.h>
#include <vector>
#include <iostream>

namespace cuzperf {

template <int N, typename valT>
class Matrix {
  bool check() {
    if (a_.size() != N) return false;
    for (auto &x : a_) if (x.size() != N) return false;
    return true;
  }
 public:
  std::vector<std::vector<valT>> a_;
  Matrix(const std::vector<std::vector<valT>>& a) : a_(a) { assert(check()); }
  Matrix(std::vector<std::vector<valT>>&& a) : a_(std::move(a)) { assert(check()); }
  std::vector<std::vector<valT>> getOrigin() const { return a_;}
  Matrix(valT x = 0) : a_(N, std::vector<valT>(N)) {
    for (int i = 0; i < N; ++i) {
      a_[i][i] = x;
    }
  }
  void all(valT x) {
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        a_[i][j] = x;
      }
    }
  }
  Matrix& operator+=(const Matrix& rhs) {
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        a_[i][j] += rhs.a_[i][j];
      }
    }
    return (*this);
  }
  Matrix operator+(const Matrix& rhs) const { return Matrix(this) += rhs; }
  Matrix& operator-=(const Matrix& rhs) {
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        a_[i][j] -= rhs.a_[i][j];
      }
    }
    return (*this);
  }
  Matrix operator-(const Matrix& rhs) const { return Matrix(this) -= rhs; }
  Matrix operator*(const Matrix& rhs) const {
    Matrix R;
    for (int i = 0; i < N; ++i) {
      for (int k = 0; k < N; ++k) {
        for (int j = 0; j < N; ++j) {
          R.a_[i][j] += a_[i][k] * rhs.a_[k][j];
        }
      }
    }
    return R;
  }
  Matrix operator*=(const Matrix& rhs) { return (*this) = (*this) * rhs; }
  void print() {
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        std::cout << a_[i][j] << ' ';
      }
      std::cout << '\n';
    }
  }
  friend Matrix pow(Matrix A, int n) {
    Matrix R(valT(1));
    while (n) {
      if (n & 1) {
        R *= A;
      }
      n >>= 1;
      A *= A;
    }
    return R;
  }
};
}  // namespace cuzperf
