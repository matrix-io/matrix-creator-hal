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

#ifndef CPP_DRIVER_WISHBONE_BUS_H_
#define CPP_DRIVER_WISHBONE_BUS_H_

#include <stdint.h>
#include <mutex>
#include <string>
#include "./bus.h"

namespace matrix_hal {

const uint32_t kFPGAClock = 50000000;  // Physical OSC = 50MHz
const int kMatrixCreatorNLeds = 35;
const int kMatrixVoiceNLeds = 18;
const int kMatrixCreator = 0x05C344E8;
const int kMatrixVoice = 0x6032BAD2;

class MatrixIOBus {
 public:
  MatrixIOBus();

  bool Init();

  bool Write(uint16_t add, unsigned char *data, int length);

  bool Read(uint16_t add, unsigned char *data, int length);

  bool Write(uint16_t add, uint16_t data);

  bool Read(uint16_t add, uint16_t *data);

  uint32_t FPGAClock() { return fpga_frequency_; }

  uint32_t MatrixName() { return matrix_name_; }

  uint32_t MatrixVersion() { return matrix_version_; }

  int MatrixLeds() { return matrix_leds_; }

  bool IsDirectBus() { return direct_nkernel_; }

 private:
  bool GetMatrixName();
  bool GetFPGAFrequency();

 private:
  uint32_t fpga_frequency_;  // Internal FPGA clock - DCM
  uint32_t matrix_name_;
  uint32_t matrix_version_;
  int matrix_leds_;
  Bus *bus_driver_;
  bool direct_nkernel_;
};
};      // namespace matrix_hal
#endif  // CPP_DRIVER_WISHBONE_BUS_H_
