/*
 * Copyright 2018 <Admobilize>
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
#define CPP_DRIVER_ZWAVE_GPIO_H_
#include <cstdint>
#include <vector>
#include "./matrix_driver.h"

namespace matrix_hal {

struct ZwaveData {
  ZwaveData() {
    cs = 0;
    sck = 0;
    mosi = 0;
    miso = 0;
  };

  uint8_t cs : 1;
  uint8_t sck : 1;
  uint8_t mosi : 1;
  uint8_t miso : 1;
};

struct ZwaveControl {
  ZwaveControl() {
    direction = 0;
    nreset = 1;
  };
  uint8_t direction : 1;
  uint8_t nreset : 1;
};

class ZwaveGPIO : public MatrixDriver {
 public:
  ZwaveGPIO();
  ~ZwaveGPIO();

  void Setup(MatrixIOBus *bus);
  bool SetProgramMode(uint8_t mode);
  bool SetMOSI(uint8_t value);
  bool SetSCK(uint8_t value);
  bool SetCS(uint8_t value);
  uint8_t MISO(){return data_.miso};


 private:
  ZwaveData data_;
  ZwaveControl control_;
};

};      // namespace matrix_hal
#endif  // CPP_DRIVER_ZWAVE_GPIO_H_
