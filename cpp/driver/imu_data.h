/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#ifndef CPP_DRIVER_IMU_DATA_H_
#define CPP_DRIVER_IMU_DATA_H_

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
};      // namespace matrix_hal
#endif  // CPP_DRIVER_IMU_DATA_H_
