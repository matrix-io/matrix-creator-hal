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
#include <valarray>
#include <cstdlib>
#include <iostream>

#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/everloop.h"
#include "../cpp/driver/uv_sensor.h"
#include "../cpp/driver/uv_data.h"
#include "../cpp/driver/wishbone_bus.h"

/* UV Index

  http://www.who.int/uv/intersunprogramme/activities/uv_index/en/
  https://en.wikipedia.org/wiki/Ultraviolet_index#How_to_use_the_index

    0-2.9  Green  "Low"
    3-5.9  Yellow "Moderate"
    6-7.9  Orange "High"
    8-10.9 Red    "Very high"
   11+     Violet "Extreme"
*/

namespace hal = matrix_hal;

struct UVTable {
  float limit;
  struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
  } color;
};

int main() {
  std::valarray<UVTable> who_uv_table = {
      {11.0, {255, 0, 255, 0}}, /* Violet "Extreme" */
      {8.0, {255, 0, 0, 0}},    /* Red    "Very high" */
      {6.0, {255, 128, 0, 0}},  /* Orange "High" */
      {3.0, {255, 255, 0, 0}},  /* Yellow "Moderate" */
      {0.0, {0, 255, 0, 0}}     /* Green  "Low" */
  };

  hal::WishboneBus bus;
  bus.SpiInit();

  hal::UVSensor uv_sensor;
  uv_sensor.Setup(&bus);

  hal::Everloop everloop;
  everloop.Setup(&bus);

  hal::EverloopImage image1d;
  hal::UVData uv_data;

  while (true) {
    uv_sensor.Read(&uv_data);
    for (auto& uv_item : who_uv_table) {
      if (uv_data.uv >= uv_item.limit) {
        for (auto& led : image1d.leds) {
          led.red = uv_item.color.r;
          led.green = uv_item.color.g;
          led.blue = uv_item.color.b;
          led.white = uv_item.color.w;
        }
        break;
      }
    }
    everloop.Write(&image1d);

    std::cout << "UV = " << uv_data.uv << "  " << std::endl;

    usleep(200000);
  }

  return 0;
}
