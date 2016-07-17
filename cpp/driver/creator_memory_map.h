/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#ifndef CPP_DRIVER_CREATOR_MEMORY_MAP_H_
#define CPP_DRIVER_CREATOR_MEMORY_MAP_H_

#include <string>

namespace matrix_hal {

/* FPGA Wishbone address map */
const uint32_t kEverloopBaseAddress = 0x2000;
const uint32_t kGPIOBaseAddress = 0x2800;
const uint16_t kMCUBaseAddress = 0x3800;

/* MCU offsets map */
const uint16_t kMemoryOffsetPressure = 0x10;
const uint16_t kMemoryOffsetHumidity = 0x20;
const uint16_t kMemoryOffsetIMU = 0x30;

};      // namespace matrix_hal
#endif  // CPP_DRIVER_CREATOR_MEMORY_MAP_H_
