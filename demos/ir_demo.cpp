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
#include <cstdio>

#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/everloop.h"
#include "../cpp/driver/wishbone_bus.h"

namespace hal = matrix_hal;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "Device has not been specified" << std::endl
              << "Usage:" << std::endl << "   " << argv[0] << " device"
              << std::endl;
    return -1;
  }

  std::string irdevice = argv[1];

  std::cout << "Press \"p\" to Power up/down " << std::endl
            << "Press \"u\" to Volume UP " << std::endl
            << "Press \"d\" to Volume Down" << std::endl;

  hal::WishboneBus bus;

  bus.SpiInit();

  hal::Everloop everloop;
  hal::EverloopImage image1d;

  everloop.Setup(&bus);

  uint32_t counter = 0;

  while (1) {
    std::string cmd;
    switch (std::getchar()) {
      case 'p':
        std::cout << "Power \n";
        for (hal::LedValue& led : image1d.leds) {
          led.red = 0;
          led.green = 0;
          led.blue = 0;
          led.white = 0;
        }
        everloop.Write(&image1d);
        cmd = "irsend SEND_ONCE " + irdevice + " KEY_POWER";
        system(cmd.c_str());
        for (auto& led : image1d.leds) {
          led.red = 20;
          everloop.Write(&image1d);
          usleep(90000);
        }
        for (auto& led : image1d.leds) {
          led.red = 0;
          led.green = 0;
          led.blue = 0;
          led.white = 0;
        }
        usleep(90000);
        everloop.Write(&image1d);
        counter = 0;
        break;

      case 'u':
        std::cout << "Volume up \n";
        counter++;
        if (counter > (image1d.leds.size() - 1)) counter = 0;
        cmd = "irsend SEND_ONCE " + irdevice + " KEY_VOLUMEUP";
        system(cmd.c_str());
        image1d.leds[counter].blue = 0;
        image1d.leds[counter].green = 30;
        everloop.Write(&image1d);
        break;

      case 'd':
        std::cout << "Volume Down \n";
        cmd = "irsend SEND_ONCE " + irdevice + " KEY_VOLUMEDOWN";
        system(cmd.c_str());
        image1d.leds[counter].green = 0;
        image1d.leds[counter].blue = 30;
        everloop.Write(&image1d);
        if (counter == 0)
          counter = image1d.leds.size() - 1;
        else
          counter--;
        break;
    }
  }

  return 0;
}
