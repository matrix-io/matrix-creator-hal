/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
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
