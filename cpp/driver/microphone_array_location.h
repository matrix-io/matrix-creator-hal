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
#include "./imu_data.h"
#include "./matrix_driver.h"

namespace matrix_hal {

/*
  x,y  position in milimeters
 */

static float micarray_location_creator[8][2] = {
    {20.0908795, -48.5036755},  /* M1 */
    {-20.0908795, -48.5036755}, /* M2 */
    {-48.5036755, -20.0908795}, /* M3 */
    {-48.5036755, 20.0908795},  /* M4 */
    {-20.0908795, 48.5036755},  /* M5 */
    {20.0908795, 48.5036755},   /* M6 */
    {48.5036755, 20.0908795},   /* M7 */
    {48.5036755, -20.0908795}   /* M8 */
};

static float micarray_location_voice[8][2] = {
    {00.00, 00.00},  /* M1 */
    {-38.13, 3.58},  /* M2 */
    {-20.98, 32.04}, /* M3 */
    {11.97, 36.38},  /* M4 */
    {35.91, 13.32},  /* M5 */
    {32.81, -19.77}, /* M6 */
    {5.00, -37.97},  /* M7 */
    {-26.57, -27.58} /* M8 */
};

};      // namespace matrix_hal
#endif  // CPP_DRIVER_MICARRAY_LOCATION_H_
