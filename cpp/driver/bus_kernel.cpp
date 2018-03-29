/*
 * Copyright 2016-2018 <Admobilize>
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

#include "cpp/driver/bus_kernel.h"
#include "cpp/driver/creator_memory_map.h"

#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <linux/types.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

namespace matrix_hal {

#define WR_VALUE 1
#define RD_VALUE 2

BusKernel::BusKernel() : regmap_fd_(0) {}

BusKernel::~BusKernel() {
  if (regmap_fd_)
    Close();
}
bool BusKernel::Init(std::string device_name) {
  if (regmap_fd_)
    Close();

  if (device_name.size() == 0)
    device_name_ = "/dev/matrixio_regmap";
  else
    device_name_ = device_name;

  std::unique_lock<std::mutex> lock(mutex_);

  regmap_fd_ = open(device_name_.c_str(), O_RDWR);
  if (regmap_fd_ < 0) {
    std::cerr << "can't open device : " << device_name_ << std::endl;
    return false;
  }

  return true;
}

bool BusKernel::Read(uint16_t add, unsigned char *data, int length) {
  std::unique_lock<std::mutex> lock(mutex_);

  rx_buffer_[0] = add;
  rx_buffer_[1] = length;

  if (ioctl(regmap_fd_, RD_VALUE, rx_buffer_)) {
    return false;
  }

  memcpy(data, &rx_buffer_[2], length);

  return true;
}

bool BusKernel::Write(uint16_t add, unsigned char *data, int length) {
  std::unique_lock<std::mutex> lock(mutex_);

  tx_buffer_[0] = add;
  tx_buffer_[1] = length;

  memcpy(&tx_buffer_[2], data, length);

  if (ioctl(regmap_fd_, WR_VALUE, tx_buffer_)) {
    return false;
  }
  return true;
}

void BusKernel::Close(void) { close(regmap_fd_); }
}; // namespace matrix_hal
