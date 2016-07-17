/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <unistd.h>
#include <string>
#include <iostream>
#include "cpp/driver/everloop.h"
#include "cpp/driver/creator_memory_map.h"

namespace matrix_hal {

Everloop::Everloop() {}

bool Everloop::Write(const EverloopImage* led_image) {
  if (!wishbone_) return false;

  unsigned char wb_data_buffer[2];

  uint32_t addr_offset = 0;
  for (const LedValue& led : led_image->leds) {
    wb_data_buffer[0] = led.red;
    wb_data_buffer[1] = led.green;
    wishbone_->SpiWrite(kEverloopBaseAddress + addr_offset, wb_data_buffer, 0);

    wb_data_buffer[1] = led.blue;
    wb_data_buffer[0] = led.white;
    wishbone_->SpiWrite(kEverloopBaseAddress + addr_offset + 1, wb_data_buffer,
                        0);

    addr_offset = addr_offset + 2;
  }
  return true;
}
};  // namespace matrix_hal
