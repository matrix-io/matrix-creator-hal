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

#ifndef CPP_DIRECTION_OF_ARRIVAL_H_
#define CPP_DIRECTION_OF_ARRIVAL_H_

#include <string>
#include <valarray>

#include "./cross_correlation.h"
#include "./microphone_array.h"

namespace matrix_hal {

class DirectionOfArrival {
 public:
  DirectionOfArrival(MicrophoneArray &mics);
  bool Init();

  void Calculate();

  float GetAzimutalAngle() { return azimutal_angle_; }
  float GetPolarAngle() { return polar_angle_; }
  int GetNearestMicrophone() { return mic_direction_; }

 private:
  MicrophoneArray &mics_;
  int length_;
  CrossCorrelation *corr_;
  std::valarray<float> current_mag_;
  std::valarray<float> current_index_;
  std::valarray<int16_t> buffer_1D_;
  std::valarray<int16_t *> buffer_2D_;

  int getAbsDiff(int index);

  uint16_t mic_direction_;
  float azimutal_angle_;
  float polar_angle_;
};
};      // namespace matrix_hal
#endif  // CPP_DIRECTION_OF_ARRIVAL_H_
