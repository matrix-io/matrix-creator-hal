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

#include "cpp/driver/wishbone_bus.h"
#include <errno.h>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <linux/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "cpp/driver/creator_memory_map.h"

namespace matrix_hal {

struct hardware_address {
  uint8_t readnwrite : 1;
  uint8_t burst : 1;
  uint16_t reg : 14;
};

WishboneBus::WishboneBus()
{}

bool WishboneBus::SpiInit() {
  return true;
}

bool WishboneBus::SpiTransfer(unsigned char *send_buffer,
                              unsigned char *receive_buffer,
                              unsigned int size) {
  return true;
}

bool WishboneBus::SpiWrite16(uint16_t add, uint16_t data) {
  return true;
}

bool WishboneBus::SpiWrite(uint16_t add, unsigned char *data, int length) {
  return true;
}

bool WishboneBus::SpiReadBurst(uint16_t add, unsigned char *data, int length) {
  return true;
}

bool WishboneBus::SpiRead(uint16_t add, unsigned char *data, int length) {
  return true;
}

bool WishboneBus::SpiRead16(uint16_t add, unsigned char *data) {
  return true;
}

bool WishboneBus::GetSoftwareVersion(char *version, int length) {
  return true;
}

bool WishboneBus::GetFPGAFrequency() {
  return true;
}

void WishboneBus::SpiClose(void) {  }
};  // namespace matrix_hal
