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

#include <valarray>

#ifndef CPP_MATRIX_HAL_FIR_H_
#define CPP_MATRIX_HAL_FIR_H_

class FIR {
 public:
  void Setup(std::valarray<float>& coeff) {
    coeff_ = coeff;
    x_.resize(coeff_.size());
  }

  float Filter(float xo) {
    float acc = 0;
    x_ = x_.shift(-1);
    x_[0] = xo;
    for (unsigned int i = 0; i < x_.size(); i++) {
      acc += coeff_[i] * x_[i];
    }
    return acc;
  }

 private:
  std::valarray<float> x_;
  std::valarray<float> coeff_;
};

#endif  // CPP_MATRIX_HAL_FIR_H_
