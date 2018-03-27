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
#include "../cpp/driver/wishbone_bus.h"




namespace hal = matrix_hal;

int main() {
  hal::WishboneBus bus;

  if (!bus.SpiInit()) return false;

  hal::EverloopImage image1d(bus.MatrixLeds());

  hal::Everloop everloop;

  everloop.Setup(&bus);

  int rot = 0; // rotation
  int speed = 1; // speed

  for (hal::LedValue& led : image1d.leds) {
    led.red = 0;
    led.green = 0;
    led.blue = 0;
    led.white = 0;
  }
  everloop.Write(&image1d);
  printf("sice:%d\n", image1d.leds.size());
  usleep(1000000);

  while (1) {
    for (int i = 0; i < (int)image1d.leds.size(); i++) {
      int index = (int)((i + 1) / (float)image1d.leds.size() * 768);
      index += rot;
      index = index > 768 ? index - 768 : index;

      int red   = index < 256 ? 255 - index : index < 512 ? 0 : index - 512;
      int green = index < 256 ? index : index < 512 ? 512 - index : 0;
      int blue  = index < 256 ? 0 : index < 512 ? index - 256 : 768 - index;
      // printf("%d\t%d\t%d\n", red, green, blue);
      image1d.leds[i].red = red / 10;
      image1d.leds[i].green = green / 10;
      image1d.leds[i].blue = blue / 10;
      // image1d.leds[i].white = 10;
    }

    everloop.Write(&image1d);
    rot = rot - speed;
    rot = rot > 768 ? rot - 768 : rot;
    rot = rot < 0 ? 768 + rot : rot;
    usleep(1000);
  }

  return 0;
}
