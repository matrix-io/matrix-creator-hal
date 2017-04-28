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
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include "../cpp/driver/wishbone_bus.h"
#include "../cpp/driver/fw_data.h"
#include "../cpp/driver/mcu_firmware.h"
#include "../cpp/driver/imu_data.h"
#include "../cpp/driver/imu_sensor.h"
#include "../cpp/driver/pressure_data.h"
#include "../cpp/driver/pressure_sensor.h"
#include "../cpp/driver/humidity_data.h"
#include "../cpp/driver/humidity_sensor.h"
#include "../cpp/driver/uv_sensor.h"
#include "../cpp/driver/uv_data.h"
#include "../cpp/driver/DCM.h"


namespace hal = matrix_hal;

//Used for DCM filter
const float Kp_ROLLPITCH = 1.2f;  //was .3423
const float Ki_ROLLPITCH = 0.0234f;
const float Kp_YAW = 1.75f;   // was 1.2 and 0.02
const float Ki_YAW = 0.002f;

int main() {

  hal::WishboneBus bus;
  bus.SpiInit();

  hal::IMUData imu_data;
  hal::IMUSensor imu_sensor;

  imu_sensor.Setup(&bus);

  // DCM Initialization
  DCM dcm; 
  dcm = DCM();

  float values[11]; // TODO: Verify the length
  float ypr[3]; 
  float ypr_old[3]; 
  float last, now, elapsed;      // sample period expressed in milliseconds

  // DCM Init
  imu_sensor.Read(&imu_data);

  values[0] = imu_data.accel_x;
  values[1] = imu_data.accel_y;
  values[2] = imu_data.accel_y;
  values[3] = imu_data.gyro_x;
  values[4] = imu_data.gyro_y;
  values[5] = imu_data.gyro_z;
  values[6] = imu_data.mag_x;
  values[7] = imu_data.mag_y;
  values[8] = imu_data.mag_z;

  dcm.setSensorVals(values);
  dcm.DCM_init(Kp_ROLLPITCH, Ki_ROLLPITCH, Kp_YAW, Ki_YAW);

  std::ofstream myfile;
  myfile.open ("imu.log");
  myfile << "Count , COMPASS_YAW , PITCH , ROLL , DCM_YAW , DCM_PITCH , DCM_ROLL" << std::endl;
  myfile.close();
  myfile.open ("imu.log", std::ofstream::app);

  int count = 0;

  while (true) {

    imu_sensor.Read(&imu_data);

    values[0] = imu_data.accel_x;
    values[1] = imu_data.accel_y;
    values[2] = imu_data.accel_z;
    values[3] = imu_data.gyro_x;
    values[4] = imu_data.gyro_y;
    values[5] = imu_data.gyro_z;
    values[6] = imu_data.mag_x;
    values[7] = imu_data.mag_y;
    values[8] = imu_data.mag_z;


    dcm.G_Dt = 0.020; // ~10 ms  // TODO : measure the actual time
    dcm.setSensorVals(values);
    dcm.calDCM();
    dcm.getEulerDeg(ypr);

    ypr_old[0] = atan2(-values[7], values[6]) * 180.0 / M_PI;
    ypr_old[1] = atan2(values[1], values[2]) * 180.0 / M_PI;
    ypr_old[2] = atan2(-values[0],
      sqrt(values[1] * values[1] + values[2] * values[2])) * 180.0 / M_PI;

    std::system("clear");

    // std::cout << "dcm_yaw  = " << ypr[0] << std::endl;
    // std::cout << "dcm_pitch  = " << ypr[1] << std::endl;
    // std::cout << "dcm_roll = " << ypr[2] << std::endl;
    
    std::cout << "Count \t COMPASS_YAW \t PITCH \t ROLL \t DCM_YAW \t DCM_PITCH \t DCM_ROLL" << std::endl;
    std::cout << count << " \t " 
    << ypr_old[0] << " \t " 
    << ypr_old[1] << " \t " 
    << ypr_old[2] << " \t " 
    << ypr[0] << " \t " 
    << ypr[1] << " \t " 
    << ypr[2] << std::endl;

    myfile << count << " \t " 
    << ypr_old[0] << " \t " 
    << ypr_old[1] << " \t " 
    << ypr_old[2] << " \t " 
    << ypr[0] << " \t " 
    << ypr[1] << " \t " 
    << ypr[2] << std::endl;


    // std::cout << "raw_yaw = " << ypr_old[0] << std::endl;
    // std::cout << "raw_pitch = " << ypr_old[1] << std::endl;
    // std::cout << "raw_roll = " << ypr_old[2] << std::endl;

    if (++count > 1000)
      count = 0;

    usleep(20000);


  }

  myfile.close();

  return 0;
}
