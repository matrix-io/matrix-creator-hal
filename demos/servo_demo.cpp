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

#define CLK_FRQ 200000000

#define INPUT 0
#define OUTPUT 1

#define PIN_0 0
#define PIN_4 4

#define TIMER 1
#define PWM 1

#define RISING_EDGE 0
#define FALLING_EDGE 1

int main() {
  hal::WishboneBus bus;
  bus.SpiInit();

  hal::IMUSensor imu_sensor;
  imu_sensor.Setup(&bus);

  hal::Everloop everloop;
  everloop.Setup(&bus);

  hal::EverloopImage image1d;

  hal::IMUData imu_data;

  hal::GPIOControl gpio;
  gpio.Setup(&bus);

  gpio.SetMode(PIN_4, OUTPUT); /* pin 4, output */
  gpio.SetMode(PIN_0, INPUT);  /* pin 0, input */

  gpio.SetFunction(PIN_4, PWM);   /* pin 4, PWM output */
  gpio.SetFunction(PIN_0, TIMER); /* pin 0, Timer input */

  gpio.SetPrescaler(0, 0x5); /* set prescaler bank 0 */
  gpio.SetPrescaler(1, 0x5); /* set prescaler bank 1 */

  gpio.Bank(0).SetupTimer(0, RISING_EDGE, FALLING_EDGE); /* set timer event */

  uint16_t period_counter = (0.02 * CLK_FRQ) / ((1 << 5) * 2);
  int16_t duty_counter = 0;

  gpio.Bank(1).SetPeriod(period_counter);
  std::cout << " Period counter : " << period_counter << "\t" << std::endl;

  while (true) {
    for (hal::LedValue& led : image1d.leds) {
      led.red = 0;
      led.blue = 0;
    }

    imu_sensor.Read(&imu_data);

    int pin;
    if (imu_data.yaw < 0.0)
      pin = (360.0 + imu_data.yaw) * 35.0 / 360.0;
    else
      pin = imu_data.yaw * 35.0 / 360.0;

    image1d.leds[pin].blue = 50;

    pin = (pin + 17) % 34; /* opposite led */

    image1d.leds[pin].red = 50;
    duty_counter = (2.75 * (imu_data.yaw + 170) + 20) * (period_counter / 1000);
    gpio.Bank(1).SetDuty(0, duty_counter);

    everloop.Write(&image1d);

    std::cout << " Duty counter : " << duty_counter << "\t";

    usleep(5000);
    uint16_t result;
    result = gpio.Bank(0).GetTimerCounter(0);
    std::cout << "Timer Counter : " << result << std::endl;
  }

  return 0;
}
