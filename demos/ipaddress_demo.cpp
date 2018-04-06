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

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#include "../cpp/driver/everloop.h"
#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/matrixio_bus.h"

namespace hal = matrix_hal;

void SetNumber(int position, int value, hal::EverloopImage *image) {
  image->leds[position].blue = 0;
  image->leds[position].green = 20;
  image->leds[position].red = 40;

  for (int i = 0; i < value; i++) {
    image->leds[i + position + 1].blue = 0;
    image->leds[i + position + 1].blue = 30;
  }
}

int main() {
  int last_integer = 0;
  hal::MatrixIOBus bus;
  if (!bus.Init()) return false;
  hal::Everloop everloop;
  hal::EverloopImage image(bus.MatrixLeds());

  everloop.Setup(&bus);

  struct ifaddrs *ifaddr_struct = NULL;
  struct ifaddrs *ifa = NULL;
  void *tmp_addr_ptr = NULL;

  getifaddrs(&ifaddr_struct);

  for (ifa = ifaddr_struct; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == nullptr) {
      continue;
    }

    // check it is IP4
    if (ifa->ifa_addr->sa_family == AF_INET) {
      tmp_addr_ptr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
      char address_buffer[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, tmp_addr_ptr, address_buffer, INET_ADDRSTRLEN);
      printf("%s IP Address %s\n", ifa->ifa_name, address_buffer);

      // get IP for last network interface configured
      if (std::strcmp(ifa->ifa_name, "lo") != 0) {
        in_addr_t address = inet_addr(address_buffer);
        last_integer = (address >> 24) & 0xFF;
        printf("show last integer: %i\n", last_integer);
      }
    }
  }
  if (ifaddr_struct != nullptr) freeifaddrs(ifaddr_struct);

  for (hal::LedValue &led : image.leds) {
    led.red = 0;
    led.green = 0;
    led.blue = 0;
    led.white = 0;
  }
  everloop.Write(&image);

  const int first_digit = last_integer / 100;
  const int second_digit = last_integer % 100 / 10;
  const int third_digit = last_integer % 10;

  SetNumber(0, first_digit, &image);
  everloop.Write(&image);

  SetNumber(first_digit + 1, second_digit, &image);
  everloop.Write(&image);

  SetNumber(first_digit + second_digit + 2, third_digit, &image);
  everloop.Write(&image);

  return 0;
}
