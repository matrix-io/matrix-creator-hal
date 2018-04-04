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

#include <string>

#include "cpp/driver/creator_memory_map.h"
#include "cpp/driver/humidity_sensor.h"

namespace matrix_hal {

bool HumiditySensor::Read(HumidityData *data) {
  if (!bus_) return false;
  union {
    int *int_data;
    float *float_data;
  };

  float_data = (float *)data;

  bus_->Read(kMCUBaseAddress + (kMemoryOffsetHumidity >> 1),
             (unsigned char *)data, sizeof(HumidityData));

  for (uint16_t i = 0; i < sizeof(HumidityData) / sizeof(int); i++)
    float_data[i] = float(int_data[i]) / 1000.0;

  return true;
}
};  // namespace matrix_hal
