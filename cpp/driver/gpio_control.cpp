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
#include "cpp/driver/gpio_control.h"
#include "cpp/driver/creator_memory_map.h"

namespace matrix_hal {

bool GPIO::EnablePWM(int prescaler) {
  if (!wishbone_) return false;
  control_ = 1;
  prescaler_ = prescaler;
  wishbone_->SpiWrite(mem_offset_, reinterpret_cast<uint8_t *>(&control_), 0);
  wishbone_->SpiWrite(mem_offset_ + 1, reinterpret_cast<uint8_t *>(&prescaler_),
                      0);
  return true;
}

bool GPIO::SetPeriod(int period) {
  if (!wishbone_) return false;
  counter_ = period;
  wishbone_->SpiWrite(mem_offset_ + 2, reinterpret_cast<uint8_t *>(&counter_),
                      0);
  return true;
}

bool GPIO::SetPrescaler(int prescaler) {
  if (!wishbone_) return false;
  prescaler_ = prescaler;
  wishbone_->SpiWrite(mem_offset_ + 1, reinterpret_cast<uint8_t *>(&prescaler_),
                      0);
  return true;
}

void GPIOControl::Setup(WishboneBus *wishbone) {
  MatrixDriver::Setup(wishbone);
  uint32_t gpio_base_addr = kGPIOBaseAddress;

  gpios_.resize(17);
  for (GPIO &gpio : gpios_) {
    gpio.Setup(wishbone);
    gpio.mem_offset_ = gpio_base_addr;
    gpio_base_addr = gpio_base_addr + 3;
  }
}

};  // namespace matrix_hal
