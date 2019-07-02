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

DirectionOfArrival::DirectionOfArrival(MicrophoneArray &mics) : mics_(mics) {}

bool DirectionOfArrival::Init() {
  length_ = mics_.NumberOfSamples();
  corr_ = new CrossCorrelation();
  corr_->Init(mics_.NumberOfSamples());
  current_mag_.resize(4);
  current_index_.resize(4);
  buffer_1D_.resize(mics_.Channels() * mics_.NumberOfSamples());
  buffer_2D_.resize(mics_.Channels());
  mic_direction_ = 0;
  azimutal_angle_ = 0;
  polar_angle_ = 0;
  for (uint16_t c = 0; c < mics_.Channels(); c++) {
    buffer_2D_[c] = &buffer_1D_[c * mics_.NumberOfSamples()];
  }
  return true;
}

int DirectionOfArrival::getAbsDiff(int index) {
  if (index < length_ / 2) {
    return index;
  }
  return length_ - 1 - index;
}

void DirectionOfArrival::Calculate() {
  // Max delay in samples between microphones of a pair
  int max_tof = 6;

  // Prepare buffer for cross correlation calculation between the microphones of
  // a pair
  for (uint32_t s = 0; s < mics_.NumberOfSamples(); s++) {
    for (uint16_t c = 0; c < mics_.Channels(); c++) { /* mics_.Channels()=8 */
      buffer_2D_[c][s] = mics_.At(s, c);
    }
  }

  // Loop over each microphone pair
  for (int channel = 0; channel < 4; channel++) {
    // Calculate the cross correlation
    corr_->Exec(buffer_2D_[channel + 4], buffer_2D_[channel]);

    float *c = corr_->Result();

    // Find the sample index of the highest peak (beginning of the window)
    int index = 0;
    float m = c[0];
    for (int i = 1; i < max_tof; i++)
      if (c[i] > m) {
        index = i;
        m = c[i];
      }

    // Find the sample index of the highest peak (end of the window)
    for (int i = length_ - max_tof; i < length_; i++)
      if (c[i] > m) {
        index = i;
        m = c[i];
      }

    // Store the highest value with the index of this microphone pair
    current_mag_[channel] = m;
    current_index_[channel] = index;
  }

  // Loop over all microphone pairs and find the microphone pair perpendicular
  // to the source
  int perp = 0;
  int index = current_index_[0];
  float mag = current_mag_[0];
  for (int channel = 0; channel < 4; channel++) {
    if (getAbsDiff(current_index_[channel]) < getAbsDiff(index)) {
      perp = channel;
      if (current_mag_[channel] > mag) mag = current_mag_[channel];
      index = current_index_[channel];
    }
  }

  // Determine the direction of the source (mic index)
  int dir = (perp + 2) % 4;
  if (current_index_[dir] > length_ / 2) {
    dir = (dir + 4);
  }

  // Calculate the physical angle
  mic_direction_ = dir;
  if (mics_.MatrixLeds() == 18)
    azimutal_angle_ = atan2(micarray_location_voice[mic_direction_][1],
                            micarray_location_voice[mic_direction_][0]);
  else
    azimutal_angle_ = atan2(micarray_location_creator[mic_direction_][1],
                            micarray_location_creator[mic_direction_][0]);
  polar_angle_ = fabs(index) * M_PI / 2.0 / float(max_tof - 1);
}

};  // namespace matrix_hal
