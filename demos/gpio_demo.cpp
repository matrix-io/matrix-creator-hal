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
#include "../cpp/driver/gpio_control.h"
#include "../cpp/driver/wishbone_bus.h"

namespace hal = matrix_hal;
#define INPUT 0
#define OUTPUT 1
#define PIN_0 0
#define PIN_1 1

#define CLK_FRQ 200000000

int main() {
  hal::WishboneBus bus;
  bus.SpiInit();

  hal::GPIOControl gpio;
  gpio.Setup(&bus);

  gpio.SetMode(PIN_0, INPUT);  /* pin 0, output */
  gpio.SetMode(PIN_1, OUTPUT); /* pin 1, input */
  uint16_t write_data = 0;
  uint16_t read_data = 0;

  while (true) {
    gpio.SetGPIOValue(PIN_1, write_data);
    usleep(100000);
    read_data = gpio.GetGPIOValue(PIN_0);
    std::cout << "Read Value : " << read_data << std::endl;
    write_data = ~write_data;
    usleep(100000);
  }

  return 0;
}
