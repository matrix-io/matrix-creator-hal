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

#include <cmath>
#include <map>
#include <string>

#include "cpp/driver/cross_correlation.h"
#include "cpp/driver/direction_of_arrival.h"
#include "cpp/driver/microphone_array_location.h"

namespace matrix_hal {

DirectionOfArrival::DirectionOfArrival(MicrophoneArray& mics)
    : mics_(mics),
      length_(mics_.NumberOfSamples()),
      corr_(length_),
      current_mag_(4),
      current_index_(4),
      buffer_1D_(mics_.Channels() * mics_.SamplingRate()),
      buffer_2D_(mics_.Channels()),
      mic_direction_(0),
      azimutal_angle_(0.0),
      polar_angle_(0.0) {
  for (uint16_t c = 0; c < mics_.Channels(); c++) {
    buffer_2D_[c] = &buffer_1D_[c * mics_.SamplingRate()];
  }
}

void DirectionOfArrival::Calculate() {
  int max_tof = 6;

  for (uint32_t s = 0; s < mics_.NumberOfSamples(); s++) {
    for (uint16_t c = 0; c < mics_.Channels(); c++) { /* mics_.Channels()=8 */
      buffer_2D_[c][s] = mics_.At(s, c);
    }
  }

  for (int channel = 0; channel < 4; channel++) {
    corr_.Exec(buffer_2D_[channel + 4], buffer_2D_[channel]);

    float* c = corr_.Result();

    int index = 0;
    float m = c[0];
    for (int i = 1; i < max_tof; i++)
      if (c[i] > m) {
        index = i;
        m = c[i];
      }

    for (int i = length_ - max_tof; i < length_; i++)
      if (c[i] > m) {
        index = i;
        m = c[i];
      }

    current_mag_[channel] = m;
    current_index_[channel] = index;
  }

  int dir = 0;
  int index = current_index_[0];
  float mag = current_mag_[0];
  for (int channel = 1; channel < 4; channel++) {
    if (mag < current_mag_[channel]) {
      dir = channel;
      mag = current_mag_[channel];
      index = current_index_[channel];
    }
  }

  if (index > 64) index = -(128 - index);
  if (mag > 2e8) {
    if (index < 0) {
      mic_direction_ = (dir + 4);
    } else {
      mic_direction_ = dir;
    }
    azimutal_angle_ = atan2(micarray_location[mic_direction_][1],
                            micarray_location[mic_direction_][0]);
    polar_angle_ = fabs(index) * M_PI / 2.0 / float(max_tof - 1);
  }
}

};  // namespace matrix_hal
