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

namespace matrix_hal {

bool IMUSensor::Read(IMUData* data) {
  if (!wishbone_) return false;

  // TODO(andres.calderon@admobilize.com): error handler
  wishbone_->SpiRead(kMCUBaseAddress + (kMemoryOffsetIMU >> 1),
                     (unsigned char*)data, sizeof(IMUData));

  return true;
}

bool IMUSensor::SetCompassCalibration(IMUCalibrationData* data) {

  wishbone_->SpiWrite(kMCUBaseAddress + (kMemoryOffsetCalib >> 1),
                     (unsigned char*)data, sizeof(IMUCalibrationData));
  return true;
}

bool IMUSensor::SetControl(IMUControl* data) {

  wishbone_->SpiWrite(kMCUBaseAddress + (kMemoryOffsetControl >> 1),
                     (unsigned char*)data, sizeof(IMUCalibrationData));
  return true;
}

bool IMUSensor::ReadCalibration(IMUCalibrationData* data) {
  if (!wishbone_) return false;

  // TODO(andres.calderon@admobilize.com): error handler
  wishbone_->SpiRead(kMCUBaseAddress + (kMemoryOffsetCalib >> 1),
                     (unsigned char*)data, sizeof(IMUCalibrationData));

  return true;
}
};  // namespace matrix_hal
