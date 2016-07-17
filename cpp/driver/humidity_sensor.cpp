/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <string>
#include "cpp/driver/humidity_sensor.h"
#include "cpp/driver/creator_memory_map.h"

namespace matrix_hal {

bool HumiditySensor::Read(HumidityData* data) {
  if (!wishbone_) return false;

  // TODO(andres.calderon@admobilize.com): error handler
  wishbone_->SpiRead(kMCUBaseAddress + (kMemoryOffsetHumidity >> 1),
                     (unsigned char*)data, sizeof(HumidityData));

  return true;
}
};  // namespace matrix_hal
