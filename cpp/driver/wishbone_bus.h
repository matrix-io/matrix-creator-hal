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
#include <string>
#include <mutex>

namespace matrix_hal {

class WishboneBus {
 public:
  WishboneBus();

  bool SpiInit();
  bool SpiWrite(uint16_t add, unsigned char* data, unsigned char inc);
  bool SpiWrite16(uint16_t add, uint16_t data);

  bool SpiReadBurst(uint16_t add, unsigned char* data, int length);
  bool SpiRead(uint16_t add, unsigned char* data, int length);
  bool SpiRead16(uint16_t add, unsigned char* data);   //TODO(andres.calderon):Change type to uint16_t
  void SpiClose();

 private:
  bool SpiTransfer(unsigned char* send_buffer, unsigned char* receive_buffer,
                   unsigned int size);


 private:
  std::string device_name_;
  int spi_fd_;
  unsigned int spi_fifo_size_;
  unsigned int spi_mode_;
  unsigned int spi_bits_;
  uint32_t spi_speed_;
  unsigned int spi_delay_;
  unsigned char rx_buffer_[4096];
  unsigned char tx_buffer_[4096];
  mutable std::mutex mutex_;
};
};      // namespace matrix_hal
#endif  // CPP_DRIVER_WISHBONE_BUS_H_
