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

#include <wiringPi.h>
#include <string>
#include <cstdlib>
#include <cmath>
#include <iostream>

#include "cpp/driver/microphone_array.h"
#include "cpp/driver/creator_memory_map.h"
#include "cpp/driver/microphone_array_location.h"

namespace matrix_hal {

MicrophoneArray::MicrophoneArray() : gain_(8) {
  raw_data_.resize(kMicarrayBufferSize);
  delay_.resize(kMicrophoneChannels);
  delay_ = 0;
}

MicrophoneArray::~MicrophoneArray() {}

void MicrophoneArray::Setup(WishboneBus* wishbone) {
  MatrixDriver::Setup(wishbone);

  // TODO(andres.calderon@admobilize.com): avoid systems calls
  std::system("gpio edge 6 both");

  wiringPiSetupSys();

  pinMode(kMicrophoneArrayIRQ, INPUT);
}

bool MicrophoneArray::Read() {
  if (!wishbone_) return false;

  if (waitForInterrupt(kMicrophoneArrayIRQ, -1) > 0) {
    if (!wishbone_->SpiReadBurst(
            kMicrophoneArrayBaseAddress,
            reinterpret_cast<unsigned char*>(&raw_data_[0]),
            sizeof(int16_t) * kMicarrayBufferSize)) {
      return false;
    }

    for (auto& data : raw_data_) data = data * gain_;
  }

  return true;
}

void MicrophoneArray::CalculateDelays(float azimutal_angle, float polar_angle,
                                      float radial_distance_mm,
                                      float sound_speed_mmseg) {
  /*
    sound source position
  */
  float x, y, z;
  x = radial_distance_mm * std::sin(azimutal_angle) * std::cos(polar_angle);
  y = radial_distance_mm * std::sin(azimutal_angle) * std::sin(polar_angle);
  z = radial_distance_mm * std::cos(azimutal_angle);

  std::valarray<float> distance(kMicrophoneChannels);

  for (int c = 0; c < kMicrophoneChannels; c++) {
    distance[c] = std::sqrt(std::pow(micarray_location[c][0] - x, 2.0) +
                            std::pow(micarray_location[c][1] - y, 2.0) +
                            std::pow(z, 2.0));
    std::cout << distance[c] << std::endl;
  }
}

};  // namespace matrix_hal
