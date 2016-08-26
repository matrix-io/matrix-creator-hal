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
#include "../cpp/driver/humidity_data.h"
#include "../cpp/driver/humidity_sensor.h"
#include "../cpp/driver/pressure_data.h"
#include "../cpp/driver/pressure_sensor.h"
#include "../cpp/driver/wishbone_bus.h"

namespace hal = matrix_hal;

int main() {
  hal::WishboneBus* bus = new hal::WishboneBus();
  bus->SpiInit();

  hal::IMUSensor imu_sensor;
  imu_sensor.Setup(bus);

  hal::HumiditySensor humidity_sensor;
  humidity_sensor.Setup(bus);

  hal::PressureSensor pressure_sensor;
  pressure_sensor.Setup(bus);

  hal::Everloop everloop;
  everloop.Setup(bus);

  hal::EverloopImage image1d;

  hal::IMUData imu_data;
  hal::HumidityData humidity_data;
  hal::PressureData pressure_data;

  while (true) {
    for (hal::LedValue& led : image1d.leds) {
      led.red = 0;
      led.blue = 0;
    }

    imu_sensor.Read(&imu_data);
    humidity_sensor.Read(&humidity_data);
    pressure_sensor.Read(&pressure_data);

    int pin;
    if (imu_data.yaw < 0.0)
      pin = (360.0 + imu_data.yaw) * 35.0 / 360.0;
    else
      pin = imu_data.yaw * 35.0 / 360.0;

    image1d.leds[pin].blue = 50;

    pin = (pin + 17) % 34; /* opposite led */

    image1d.leds[pin].red = 50;

    everloop.Write(&image1d);

    std::cout << "yaw = " << imu_data.yaw << "\t";
    std::cout << "roll = " << imu_data.roll << "\t";
    std::cout << "pitch = " << imu_data.pitch << std::endl;

    std::cout << "a " << imu_data.accel_x << ", " << imu_data.accel_y << ", "<< imu_data.accel_z << std::endl;
    std::cout << "g " << imu_data.gyro_x << ", " << imu_data.gyro_y << ", "<< imu_data.gyro_z << std::endl;
    std::cout << "m " << imu_data.mag_x << ", " << imu_data.mag_y  << ", "<< imu_data.mag_z << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << "humidity=" << humidity_data.humidity << " temperature=" << humidity_data.temperature << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << "pressure=" << pressure_data.pressure << " temperature=" << pressure_data.temperature << std::endl;
    std::cout << std::endl << std::endl;

    usleep(200000);
  }

  return 0;
}
