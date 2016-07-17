/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <string>
#include "cpp/driver/imu_sensor.h"
#include "cpp/driver/creator_memory_map.h"

namespace matrix_hal {

bool IMUSensor::Read(IMUData* data) {
  if (!wishbone_) return false;

  // TODO(andres.calderon@admobilize.com): error handler
  wishbone_->SpiRead(kMCUBaseAddress + (kMemoryOffsetIMU >> 1),
                     (unsigned char*)data, sizeof(IMUData));

  return true;
}
};  // namespace matrix_hal
