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

#include <iomanip>
#include <iostream>

#include "../cpp/driver/fw_data.h"
#include "../cpp/driver/matrixio_bus.h"
#include "../cpp/driver/mcu_firmware.h"

namespace hal = matrix_hal;

int main() {
  hal::MatrixIOBus bus;
  if (!bus.Init()) return false;

  hal::MCUFirmware mcu_firmware;
  mcu_firmware.Setup(&bus);

  hal::MCUData mcu_data;

  mcu_firmware.Read(&mcu_data);

  std::cout << "MCU ID = 0x" << std::hex << mcu_data.ID << std::endl;
  std::cout << "MCU version = 0x" << mcu_data.version << std::endl;

  return 0;
}
