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

#include <unistd.h>
#include <cstdlib>
#include <iostream>

#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/everloop.h"
#include "../cpp/driver/uv_sensor.h"
#include "../cpp/driver/uv_data.h"
#include "../cpp/driver/wishbone_bus.h"

namespace hal = matrix_hal;

int main() {
  hal::WishboneBus* bus = new hal::WishboneBus();
  bus->SpiInit();

  hal::UVSensor uv_sensor;
  uv_sensor.Setup(bus);

  hal::Everloop everloop;
  everloop.Setup(bus);

  hal::EverloopImage image1d;
  hal::UVData uv_data;

  while (true) {
    uv_sensor.Read(&uv_data);

    std::cout << "UV = " << uv_data.uv << "  " << std::endl;

    usleep(200000);
  }

  return 0;
}
