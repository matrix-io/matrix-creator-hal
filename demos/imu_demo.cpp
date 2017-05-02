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
#include "../cpp/driver/iCompass.h"


namespace hal = matrix_hal;

//Magnetic declination angle for iCompass
#define MAG_DEC -6.683333  //degrees for Miami Beach, FL (from http://www.magnetic-declination.com/)


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

  iCompass maghead; 
  maghead = iCompass(MAG_DEC);

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
  myfile << "Count  \t YAW \t   PITCH   \t   ROLL   \t  YAW_2  \t PITCH_2 \t ROLL_2 \t accel_x \t accel_y \t accel_z \t gyro_x \t gyro_y \t gyro_z \t mag_x \t mag_y \t mag_z " << std::endl;
  // myfile << "Count , COMPASS_YAW , PITCH , ROLL , DCM_YAW , DCM_PITCH , DCM_ROLL" << std::endl;
  myfile.close();
  myfile.open ("imu.log", std::ofstream::app);

  int count = 0;

  float mag_max_x = -1000; 
  float mag_max_y = -1000; 
  float mag_max_z = -1000; 

  float mag_min_x = 1000; 
  float mag_min_y = 1000; 
  float mag_min_z = 1000; 

  bool first_loop = true;

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

    // if (first_loop) {
    //   mag_max_x = values[6];
    //   mag_max_y = values[7];
    //   mag_max_z = values[8];

    //   mag_min_x = values[6];
    //   mag_min_y = values[7];
    //   mag_min_z = values[8];
    // }
    // first_loop = false;

    //Compass Offset correction
    // float mag_offset_x = 0.302190000000000;
    // float mag_offset_y = 0.175560000000000;
    // float mag_offset_z = -0.0753200000000000;

    if (values[6] > mag_max_x) mag_max_x = values[6];
    if (values[7] > mag_max_y) mag_max_y = values[7];
    if (values[8] > mag_max_z) mag_max_z = values[8];

    if (values[6] < mag_min_x) mag_min_x = values[6];
    if (values[7] < mag_min_y) mag_min_y = values[7];
    if (values[8] < mag_min_z) mag_min_z = values[8];

    float mag_offset_x = (mag_max_x + mag_min_x)/2;
    float mag_offset_y = (mag_max_y + mag_min_y)/2;
    float mag_offset_z = (mag_max_z + mag_min_z)/2;

    values[3] = values[6] - mag_offset_x ;
    values[4] = values[7] - mag_offset_y ;
    values[5] = values[8] - mag_offset_z ;

    //Compass Tilt compensation
    // values[9] = maghead.iheading(1, 0, 0, values[0], values[1], values[2], values[6], values[7], values[8]);

    // DCM
    // dcm.G_Dt = 0.020; // ~10 ms  // TODO : measure the actual time
    // dcm.setSensorVals(values);
    // dcm.calDCM();
    // dcm.getEulerDeg(ypr);

    ypr[0] = atan2(-values[7], values[6]) * 180.0 / M_PI;
    ypr[1] = atan2(values[1], values[2]) * 180.0 / M_PI;
    ypr[2] = atan2(-values[0],
      sqrt(values[1] * values[1] + values[2] * values[2])) * 180.0 / M_PI;

    ypr_old[0] = atan2(-values[4], values[3]) * 180.0 / M_PI;
    ypr_old[1] = atan2(values[1], values[2]) * 180.0 / M_PI;
    ypr_old[2] = atan2(-values[0],
      sqrt(values[1] * values[1] + values[2] * values[2])) * 180.0 / M_PI;

    std::system("clear");

    // std::cout << "dcm_yaw  = " << ypr[0] << std::endl;
    // std::cout << "dcm_pitch  = " << ypr[1] << std::endl;
    // std::cout << "dcm_roll = " << ypr[2] << std::endl;
    
    std::cout << "Count  \t YAW \t   PITCH   \t   ROLL   \t  YAW_2  \t PITCH_2 \t ROLL_2 \t accel_x \t accel_y \t accel_z \t gyro_x \t gyro_y \t gyro_z \t mag_x \t mag_y \t mag_z " << std::endl;
    std::cout << count << " \t " 
    << ypr[0] << " \t " 
    << ypr[1] << " \t " 
    << ypr[2] << " \t " 
    << ypr_old[0] << " \t " 
    << ypr_old[1] << " \t " 
    << ypr_old[2] << " \t " 
    << values[0] << " \t " 
    << values[1] << " \t " 
    << values[2] << " \t " 
    << values[3] << " \t " 
    << values[4] << " \t " 
    << values[5] << " \t " 
    << values[6] << " \t " 
    << values[7] << " \t " 
    << values[8] << " \t " 
    << std::endl;

    myfile << count << " \t " 
    << ypr[0] << " \t " 
    << ypr[1] << " \t " 
    << ypr[2] << " \t "
    << ypr_old[0] << " \t " 
    << ypr_old[1] << " \t " 
    << ypr_old[2] << " \t " 
    << values[0] << " \t " 
    << values[1] << " \t " 
    << values[2] << " \t " 
    << values[3] << " \t " 
    << values[4] << " \t " 
    << values[5] << " \t " 
    << values[6] << " \t " 
    << values[7] << " \t " 
    << values[8] << " \t " 
    << mag_offset_x << " \t " 
    << mag_offset_y << " \t " 
    << mag_offset_z << " \t " 

    << std::endl;


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
