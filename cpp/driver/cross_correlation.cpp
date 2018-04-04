/*
 * Copyright 2016 <Admobilize>
 * MATRIX Labs  [http://creator.matrix.one]
 * This file is part of MATRIX Creator HAL
 *
 * MATRIX Creator HAL is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cpp/driver/cross_correlation.h"
#include <cstring>

namespace matrix_hal {

CrossCorrelation::CrossCorrelation()
    : order_(0),
      in_(NULL),
      A_(NULL),
      B_(NULL),
      C_(NULL),
      c_(NULL),
      forward_plan_a_(NULL),
      forward_plan_b_(NULL),
      inverse_plan_(NULL) {}

CrossCorrelation::~CrossCorrelation() { Release(); }

void CrossCorrelation::Release() {
  if (forward_plan_a_) fftwf_destroy_plan(forward_plan_a_);
  if (forward_plan_b_) fftwf_destroy_plan(forward_plan_b_);
  if (inverse_plan_) fftwf_destroy_plan(inverse_plan_);

  if (in_) fftwf_free(in_);
  if (A_) fftwf_free(A_);
  if (B_) fftwf_free(B_);
  if (C_) fftwf_free(C_);
  if (c_) fftwf_free(c_);
}

bool CrossCorrelation::Init(int N) {
  order_ = N;
  /*
  fftwf_malloc function that behave identically to malloc, except that they
  guarantee that the returned pointer obeys any special alignment restrictions
  imposed by any algorithm in FFTW (e.g. for SIMD acceleration).
  */
  in_ = (float *)fftwf_malloc(sizeof(float) * order_);
  if (!in_) return false;

  A_ = (float *)fftwf_malloc(sizeof(float) * order_);
  if (!A_) return false;

  B_ = (float *)fftwf_malloc(sizeof(float) * order_);
  if (!B_) return false;

  C_ = (float *)fftwf_malloc(sizeof(float) * order_);
  if (!C_) return false;

  c_ = (float *)fftwf_malloc(sizeof(float) * order_);
  if (!c_) return false;

  forward_plan_a_ =
      fftwf_plan_r2r_1d(order_, in_, A_, FFTW_R2HC, FFTW_ESTIMATE);
  if (!forward_plan_a_) return false;

  forward_plan_b_ =
      fftwf_plan_r2r_1d(order_, in_, B_, FFTW_R2HC, FFTW_ESTIMATE);
  if (!forward_plan_b_) return false;

  inverse_plan_ = fftwf_plan_r2r_1d(order_, C_, c_, FFTW_HC2R, FFTW_ESTIMATE);
  if (!inverse_plan_) return false;

  return true;
}

float *CrossCorrelation::Result() { return c_; }

void CrossCorrelation::Exec(int16_t *a, int16_t *b) {
  for (int i = 0; i < order_; i++) {
    in_[i] = a[i];
  }

  fftwf_execute(forward_plan_a_);

  for (int i = 0; i < order_; i++) {
    in_[i] = b[i];
  }

  fftwf_execute(forward_plan_b_);

  Corr(C_, A_, B_);

  fftwf_execute(inverse_plan_);

  for (int i = 0; i < order_; i++) {
    c_[i] = c_[i] / order_;
  }
}

void CrossCorrelation::Corr(float *out, float *x, float *y) {
  std::memset(reinterpret_cast<void *>(out), 0, order_ * sizeof(float));

  out[0] = x[0] * y[0];                             // r0
  out[order_ / 2] = x[order_ / 2] * y[order_ / 2];  // r(n/2)

  for (int j = 1; j < order_ / 2; j++) {
    float a = x[j];
    float b = x[order_ - j];
    float c = y[j];
    float d = -y[order_ - j];
    out[j] += a * c - b * d;           // Re
    out[order_ - j] += b * c + a * d;  // Im
  }
}

};  // namespace matrix_hal
