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

#ifndef CPP_DRIVER_UV_SENSOR_H_
#define CPP_DRIVER_UV_SENSOR_H_

#include <string>
#include "./matrix_driver.h"
#include "./uv_data.h"

namespace matrix_hal {

class UVSensor : public MatrixDriver {
 public:
  bool Read(UVData* data);
};
};      // namespace matrix_hal
#endif  // CPP_DRIVER_PRESSURE_SENSOR_H_
