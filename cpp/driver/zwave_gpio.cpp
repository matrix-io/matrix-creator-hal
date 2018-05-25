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

ZwaveGPIO::ZwaveGPIO() : data_(0), control_(0x2) {}  // NRESET = 1

void ZwaveGPIO::Setup(MatrixIOBus *bus) {
  MatrixDriver::Setup(bus);
  bus_->Write(kZwaveGPIOBaseAddress + 1, control_);
}

bool ZwaveGPIO::SetData(uint16_t pin, uint16_t value) {
  if (!bus_) return false;

  uint32_t mask = 1 << pin;

  data_ = value << pin | (data_ & ~mask);

  return bus_->Write(kZwaveGPIOBaseAddress, data_);
}

bool ZwaveGPIO::SetControl(uint16_t pin, uint16_t value) {
  if (!bus_) return false;

  uint32_t mask = 1 << pin;

  control_ = value << pin | (control_ & ~mask);

  return bus_->Write(kZwaveGPIOBaseAddress + 1, control_);
}

uint16_t ZwaveGPIO::GetMISO() {
  uint32_t mask = 0x1 << ZwaveData::MISO;

  bus_->Read(kZwaveGPIOBaseAddress, &data_);
  return (data_ & mask) >> ZwaveData::MISO;
}
};  // namespace matrix_hal
