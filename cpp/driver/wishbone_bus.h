/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
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
  bool SpiReadBurst(uint16_t add, unsigned char* data, int length);
  bool SpiRead(uint16_t add, unsigned char* data, int length);
  void SpiClose();

 private:
  bool SpiTransfer(unsigned char* send_buffer, unsigned char* receive_buffer,
                   unsigned int size);

  bool SpiRead16(uint16_t add, unsigned char* data);

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
