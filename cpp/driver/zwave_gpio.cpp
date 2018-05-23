/*
 * Copyright 2018 <Admobilize>
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

#include <iostream>
#include <string>

#include "cpp/driver/creator_memory_map.h"
#include "cpp/driver/zwave_gpio.h"

namespace matrix_hal {

ZwaveGPIO::ZwaveGPIO() {}

void ZwaveGPIO::Setup(MatrixIOBus *bus) {
  MatrixDriver::Setup(bus);
  bus_->Write(kZwaveGPIOBaseAddress + 1, control_);
}

bool ZwaveGPIO::SetProgramMode(uint8_t mode) {
  control_.direction = mode;
  bus_->Write(kZwaveGPIOBaseAddress + 1, control_);
}

bool ZwaveGPIO::SetReset(uint8_t nreset) {
  control_.nreset= nreset;
  bus_->Write(kZwaveGPIOBaseAddress + 1, control_);
}

bool ZwaveGPIO::SetMOSI(uint8_t value) {
  data_.mosi = value;
  bus_->Write(kZwaveGPIOBaseAddress, data_);
}

bool ZwaveGPIO::SetSCK(uint8_t value) {
  data_.sck = value;
  bus_->Write(kZwaveGPIOBaseAddress, data_);
}

bool ZwaveGPIO::SetCS(uint8_t value) {
  data_.cs = value;
  bus_->Write(kZwaveGPIOBaseAddress, data_);
}

bool ZwaveGPIO::GetMISO(uint8_t value) {
  bus_->Read(kZwaveGPIOBaseAddress, data_);
}


};  // namespace matrix_hal