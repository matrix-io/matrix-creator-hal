/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <unistd.h>
#include <iostream>

#include "../cpp/driver/imu_data.h"
#include "../cpp/driver/imu_sensor.h"
#include "../cpp/driver/wishbone_bus.h"

namespace hal = matrix_hal;

int main() {
  hal::WishboneBus* bus = new hal::WishboneBus();
  bus->SpiInit();

  hal::IMUSensor imu_sensor;
  imu_sensor.Setup(bus);

  hal::IMUData imu_data;

  while (true) {
    imu_sensor.Read(&imu_data);

    std::cout << "yaw = " << imu_data.yaw << std::endl;
    std::cout << "roll = " << imu_data.roll << std::endl;
    std::cout << "pitch = " << imu_data.pitch << std::endl;
    usleep(500000);
  }

  return 0;
}
