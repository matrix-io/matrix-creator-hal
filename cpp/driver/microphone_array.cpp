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

#include "cpp/driver/microphone_array.h"
#include "cpp/driver/creator_memory_map.h"

namespace matrix_hal {

MicrophoneArray::MicrophoneArray() : gain_(8) {
  raw_data_.resize(kMicarrayBufferSize);
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
};  // namespace matrix_hal
