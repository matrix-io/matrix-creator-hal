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

#ifndef CPP_DRIVER_GPIO_CONTROL_H_
#define CPP_DRIVER_GPIO_CONTROL_H_
#include <cstdint>
#include <vector>
#include "./matrix_driver.h"

namespace matrix_hal {

class GPIOBank : public MatrixDriver {
 public:
  GPIOBank();
  bool SetupTimer(uint16_t channel, uint16_t init_event, uint16_t final_event);
  bool SetPeriod(uint16_t period);
  bool SetDuty(uint16_t channel, uint16_t duty);

  uint16_t GetTimerCounter(uint16_t channel);
  uint16_t mem_offset_;
  uint16_t timer_setup_;
};

class GPIOControl : public MatrixDriver {
 public:
  GPIOControl();
  void Setup(MatrixIOBus *bus);
  bool SetMode(unsigned char *pinList, int length, uint16_t mode);
  bool SetMode(uint16_t pin, uint16_t mode);
  bool SetFunction(uint16_t pin, uint16_t function);
  uint16_t GetGPIOValue(uint16_t pin);
  uint16_t GetGPIOValues();
  bool SetGPIOValue(uint16_t pin, uint16_t value);
  bool SetGPIOValues(unsigned char *pinList, int length, uint16_t value);
  uint16_t GetIRValue();
  bool SetIR(uint16_t value);
  bool SetRingIR(uint16_t value);

  bool SetPrescaler(uint16_t bank, uint16_t prescaler);
  GPIOBank &Bank(uint16_t bank) { return banks_[bank]; }

  bool Set9GServoAngle(float angle, uint16_t pin);
  bool SetServoAngle(float angle, float min_pulse_ms, uint16_t pin);
  bool SetPWM(float frequency, float percentage, uint16_t pin);

  std::vector<GPIOBank> banks_;
  uint16_t mode_;
  uint16_t value_;
  uint16_t function_;
  uint16_t prescaler_;
};

};      // namespace matrix_hal
#endif  // CPP_DRIVER_GPIO_CONTROL_H_
