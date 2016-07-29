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
#include <iostream>
#include "cpp/driver/gpio_control.h"
#include "cpp/driver/creator_memory_map.h"

namespace matrix_hal {

bool GPIOBank::SetPeriod(uint16_t period) {
  if (!wishbone_) return false;
  wishbone_->SpiWrite16(mem_offset_, period);
  return true;
}

bool GPIOBank::SetDuty(uint16_t channel, uint16_t duty) {
  if (!wishbone_) return false;
  wishbone_->SpiWrite16(mem_offset_ + 1 + channel, duty);
  return true;
}

GPIOControl::GPIOControl() : mode_(0x0), function_(0x0), prescaler_(0x0) {}

bool GPIOControl::SetMode(uint16_t pin, uint16_t mode) {
  if (!wishbone_) return false;

  uint32_t mask = 1 << pin;

  mode_ = mode << pin | (mode_ & ~mask);
  std::cout << "mode=" << mode_ << std::endl;
  wishbone_->SpiWrite16(kGPIOBaseAddress, mode_);
  return true;
}

bool GPIOControl::SetFunction(uint16_t pin, uint16_t function) {
  if (!wishbone_) return false;

  uint32_t mask = 1 << pin;

  function_ = function << pin | (function_ & ~mask);

  std::cout << "function_=" << function_ << std::endl;

  wishbone_->SpiWrite16(kGPIOBaseAddress + 3, function_);
  return true;
}

bool GPIOControl::SetPrescaler(uint16_t bank, uint16_t prescaler) {
  if (!wishbone_) return false;
  uint32_t mask = 0xF << (4 * bank);

  prescaler_ = prescaler << (4 * bank) | (prescaler_ & ~mask);

  std::cout << "prescaler_=" << prescaler_ << std::endl;

  wishbone_->SpiWrite16(kGPIOBaseAddress + 4, prescaler_);
  return true;
}

void GPIOControl::Setup(WishboneBus *wishbone) {
  MatrixDriver::Setup(wishbone);
  uint32_t gpio_base_addr = kGPIOBaseAddress + 5;

  banks_.resize(4);
  for (GPIOBank &bank : banks_) {
    bank.Setup(wishbone);
    bank.mem_offset_ = gpio_base_addr;
    gpio_base_addr = gpio_base_addr + 5;
  }
}

};  // namespace matrix_hal
