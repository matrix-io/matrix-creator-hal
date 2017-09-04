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
#include <wiringPi.h>
#include <iostream>
#include <string>
#include "cpp/driver/uart_control.h"
#include "cpp/driver/creator_memory_map.h"

namespace matrix_hal {

const uint16_t kUartIRQ = 6;
const uint16_t UART_BUSY = 0x0010;

uint16_t UartControl::GetUartValue() {
  if (!wishbone_) return false;
  uint16_t value;
  if (waitForInterrupt(kUartIRQ, -1) > 0) {
    wishbone_->SpiRead16(kUartBaseAddress + 1, (unsigned char *)&value);
    return value;
  }
}

bool UartControl::GetUartUCR() {
  if (!wishbone_) return false;
  uint16_t value;
  wishbone_->SpiRead16(kUartBaseAddress, (unsigned char *)&value);
  ucr_ = value;
}

bool UartControl::SetUartValue(uint16_t data) {
  if (!wishbone_) return false;
  GetUartUCR();
  while (ucr_ & UART_BUSY);
  return wishbone_->SpiWrite16(kUartBaseAddress + 1, data);
}

UARTControl::UARTControl() : ucr_(0x0) {}

void UartControl::Setup(WishboneBus *wishbone) {
  MatrixDriver::Setup(wishbone);
  // TODO(andres.calderon@admobilize.com): avoid systems calls
  std::system("gpio edge 6 rise");

  wiringPiSetupSys();

  pinMode(kMicrophoneArrayIRQ, INPUT);
}
}