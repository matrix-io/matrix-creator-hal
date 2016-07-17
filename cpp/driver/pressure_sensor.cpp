/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <string>
#include "cpp/driver/pressure_sensor.h"
#include "cpp/driver/creator_memory_map.h"

namespace matrix_hal {

bool PressureSensor::Read(PressureData* data) {
  if (!wishbone_) return false;

  // TODO(andres.calderon@admobilize.com): error handler
  wishbone_->SpiRead(kMCUBaseAddress + (kMemoryOffsetPressure >> 1),
                     (unsigned char*)data, sizeof(PressureData));

  return true;
}
};  // namespace matrix_hal
