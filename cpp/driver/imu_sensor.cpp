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
#include "cpp/driver/imu_sensor.h"
#include "cpp/driver/creator_memory_map.h"

// Compass offset calibraiton values.12 values before * 2 indexes by value = 24
const uint16_t kMagOffsetsX = 0x18;
const uint16_t kMagOffsetsY = 0x1A;
const uint16_t kMagOffsetsZ = 0x1C;

namespace matrix_hal {

bool IMUSensor::Read(IMUData* data) {
  if (!wishbone_) return false;

  // TODO(andres.calderon@admobilize.com): error handler
  wishbone_->SpiRead(kMCUBaseAddress + (kMemoryOffsetIMU >> 1),
                     (unsigned char*)data, sizeof(IMUData));

  return true;
}

bool IMUSensor::SetCompassCalibration(float offset_x, float offset_y,
                                      float offset_z) {
  uint16_t add = kMCUBaseAddress + (kMemoryOffsetIMU >> 1);

  uint16_t *buffer = (uint16_t *)(&offset_x);
  wishbone_->SpiWrite16(add + kMagOffsetsX, buffer[0]);
  wishbone_->SpiWrite16(add + kMagOffsetsX + 1, buffer[1]);  

  buffer = (uint16_t *)(&offset_y);
  wishbone_->SpiWrite16(add + kMagOffsetsY, buffer[0]);
  wishbone_->SpiWrite16(add + kMagOffsetsY + 1, buffer[1]);

  buffer = (uint16_t *)(&offset_z);
  wishbone_->SpiWrite16(add + kMagOffsetsZ, buffer[0]);
  wishbone_->SpiWrite16(add + kMagOffsetsZ + 1, buffer[1]);

  return true;
}
};  // namespace matrix_hal
