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
#include <iostream>

#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/everloop.h"
#include "../cpp/driver/imu_data.h"
#include "../cpp/driver/imu_sensor.h"
#include "../cpp/driver/wishbone_bus.h"

namespace hal = matrix_hal;

int main() {
  hal::WishboneBus* bus = new hal::WishboneBus();
  bus->SpiInit();

  hal::IMUSensor imu_sensor;
  imu_sensor.Setup(bus);

  hal::Everloop everloop;
  everloop.Setup(bus);

  hal::EverloopImage image1d;

  hal::IMUData imu_data;

  while (true) {
    for (hal::LedValue& led : image1d.leds) {
      led.red = 0;
      led.white = 0;
    }

    imu_sensor.Read(&imu_data);

    int pin = imu_data.yaw * 35.0 / 360.0;
    pin = pin >= 0 ? pin : 35 - pin;

    image1d.leds[pin].white = 20;

    pin = (imu_data.yaw - 180.0) * 35.0 / 360.0;
    pin = pin >= 0 ? pin : 35 - pin;

    image1d.leds[pin].red = 30;

    everloop.Write(&image1d);

    std::cout << "yaw = " << imu_data.yaw << "\t";
    std::cout << "roll = " << imu_data.roll << "\t";
    std::cout << "pitch = " << imu_data.pitch << std::endl;
    usleep(500000);
  }

  return 0;
}
