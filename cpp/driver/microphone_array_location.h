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

#ifndef CPP_DRIVER_MICARRAY_LOCATION_H_
#define CPP_DRIVER_MICARRAY_LOCATION_H_

#include <string>
#include "./matrix_driver.h"
#include "./imu_data.h"

namespace matrix_hal {

/*
  x,y  position in milimeters
 */
float micarray_location[8][2] = {
    {19.708196, -47.579795},  /* M1 */
    {-20.291803, -47.579795}, /* M2 */
    {-48.420204, -19.708196}, /* M3 */
    {-48.420204, 20.291803},  /* M4 */
    {-29.291803, 48.420204},  /* M5 */
    {19.708196, 48.420204},   /* M6 */
    {47.579795, 20.291803},   /* M7 */
    {47.579795, -19.708196}   /* M8 */
};

};      // namespace matrix_hal
#endif  // CPP_DRIVER_MICARRAY_LOCATION_H_
