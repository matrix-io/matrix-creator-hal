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
#include <cstdint>
#include <cmath>
#include <map>

#include "cpp/driver/microphone_array.h"
#include "cpp/driver/creator_memory_map.h"
#include "cpp/driver/microphone_array_location.h"

namespace matrix_hal {

MicrophoneArray::MicrophoneArray() : gain_(8) {
  raw_data_.resize(kMicarrayBufferSize);

  delayed_data_.resize(kMicarrayBufferSize);

  fifos_.resize(kMicrophoneChannels);

  beamformed_.resize(NumberOfSamples());

  CalculateDelays(0.0, 0.0);
}

MicrophoneArray::~MicrophoneArray() {}

void MicrophoneArray::Setup(WishboneBus* wishbone) {
  MatrixDriver::Setup(wishbone);

  // TODO(andres.calderon@admobilize.com): avoid systems calls
  std::system("gpio edge 23 both");

  wiringPiSetupSys();

  pinMode(kMicrophoneArrayIRQ, INPUT);
}

//  Read audio from the FPGA and calculate beam using delay & sum method
bool MicrophoneArray::Read() {
  // TODO(andres.calderon@admobilize.com): avoid double buffer
  if (!wishbone_) return false;

  if (waitForInterrupt(kMicrophoneArrayIRQ, -1) > 0) {
    if (!wishbone_->SpiReadBurst(
            kMicrophoneArrayBaseAddress,
            reinterpret_cast<unsigned char*>(&raw_data_[0]),
            sizeof(int16_t) * kMicarrayBufferSize)) {
      return false;
    }

    for (uint32_t s = 0; s < NumberOfSamples(); s++) {
      int sum = 0;
      for (uint16_t c = 0; c < kMicrophoneChannels; c++) {
        delayed_data_[s * kMicrophoneChannels + c] =
            fifos_[c].PushPop(raw_data_[s * kMicrophoneChannels + c]) * gain_;

        sum += delayed_data_[s * kMicrophoneChannels + c];
      }

      beamformed_[s] = std::min(INT16_MAX, std::max(sum, INT16_MIN));
    }
  }

  return true;
}

void MicrophoneArray::CalculateDelays(float azimutal_angle, float polar_angle,
                                      float radial_distance_mm,
                                      float sound_speed_mmseg) {
  //  sound source position
  float x, y, z;
  x = radial_distance_mm * std::sin(azimutal_angle) * std::cos(polar_angle);
  y = radial_distance_mm * std::sin(azimutal_angle) * std::sin(polar_angle);
  z = radial_distance_mm * std::cos(azimutal_angle);

  std::map<float, int> distance_map;

  // sorted distances from source position to each microphone
  for (int c = 0; c < kMicrophoneChannels; c++) {
    const float distance = std::sqrt(
        std::pow(micarray_location[c][0] - x, 2.0) +
        std::pow(micarray_location[c][1] - y, 2.0) + std::pow(z, 2.0));
    distance_map[distance] = c;
  }

  // fifo resize for delay compensation
  float min_distance = distance_map.begin()->first;
  for (std::map<float, int>::iterator it = distance_map.begin();
       it != distance_map.end(); ++it) {
    int delay = std::round((it->first - min_distance) * kSamplingRate /
                           sound_speed_mmseg);
    fifos_[it->second].Resize(delay);
  }
}

};  // namespace matrix_hal
