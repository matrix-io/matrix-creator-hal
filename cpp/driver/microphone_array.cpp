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
#include <cmath>
#include <condition_variable>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <valarray>

#include "cpp/driver/creator_memory_map.h"
#include "cpp/driver/microphone_array.h"
#include "cpp/driver/microphone_array_location.h"

static std::mutex irq_m;
static std::condition_variable irq_cv;

void irq_callback(void) { irq_cv.notify_all(); }

namespace matrix_hal {

MicrophoneArray::MicrophoneArray()
    : lock_(irq_m), gain_(3), sampling_frequency_(16000) {
  raw_data_.resize(kMicarrayBufferSize);

  delayed_data_.resize(kMicarrayBufferSize);

  fifos_.resize(kMicrophoneChannels);

  beamformed_.resize(NumberOfSamples());

  CalculateDelays(0.0, 0.0);
}

MicrophoneArray::~MicrophoneArray() {}

void MicrophoneArray::Setup(WishboneBus *wishbone) {
  MatrixDriver::Setup(wishbone);

  wiringPiSetup();

  pinMode(kMicrophoneArrayIRQ, INPUT);
  ReadConfValues();
  wiringPiISR(kMicrophoneArrayIRQ, INT_EDGE_BOTH, &irq_callback);
}

//  Read audio from the FPGA and calculate beam using delay & sum method
bool MicrophoneArray::Read() {
  // TODO(andres.calderon@admobilize.com): avoid double buffer
  if (!wishbone_) return false;

  irq_cv.wait(lock_);

  for (int c = 0; c < 2; c++) {
    if (!wishbone_->SpiReadBurst(
            kMicrophoneArrayBaseAddress + c * 2047,
            reinterpret_cast<unsigned char *>(&raw_data_[c * 2047]),
            sizeof(int16_t) * 2047)) {
      return false;
    }
  }
  wishbone_->SpiReadBurst(kMicrophoneArrayBaseAddress,
                          reinterpret_cast<unsigned char *>(&raw_data_[4094]),
                          sizeof(int16_t) * 2);

  for (uint32_t s = 0; s < NumberOfSamples(); s++) {
    int sum = 0;
    for (int c = 0; c < kMicrophoneChannels; c++) {
      // delaying data for beamforming 'delay & sum' algorithm
      delayed_data_[s * kMicrophoneChannels + c] =
          fifos_[c].PushPop(raw_data_[c * NumberOfSamples() + s]);

      // accumulation data for beamforming 'delay & sum' algorithm
      sum += delayed_data_[s * kMicrophoneChannels + c];
    }

    beamformed_[s] = std::min(INT16_MAX, std::max(sum, INT16_MIN));
  }
  return true;
}

// Setting fifos for the 'delay & sum' algorithm
void MicrophoneArray::CalculateDelays(float azimutal_angle, float polar_angle,
                                      float radial_distance_mm,
                                      float sound_speed_mmseg) {
  if (sound_speed_mmseg == 0) {
    std::cerr << "Bad Configuration, Sound Speed must be greather than 0"
              << std::endl;
    return;
  }
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
    int delay = std::round((it->first - min_distance) * sampling_frequency_ /
                           sound_speed_mmseg);
    fifos_[it->second].Resize(delay);
  }
}

bool MicrophoneArray::GetGain() {
  if (!wishbone_) return false;
  uint16_t value;
  wishbone_->SpiRead16(kConfBaseAddress + 0x07, (unsigned char *)&value);
  gain_ = value;
  return true;
}

bool MicrophoneArray::SetGain(uint16_t gain) {
  if (!wishbone_) return false;
  wishbone_->SpiWrite16(kConfBaseAddress + 0x07, gain);
  gain_ = gain;
  return true;
}

bool MicrophoneArray::SetSamplingRate(uint32_t sampling_frequency) {
  if (sampling_frequency == 0) {
    std::cerr << "Bad Configuration, sampling_frequency must be greather than 0"
              << std::endl;
    return false;
  }

  uint16_t MIC_gain, MIC_constant;
  for (int i = 0;; i++) {
    if (MIC_sampling_frequencies[i][0] == 0) return false;
    if (sampling_frequency == MIC_sampling_frequencies[i][0]) {
      sampling_frequency_ = MIC_sampling_frequencies[i][0];
      MIC_constant = MIC_sampling_frequencies[i][1];
      MIC_gain = MIC_sampling_frequencies[i][2];
      break;
    }
  }

  sampling_frequency_ = sampling_frequency;
  SetGain(MIC_gain);
  wishbone_->SpiWrite16(kConfBaseAddress + 0x06, MIC_constant);

  return true;
}

bool MicrophoneArray::GetSamplingRate() {
  if (!wishbone_) return false;
  uint16_t value;
  wishbone_->SpiRead16(kConfBaseAddress + 0x06, (unsigned char *)&value);

  for (int i = 0;; i++) {
    if (MIC_sampling_frequencies[i][0] == 0) return false;
    if (value == MIC_sampling_frequencies[i][0]) {
      sampling_frequency_ = MIC_sampling_frequencies[i][0];
      break;
    }
  }

  return true;
}
void MicrophoneArray::ReadConfValues() {
  GetGain();
  GetSamplingRate();
}

void MicrophoneArray::ShowConfiguration() {
  std::cout << "Audio Configuration: " << std::endl;
  std::cout << "Sampling Frequency: " << sampling_frequency_ << std::endl;
  std::cout << "Gain : " << gain_ << std::endl;
}

};  // namespace matrix_hal
