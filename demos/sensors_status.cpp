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

#include <unistd.h>
#include <cstdlib>
#include <iomanip>
#include <iostream>

#include "../cpp/driver/fw_data.h"
#include "../cpp/driver/humidity_data.h"
#include "../cpp/driver/humidity_sensor.h"
#include "../cpp/driver/imu_data.h"
#include "../cpp/driver/imu_sensor.h"
#include "../cpp/driver/matrixio_bus.h"
#include "../cpp/driver/mcu_firmware.h"
#include "../cpp/driver/pressure_data.h"
#include "../cpp/driver/pressure_sensor.h"
#include "../cpp/driver/uv_data.h"
#include "../cpp/driver/uv_sensor.h"

namespace hal = matrix_hal;

int main() {
  hal::MatrixIOBus bus;
  if (!bus.Init()) return false;

  hal::IMUSensor imu_sensor;
  imu_sensor.Setup(&bus);

  hal::PressureSensor pressure_sensor;
  pressure_sensor.Setup(&bus);

  hal::HumiditySensor humidity_sensor;
  humidity_sensor.Setup(&bus);

  hal::UVSensor uv_sensor;
  uv_sensor.Setup(&bus);

  hal::MCUFirmware mcu_firmware;
  mcu_firmware.Setup(&bus);

  hal::IMUData imu_data;
  hal::PressureData pressure_data;
  hal::HumidityData humidity_data;
  hal::UVData uv_data;
  hal::MCUData mcu_data;

  mcu_firmware.Read(&mcu_data);
  imu_sensor.Read(&imu_data);
  pressure_sensor.Read(&pressure_data);
  humidity_sensor.Read(&humidity_data);
  uv_sensor.Read(&uv_data);

  std::system("clear");

  std::cout << std::setprecision(2);
  std::cout << "IMUY:" << imu_data.yaw << "°\t";
  std::cout << "IMUR:" << imu_data.roll << "°\t";
  std::cout << "IMUP:" << imu_data.pitch << "°" << std::endl;
  std::cout << "HUMI:" << humidity_data.humidity << "%\t";
  std::cout << "HTMP:" << humidity_data.temperature << "°C\t";
  std::cout << "UVID:" << uv_data.uv << std::endl;
  std::cout << std::setprecision(5);
  std::cout << "PRSS:" << pressure_data.pressure << "\t";
  std::cout << "PrAL:" << pressure_data.altitude << "\t";
  std::cout << "PrTP:" << pressure_data.temperature << std::endl;
  std::cout << "MCU :0x" << std::hex << mcu_data.ID << "\t";
  std::cout << "VER :0x" << mcu_data.version << std::endl << std::endl;

  return 0;
}
