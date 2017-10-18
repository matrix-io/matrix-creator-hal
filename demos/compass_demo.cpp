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
  hal::WishboneBus bus;
  bus.SpiInit();

  hal::IMUSensor imu_sensor;
  imu_sensor.Setup(&bus);

  hal::HumiditySensor humidity_sensor;
  humidity_sensor.Setup(&bus);

  hal::PressureSensor pressure_sensor;
  pressure_sensor.Setup(&bus);

  hal::Everloop everloop;
  everloop.Setup(&bus);

  hal::EverloopImage image1d;

  hal::IMUData imu_data;
  hal::HumidityData humidity_data;
  hal::PressureData pressure_data;

  std::cout << "yaw\t"
            << "pitch\t"
            << "roll\t"
            << "gyro_x\t"
            << "gyro_y\t"
            << "gyro_z\t"
            << "mag_x\t"
            << "mag_y\t"
            << "mag_z\t"
            << "off_x\t"
            << "off_y\t"
            << "off_z\t" << std::endl;

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

    printf("%.2f\t",imu_data.yaw );
    printf("%.2f\t",imu_data.pitch );
    printf("%.2f\t",imu_data.roll );
    printf("%.2f\t",imu_data.gyro_x );
    printf("%.2f\t",imu_data.gyro_y );
    printf("%.2f\t",imu_data.gyro_z );
    printf("%.2f\t",imu_data.mag_x );
    printf("%.2f\t",imu_data.mag_y );
    printf("%.2f\t",imu_data.mag_z );
    printf("%.2f\t",imu_data.mag_offset_x );
    printf("%.2f\t",imu_data.mag_offset_x );
    printf("%.2f\t",imu_data.mag_offset_x );
    printf("\n");

    // std::cout << imu_data.yaw << "\t"
    //           << imu_data.pitch << "\t"
    //           << imu_data.roll<< "\t"
    //           << imu_data.mag_x<< "\t"
    //           << imu_data.mag_y<< "\t"
    //           << imu_data.mag_z<< "\t"
    //           << imu_data.mag_offset_x<< "\t"
    //           << imu_data.mag_offset_x<< "\t"
    //           << imu_data.mag_offset_x << "\t"
    //           << std::endl;

    usleep(100000);
  }

  return 0;
}
