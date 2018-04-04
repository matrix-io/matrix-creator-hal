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

#include "../cpp/driver/everloop.h"
#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/gpio_control.h"
#include "../cpp/driver/imu_data.h"
#include "../cpp/driver/imu_sensor.h"
#include "../cpp/driver/matrixio_bus.h"

namespace hal = matrix_hal;
#define INPUT 0
#define OUTPUT 1
#define PIN_0 0
#define PIN_1 1

#define CLK_FRQ 200000000

int main() {
  hal::MatrixIOBus bus;
  if (!bus.Init()) return false;

  hal::GPIOControl gpio;
  gpio.Setup(&bus);
  unsigned char inputPinList[8] = {0, 2, 4, 6, 8, 10, 12, 14};
  unsigned char outputPinList[8] = {1, 3, 5, 7, 9, 11, 13, 15};

  gpio.SetMode(inputPinList, sizeof(inputPinList), INPUT);
  gpio.SetMode(outputPinList, sizeof(outputPinList), OUTPUT);

  uint16_t write_data = 1;
  gpio.SetGPIOValues(outputPinList, sizeof(outputPinList), write_data);
  uint16_t read_data = 0;

  while (true) {
    std::system("clear");
    read_data = gpio.GetGPIOValues();
    std::cout << "Read Value : " << std::hex << read_data << std::endl
              << std::endl;
    usleep(10000);
  }

  return 0;
}
