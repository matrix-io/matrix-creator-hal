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

#include <iostream>
#include <string>

#include "cpp/driver/creator_memory_map.h"
#include "cpp/driver/gpio_control.h"

namespace matrix_hal {

GPIOBank::GPIOBank() : mem_offset_(0x0), timer_setup_(0x0) {}

bool GPIOBank::SetupTimer(uint16_t channel, uint16_t init_event,
                          uint16_t final_event) {
  if (!bus_) return false;
  uint32_t mask = 1 << channel;
  timer_setup_ = init_event << channel | (timer_setup_ & ~mask);
  mask = 1 << (channel + 4);
  timer_setup_ = final_event << (channel + 4) | (timer_setup_ & ~mask);
  return bus_->Write(mem_offset_, timer_setup_);
}

bool GPIOBank::SetPeriod(uint16_t period) {
  if (!bus_) return false;
  return bus_->Write(mem_offset_ + 1, period);
}

bool GPIOBank::SetDuty(uint16_t channel, uint16_t duty) {
  if (!bus_) return false;
  return bus_->Write(mem_offset_ + 2 + channel, duty);
}

uint16_t GPIOBank::GetTimerCounter(uint16_t channel) {
  if (!bus_) return false;
  uint16_t counter;
  bus_->Read(mem_offset_ + 2 + channel, &counter);
  return counter;
}

GPIOControl::GPIOControl()
    : mode_(0x0), value_(0x0), function_(0x0), prescaler_(0x0) {}

bool GPIOControl::SetMode(unsigned char *pinList, int length, uint16_t mode) {
  if (!bus_) return false;

  for (int i = 0; i < length; i++) {
    uint32_t mask = 1 << pinList[i];
    mode_ = mode << pinList[i] | (mode_ & ~mask);
  }
  return bus_->Write(kGPIOBaseAddress, mode_);
}

bool GPIOControl::SetMode(uint16_t pin, uint16_t mode) {
  if (!bus_) return false;

  uint32_t mask = 1 << pin;

  mode_ = mode << pin | (mode_ & ~mask);
  return bus_->Write(kGPIOBaseAddress, mode_);
}

bool GPIOControl::SetFunction(uint16_t pin, uint16_t function) {
  if (!bus_) return false;

  uint32_t mask = 1 << pin;

  function_ = function << pin | (function_ & ~mask);

  return bus_->Write(kGPIOBaseAddress + 2, function_);
}

bool GPIOControl::SetPrescaler(uint16_t bank, uint16_t prescaler) {
  if (!bus_) return false;
  uint32_t mask = 0xF << (4 * bank);

  prescaler_ = prescaler << (4 * bank) | (prescaler_ & ~mask);

  return bus_->Write(kGPIOBaseAddress + 3, prescaler_);
}

bool GPIOControl::SetGPIOValue(uint16_t pin, uint16_t value) {
  if (!bus_) return false;
  uint32_t mask = 0x1 << pin;
  value_ = value << pin | (value_ & ~mask);
  return bus_->Write(kGPIOBaseAddress + 1, value_);
}

bool GPIOControl::SetGPIOValues(unsigned char *pinList, int length,
                                uint16_t value) {
  if (!bus_) return false;

  for (int i = 0; i < length; i++) {
    uint32_t mask = 0x1 << pinList[i];
    value_ = value << pinList[i] | (value_ & ~mask);
  }
  return bus_->Write(kGPIOBaseAddress + 1, value_);
}

uint16_t GPIOControl::GetGPIOValue(uint16_t pin) {
  if (!bus_) return false;

  uint32_t mask = 0x1 << pin;
  uint16_t value;

  bus_->Read(kGPIOBaseAddress + 1, &value);
  value = (value & mask) >> pin;

  return value;
}

uint16_t GPIOControl::GetGPIOValues() {
  if (!bus_) return false;
  uint16_t value;

  bus_->Read(kGPIOBaseAddress + 1, &value);

  return value;
}

uint16_t GPIOControl::GetIRValue() {
  if (!bus_) return false;
  uint16_t value;

  bus_->Read(kGPIOBaseAddress + 10, &value);

  return value;
}

bool GPIOControl::SetIR(uint16_t value) {
  if (!bus_) return false;
  return bus_->Write(kGPIOBaseAddress + 28, value);
}

bool GPIOControl::SetRingIR(uint16_t value) {
  if (!bus_) return false;
  return bus_->Write(kGPIOBaseAddress + 29, value);
}

void GPIOControl::Setup(MatrixIOBus *bus) {
  MatrixDriver::Setup(bus);
  uint32_t gpio_base_addr = kGPIOBaseAddress + 4;

  banks_.resize(4);
  for (GPIOBank &bank : banks_) {
    bank.Setup(bus);
    bank.mem_offset_ = gpio_base_addr;
    gpio_base_addr = gpio_base_addr + 6;
  }
}

bool GPIOControl::Set9GServoAngle(float angle, uint16_t pin) {
  if (!bus_) return false;
  if (pin > 15) return false;

  // Servo constants based on SG90 9g Micro Servo
  const float ServoRatio = 37.7;
  const int ServoOffset = 1800;

  // We choose a prescaler of 32 to work with a lower frequency
  // FPGA clock is being divided by 32
  // 32 = (1 << 5)
  const uint16_t GPIOPrescaler = 0x5;

  // We need 50Hz for servo, so 1 / 50Hz = 0.02 sec
  // from : https://en.wikipedia.org/wiki/Servo_(radio_control)
  const float period_seconds = 0.02;

  /* Getting period_counter to generate 50Hz:

  FPGAClock = 150000000
  FPGAClockAfterPrescaler = 150000000 / 32 = 4687500

  Period counter required for 50Hz
  period_counter = 0.02 / ( 1 / 4687500 ) = 93750

  FPGA firmware need only half of the period counter
  half_period_counter = period_counter / 2 = 46875

  When all math is combined you get
  final_period_counter =
      (period_seconds * FPGAClock / ((1 << GPIOPrescaler) * 2);

  */
  uint32_t period_counter =
      (period_seconds * bus_->FPGAClock()) / ((1 << GPIOPrescaler) * 2);

  // Using servo parameters to get duty
  uint16_t duty_counter = (ServoRatio * angle) + ServoOffset;

  uint16_t bank = pin / 4;
  uint16_t channel = pin % 4;

  bool b1 = SetPrescaler(bank, GPIOPrescaler);
  bool b2 = Bank(bank).SetPeriod(period_counter);
  bool b3 = Bank(bank).SetDuty(channel, duty_counter);

  return (b1 && b2 && b3);
}

bool GPIOControl::SetServoAngle(float angle, float min_pulse_ms, uint16_t pin) {
  if (!bus_) return false;
  if (pin > 15) return false;

  // We choose a prescaler of 32 to work with a lower frequency
  // FPGA clock is being divided by 32
  // 32 = (1 << 5)
  const uint16_t GPIOPrescaler = 0x5;

  // We need 50Hz for servo, so 1 / 50Hz = 0.02 sec
  // from : https://en.wikipedia.org/wiki/Servo_(radio_control)
  const float period_seconds = 0.02;

  /* Getting period_counter to generate 50Hz:

  FPGAClock = 150000000
  FPGAClockAfterPrescaler = 150000000 / 32 = 4687500

  Period counter required for 50Hz
  period_counter = 0.02 / ( 1 / 4687500 ) = 93750

  FPGA firmware need only half of the period counter
  half_period_counter = period_counter / 2 = 46875

  When all math is combined you get
  final_period_counter =
      (period_seconds * FPGAClock / ((1 << GPIOPrescaler) * 2);

  */
  uint32_t period_counter =
      (period_seconds * bus_->FPGAClock()) / ((1 << GPIOPrescaler) * 2);

  // Servo pulse width is symmetrical, with 1.5ms as neutral position
  // 1.5ms / 20ms = 0.075
  uint32_t ServoMiddle = period_counter * 0.075;
  uint32_t ServoOffset = period_counter * (min_pulse_ms / 20);
  float ServoRatio = (ServoMiddle - ServoOffset) / 90;

  // Using servo parameters to get duty
  uint16_t duty_counter = (ServoRatio * angle) + ServoOffset;

  uint16_t bank = pin / 4;
  uint16_t channel = pin % 4;

  bool b1 = SetPrescaler(bank, GPIOPrescaler);
  bool b2 = Bank(bank).SetPeriod(period_counter);
  bool b3 = Bank(bank).SetDuty(channel, duty_counter);

  return (b1 && b2 && b3);
}

bool GPIOControl::SetPWM(float frequency, float percentage, uint16_t pin) {
  if (!bus_) return false;
  if (pin > 15) return false;

  // We choose a prescaler of 32 to work with a lower frequency
  // FPGA clock is being divided by 32
  // 32 = (1 << 5)
  const uint16_t GPIOPrescaler = 0x5;
  float period_seconds = 1 / frequency;

  /* Getting period_counter to generate desired frequency:

  FPGAClock = 150000000
  FPGAClockAfterPrescaler = 150000000 / 32 = 4687500

  Period counter required for desired frequency
  period_counter = period_seconds / ( 1 / 4687500 )

  FPGA firmware need only half of the period counter
  half_period_counter = period_counter / 2

  When all math is combined you get
  final_period_counter =
      (period_seconds * FPGAClock / ((1 << GPIOPrescaler) * 2);

  */
  uint32_t period_counter =
      (period_seconds * bus_->FPGAClock()) / ((1 << GPIOPrescaler) * 2);
  uint16_t duty_counter = (period_counter * percentage) / 100;

  uint16_t bank = pin / 4;
  uint16_t channel = pin % 4;

  bool b1 = SetPrescaler(bank, GPIOPrescaler);
  bool b2 = Bank(bank).SetPeriod(period_counter);
  bool b3 = Bank(bank).SetDuty(channel, duty_counter);

  return (b1 && b2 && b3);
}

};  // namespace matrix_hal
