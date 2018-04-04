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

const int kFPGAClock = 200000000;
const uint16_t kGPIOOutputMode = 1;
const uint16_t kGPIOInputMode = 0;

const uint16_t kPin0 = 0;

const uint16_t kGPIOPrescaler = 0x5;
const uint16_t kPWMFunction = 1;

const float kPWMPeriod = 0.02;  // Seconds

/* Constants Values based on 9g ServoMotor Calibration */

const int kServoRatio = 37.7;
const int kServoOffset = 1800;

int main() {
  std::cout << "Set desired Angle in degrees" << std::endl << std::endl;

  hal::MatrixIOBus bus;
  if (!bus.Init()) return false;

  hal::Everloop everloop;
  everloop.Setup(&bus);

  hal::EverloopImage image1d(bus.MatrixLeds());

  hal::GPIOControl gpio;
  gpio.Setup(&bus);

  gpio.SetMode(kPin0, kGPIOOutputMode);  /* pin 0, output */
  gpio.SetFunction(kPin0, kPWMFunction); /* pin 0, pwm */

  gpio.SetPrescaler(0, kGPIOPrescaler); /* set prescaler bank 0 */

  uint16_t period_counter =
      (kPWMPeriod * kFPGAClock) /
      ((1 << kGPIOPrescaler) * 2); /* Set Period counter for 20ms */
  int16_t duty_counter;
  float angle;

  gpio.Bank(0).SetPeriod(period_counter);
  std::cout << " Period counter : " << period_counter << std::endl;

  while (true) {
    std::cout << " Angle :";
    std::cin >> angle;
    std::cout << std::endl;
    for (hal::LedValue &led : image1d.leds) {
      led.red = 0;
      led.blue = 0;
    }

    uint16_t ledAngle = -angle / 10 + 18;

    for (int i = (image1d.leds.size() / 2); i >= ledAngle; i--) {
      image1d.leds[i].blue = 50;
    }

    duty_counter = (kServoRatio * angle) + kServoOffset;
    gpio.Bank(0).SetDuty(0, duty_counter);

    everloop.Write(&image1d);

    std::cout << " Duty counter : " << duty_counter << std::endl;

    usleep(5000);
  }

  return 0;
}
