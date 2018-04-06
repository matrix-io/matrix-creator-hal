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

#ifndef CPP_CROSS_CORRELATION_H_
#define CPP_CROSS_CORRELATION_H_

#include <fftw3.h>
#include <stdint.h>

namespace matrix_hal {

/*
Cross-correlation between signals implemented in frequency domain.
*/
class CrossCorrelation {
 public:
  CrossCorrelation();
  ~CrossCorrelation();
  bool Init(int N);
  void Release();
  void Exec(int16_t *a, int16_t *b);
  float *Result();

 private:
  void Corr(float *out, float *x, float *y);

  int order_;
  float *in_;
  float *A_;
  float *B_;
  float *C_;
  float *c_;

  fftwf_plan forward_plan_a_;
  fftwf_plan forward_plan_b_;
  fftwf_plan inverse_plan_;
};

};      // namespace matrix_hal
#endif  // CPP_CROSS_CORRELATION_H_
