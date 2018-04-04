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
#include "../cpp/driver/matrixio_bus.h"

namespace hal = matrix_hal;

int main() {
  hal::MatrixIOBus bus;

  if (!bus.Init()) return false;

  hal::EverloopImage image1d(bus.MatrixLeds());

  hal::Everloop everloop;

  everloop.Setup(&bus);

  unsigned counter = 0;

  while (1) {
    for (hal::LedValue &led : image1d.leds) {
      led.red = 0;
      led.green = 0;
      led.blue = 0;
      led.white = 0;
    }
    image1d.leds[(counter / 2) % image1d.leds.size()].red = 20;
    image1d.leds[(counter / 7) % image1d.leds.size()].green = 30;
    image1d.leds[(counter / 11) % image1d.leds.size()].blue = 30;
    image1d.leds[image1d.leds.size() - 1 - (counter % image1d.leds.size())]
        .white = 10;

    everloop.Write(&image1d);
    ++counter;
    usleep(20000);
  }

  return 0;
}
