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
    : device_name_("/dev/spidev0.0"),
      spi_mode_(3),
      spi_bits_(8),
      spi_speed_(10000000),
      spi_delay_(0),
      fpga_frequency_(125000000) {}

bool WishboneBus::SpiInit() {
  std::unique_lock<std::mutex> lock(mutex_);

  spi_fd_ = open(device_name_.c_str(), O_RDWR);
  if (spi_fd_ < 0) {
    std::cerr << "can't open device" << std::endl;
    return false;
  }

  if (ioctl(spi_fd_, SPI_IOC_WR_MODE, &spi_mode_) == -1) {
    std::cerr << "can't set spi mode" << std::endl;
    return false;
  }

  if (ioctl(spi_fd_, SPI_IOC_RD_MODE, &spi_mode_) == -1) {
    std::cerr << "can't get spi mode" << std::endl;
    return false;
  }

  /*
   * bits per word
   */
  if (ioctl(spi_fd_, SPI_IOC_WR_BITS_PER_WORD, &spi_bits_) == -1) {
    std::cerr << "can't set bits per word" << std::endl;
    return false;
  }

  if (ioctl(spi_fd_, SPI_IOC_RD_BITS_PER_WORD, &spi_bits_) == -1) {
    std::cerr << "can't get bits per word" << std::endl;
    return false;
  }

  /*
   * max speed hz
   */
  if (ioctl(spi_fd_, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed_) == -1) {
    std::cerr << "can't set max speed Hz" << std::endl;
    return false;
  }

  if (ioctl(spi_fd_, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed_) == -1) {
    std::cerr << "can't get max speed Hz" << std::endl;
    return false;
  }

  return true;
}

bool WishboneBus::SpiTransfer(unsigned char *send_buffer,
                              unsigned char *receive_buffer,
                              unsigned int size) {
  spi_ioc_transfer tr;
  memset(&tr, 0, sizeof(tr));

  tr.tx_buf = (uint64_t)send_buffer;
  tr.rx_buf = (uint64_t)receive_buffer;
  tr.len = size;
  tr.delay_usecs = spi_delay_;
  tr.speed_hz = spi_speed_;
  tr.bits_per_word = spi_bits_;

  if (ioctl(spi_fd_, SPI_IOC_MESSAGE(1), &tr) < 1) {
    std::cerr << "can't send spi message" << std::endl;
    return false;
  }
  return true;
}

bool WishboneBus::SpiWrite16(uint16_t add, uint16_t data) {
  std::unique_lock<std::mutex> lock(mutex_);

  hardware_address* hw_addr = reinterpret_cast<hardware_address*>(tx_buffer_);
  hw_addr->reg = add;
  hw_addr->burst = 0;
  hw_addr->readnwrite = 0;

  uint8_t *p = (uint8_t *)&data;

  tx_buffer_[2] = p[0];
  tx_buffer_[3] = p[1];

  return SpiTransfer(tx_buffer_, rx_buffer_, 4);
}

bool WishboneBus::SpiWrite(uint16_t add, unsigned char *data, int length) {

  uint16_t *words = reinterpret_cast<uint16_t *>(data);

  for (uint16_t w = 0; w < (length / 2); w++) {
    if (!SpiWrite16(add + w, words[w])) return false;
  }
  return true;
}

bool WishboneBus::SpiReadBurst(uint16_t add, unsigned char *data, int length) {
  std::unique_lock<std::mutex> lock(mutex_);

  hardware_address* hw_addr = reinterpret_cast<hardware_address*>(tx_buffer_);
  hw_addr->reg = add;
  hw_addr->burst = 1;
  hw_addr->readnwrite = 1;

  if (SpiTransfer(tx_buffer_, rx_buffer_, length + 2)) {
    memcpy(data, &rx_buffer_[2], length);
    return true;
  }
  return false;
}

bool WishboneBus::SpiRead(uint16_t add, unsigned char *data, int length) {
  uint16_t *words = reinterpret_cast<uint16_t *>(data);

  for (uint16_t w = 0; w < (length / 2); w++) {
    if (!SpiRead16(add + w,  (unsigned char *)&words[w])) return false;
  }
  return true;
}

bool WishboneBus::SpiRead16(uint16_t add, unsigned char *data) {
  std::unique_lock<std::mutex> lock(mutex_);

  const int length = 2;

  hardware_address* hw_addr = reinterpret_cast<hardware_address*>(tx_buffer_);
  hw_addr->reg = add;
  hw_addr->burst = 0;
  hw_addr->readnwrite = 1;

  if (SpiTransfer(tx_buffer_, rx_buffer_, length + 2)) {
    memcpy(data, &rx_buffer_[2], length);
    return true;
  }
  return false;
}

bool WishboneBus::GetSoftwareVersion(char *version, int length) {
  if (!SpiRead(kConfBaseAddress, (unsigned char *)version, length))
    return false;
  return true;
}

bool WishboneBus::GetFPGAFrequency() {
  uint16_t values[2];
  if (!SpiRead(kConfBaseAddress + 4, (unsigned char *)values, sizeof(values)))
    return false;
  fpga_frequency_ = (kFPGAClock * values[1]) / values[0];
  return true;
}

void WishboneBus::SpiClose(void) { close(spi_fd_); }
};  // namespace matrix_hal