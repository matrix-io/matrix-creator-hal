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
#include <cmath>

#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/everloop.h"
#include "../cpp/driver/wishbone_bus.h"

namespace hal = matrix_hal;

int main() {
  hal::WishboneBus bus;

  bus.SpiInit();

  hal::Everloop everloop;
  hal::EverloopImage image1d;

  everloop.Setup(&bus);

 int red [18] = { 229, 158, 83, 11, 0, 83, 0, 0, 0, 0, 26, 106, 182, 210, 205, 200, 195, 191};
 int green [18] = { 221, 224, 219, 214, 210, 219, 200, 146, 80, 56, 0, 0, 0, 0, 0, 0, 59, 127};
 int blue [18] = { 0, 0, 0, 0, 0, 57, 185, 195, 191, 229, 224, 219, 214, 165, 86, 11, 0, 0};
 
 // double dim [18] = { 0.1, 0.1, 0.1, 0, 0.2, 0.5, 0.10, 0.30, 0.50, 0.80, 1, 0.50, 0.30, 0.10, 0.5, 0.2, 0.1, 0.1};



 double  gen_dim = 0.15;
 int  color_rot = 0;
 // int  dim_rot = 0;

  while (1) {
  // for (hal::LedValue& led : image1d.leds) {
    color_rot--;
    if (color_rot < 0) {
      color_rot = 17; 
    }

    // dim_rot++;
    // if (dim_rot > 17) {
    //   dim_rot = 0;
    // }

   for (int i = 0; i < 18; i++) {

    int color_i = i + color_rot;
    if(color_i > 17) 
      color_i = color_i - 17;

    // int dim_i = i + dim_rot;
    // if(dim_i > 17) 
    //   dim_i = dim_i - 17;

    // image1d.leds[i].red =   red[color_i] * gen_dim * dim[dim_i];
    // image1d.leds[i].green = green[color_i] * gen_dim * dim[dim_i];
    // image1d.leds[i].blue =  blue[color_i] * gen_dim * dim[dim_i];
    // image1d.leds[i].white =  0;


    image1d.leds[i].red =   red[color_i] * gen_dim ;
    image1d.leds[i].green = green[color_i] * gen_dim ;
    image1d.leds[i].blue =  blue[color_i] * gen_dim ;
    image1d.leds[i].white =  0;

  }
    everloop.Write(&image1d);
    usleep(80000);
  }

  return 0;
}
