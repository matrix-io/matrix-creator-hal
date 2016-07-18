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

#ifndef CPP_DRIVER_MICROPHONE_ARRAY_H_
#define CPP_DRIVER_MICROPHONE_ARRAY_H_

#include <string>
#include <valarray>
#include "./matrix_driver.h"
#include "./pressure_data.h"

namespace matrix_hal {

const uint16_t kMicarrayBufferSize = 1024;
const uint32_t kMicrophoneArrayIRQ = 6;
const uint32_t kChannels = 8;

class MicrophoneArray : public MatrixDriver {
 public:
  MicrophoneArray();

  ~MicrophoneArray();

  void Setup(WishboneBus* wishbone);

  bool Read();

  int16_t NumberOfSamples() { return kMicarrayBufferSize / kChannels; }

  int16_t& At(int16_t sample, int16_t channel) {
    return raw_data_[sample * kChannels + channel];
  }

 private:
  std::valarray<int16_t> raw_data_;
};
};      // namespace matrix_hal
#endif  // CPP_DRIVER_MICROPHONE_ARRAY_H_
