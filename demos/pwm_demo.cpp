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

#include "../cpp/driver/gpio_control.h"
#include "../cpp/driver/matrixio_bus.h"

namespace hal = matrix_hal;

const int kFPGAClock = 150000000;
const uint16_t kGPIOOutputMode = 1;
const uint16_t kGPIOInputMode = 0;

const uint16_t kGPIOPrescaler = 0x5;
const uint16_t kPWMFunction = 1;

const float kPWMPeriod = 0.02;  // Seconds

int main() {
  std::cout << "Set desired Duty cycle in percentage" << std::endl << std::endl;

  hal::MatrixIOBus bus;
  if (!bus.Init()) return false;

  hal::GPIOControl gpio;
  gpio.Setup(&bus);

  for (int i = 0; i < 16; i++) {
    gpio.SetMode(i, kGPIOOutputMode);  /* pin 0, output */
    gpio.SetFunction(i, kPWMFunction); /* pin 0, pwm */
  }

  uint16_t period_counter =
      (kPWMPeriod * kFPGAClock) /
      ((1 << kGPIOPrescaler) * 2); /* Set Period counter for 20ms */

  uint16_t duty_counter;
  float percentage;
  int c_percentage;

  for (int j = 0; j < 4; j++) {
    gpio.SetPrescaler(j, kGPIOPrescaler); /* set prescaler bank 0 */
    gpio.Bank(j).SetPeriod(period_counter);
  }

  std::cout << " Period counter : " << period_counter << std::endl;

  while (true) {
    std::cout << " Percentage :";
    std::cin >> percentage;
    std::cout << std::endl;

    for (int k = 0; k < 4; k++)
      for (int c = 0; c < 4; c++) {
        c_percentage = (percentage - 10 * k - 5 * c);
        if (c_percentage < 0) c_percentage = 100 + c_percentage;
        duty_counter = (period_counter * c_percentage / 100);

        gpio.Bank(k).SetDuty(c, duty_counter);
        std::cout << " Duty counter "
                  << " [ %: " << c_percentage << " ] [ bank:" << k
                  << " ] [channel: " << c << " ] = " << duty_counter
                  << std::endl;
      }

    std::cout << std::endl;

    usleep(5000);
  }

  return 0;
}
