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

#ifndef CPP_DRIVER_IMU_DATA_H_
#define CPP_DRIVER_IMU_DATA_H_

const int32_t OFFSET_WRITE_ENABLE = 0x25352535;

namespace matrix_hal {

class IMUData {
 public:
  // Vertical axis (yaw)
  float yaw;

  // Lateral axis (pitch)
  float pitch;

  // Longitudinal axis (roll)
  float roll;

  float accel_x;
  float accel_y;
  float accel_z;

  float gyro_x;
  float gyro_y;
  float gyro_z;

  float mag_x;
  float mag_y;
  float mag_z;  
};

class IMUControl{
  public:
  int32_t mag_ofsset_wr_flag;
};

class IMUCalibrationData {
public:
  // Calibration offset of compass x axis
  int32_t mag_offset_x;
  // Calibration offset of compass y axis
  int32_t mag_offset_y;
  // Calibration offset of compass z axis
  int32_t mag_offset_z;
};
};      // namespace matrix_hal
#endif  // CPP_DRIVER_IMU_DATA_H_
