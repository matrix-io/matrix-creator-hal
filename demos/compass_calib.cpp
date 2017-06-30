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
#include <iostream>
#include <math.h>
#include <iomanip>
#include <limits>

#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/everloop.h"
#include "../cpp/driver/imu_data.h"
#include "../cpp/driver/imu_sensor.h"
#include "../cpp/driver/humidity_data.h"
#include "../cpp/driver/humidity_sensor.h"
#include "../cpp/driver/pressure_data.h"
#include "../cpp/driver/pressure_sensor.h"
#include "../cpp/driver/wishbone_bus.h"

#define Z_AXIS_SAMPLES  1000
#define ANGLE_THRESHOLD  15 // Max angle permited to continue in flat state. In Degrees. (TODO: improve comment)
#define CALIB_ARRAY_SIZE 360

namespace hal = matrix_hal;

bool calibration_finished = false;

float mag_x_max = std::numeric_limits<float>::min();
float mag_y_max = std::numeric_limits<float>::min();
float mag_z_max = std::numeric_limits<float>::min();

float mag_x_min = std::numeric_limits<float>::max();
float mag_y_min = std::numeric_limits<float>::max();
float mag_z_min = std::numeric_limits<float>::max();

float x_mag_calib[CALIB_ARRAY_SIZE];
float y_mag_calib[CALIB_ARRAY_SIZE]; 
float Z_mag_calib[CALIB_ARRAY_SIZE]; 

float x_mag_offset;
float y_mag_offset;
float z_mag_offset;

float acc_x = 0;
float acc_y = 0;
float acc_z = 0;

float mag_x = 0;
float mag_y = 0;
float mag_z = 0;

float yaw   = 0;
float pitch = 0;
float roll  = 0;

float gravity = 0;
int count = 0;

enum OrientationType{
  NONE,
  X_AXIS,
  Y_AXIS,
  Z_AXIS,
};

std::vector<int> xy_count (36);
std::vector<float> xy_value (36);
std::vector<int> xz_count (36);
std::vector<float> xz_value (36);

bool xy_valid = false;
bool xz_valid = false;

float xy_amp_distance;
float xy_offset_distance;

float xz_amp_distance;
float xz_offset_distance;

bool xy_calib_done = false;
bool xz_calib_done = false;

OrientationType calc_orientation(float acc_x, float acc_y,float acc_z)
{
  OrientationType orientation = NONE;
  float g = sqrt(acc_x * acc_x + acc_y * acc_y + acc_z * acc_z);

  float alpha_x = acos(acc_x/g)* 180 / M_PI;
  float alpha_y = acos(acc_y/g)* 180 / M_PI;
  float alpha_z = acos(acc_z/g)* 180 / M_PI;
  
  if (90 - abs(alpha_x - 90) < ANGLE_THRESHOLD){
      orientation = X_AXIS;
  }else if (90 - abs(alpha_y - 90) < ANGLE_THRESHOLD){
      orientation = Y_AXIS;
  }else if(90 - abs(alpha_z - 90) < ANGLE_THRESHOLD){
      orientation = Z_AXIS;
  }else{
      orientation = NONE;
  }

  return orientation;
}

int main() {

  hal::WishboneBus bus;
  bus.SpiInit();

  hal::IMUSensor imu_sensor;
  imu_sensor.Setup(&bus);
  hal::IMUData imu_data;

  hal::Everloop everloop;
  everloop.Setup(&bus);
  hal::EverloopImage image1d;
  
  OrientationType orientation = NONE;

  float xy_rot = 0;
  float xz_rot = 0;
  uint xy_angle_index = 0;
  uint xz_angle_index = 0;

  while (true){

    imu_sensor.Read(&imu_data);

    // Reading values from PFGA
    acc_x = imu_data.accel_x;    
    acc_y = imu_data.accel_y;    
    acc_z = imu_data.accel_z;

    mag_x = imu_data.mag_x;
    mag_y = imu_data.mag_y;
    mag_z = imu_data.mag_z;

    orientation = calc_orientation(acc_x, acc_y, acc_z);

    if (orientation == Z_AXIS)
    {
      mag_x_max = (mag_x > mag_x_max)? mag_x : mag_x_max; 
      mag_x_min = (mag_x < mag_x_min)? mag_x : mag_x_min; 
      mag_y_max = (mag_y > mag_y_max)? mag_y : mag_y_max; 
      mag_y_min = (mag_y < mag_y_min)? mag_y : mag_y_min; 

      x_mag_offset = (mag_x_max + mag_x_min ) / 2;
      y_mag_offset = (mag_y_max + mag_y_min ) / 2;

      // validating : Waiting to have enough values to start calculating angle.
      xy_amp_distance = sqrt(pow(mag_x_max - mag_x_min,2) + pow(mag_y_max - mag_y_min,2))/2;
      xy_offset_distance = sqrt(pow(x_mag_offset,2) + pow(y_mag_offset,2));
      xy_valid = xy_offset_distance/xy_amp_distance < 5; 

      if (xy_valid)
      {
        xy_rot = atan2(mag_y - y_mag_offset, mag_x - x_mag_offset ) * 180 / M_PI + 180; // from rad to 0-360 deg

        xy_angle_index = fmin(xy_rot,359); // saturation in 360 range
        xy_angle_index = fmax(xy_rot,0); // saturation in 0 range

        xy_angle_index = xy_angle_index / 10; // making 24 slots of 360/24 = 15 deg
        
        xy_count[xy_angle_index]++;
      } 

      

    }else if ( orientation == Y_AXIS )
    {
      mag_x_max = (mag_x > mag_x_max)? mag_x : mag_x_max; 
      mag_x_min = (mag_x < mag_x_min)? mag_x : mag_x_min; 
      mag_z_max = (mag_z > mag_z_max)? mag_z : mag_z_max; 
      mag_z_min = (mag_z < mag_z_min)? mag_z : mag_z_min;   

      x_mag_offset = (mag_x_max + mag_x_min ) / 2;
      z_mag_offset = (mag_z_max + mag_z_min ) / 2;

      // validating : Waiting to have enough values to start calculating angle.
      xz_amp_distance = sqrt(pow(mag_x_max - mag_x_min,2) + pow(mag_z_max - mag_z_min,2))/2;
      xz_offset_distance = sqrt(pow(x_mag_offset,2) + pow(z_mag_offset,2));
      xz_valid = xz_offset_distance/xz_amp_distance < 5; 

      if (xz_valid){

        xz_rot = atan2(mag_x - x_mag_offset, mag_z - z_mag_offset) * 180 / M_PI + 180; // from rad to 0-360 deg

        xz_angle_index = fmin(xz_rot,359); // saturation in 360 range
        xz_angle_index = fmax(xz_rot,0); // saturation in 0 range
        
        xz_angle_index = xz_angle_index / 10; // making 24 slots of 360/24 = 15 deg

        xz_count[xz_angle_index]++;  
      }
    }

    // *********************  PRINTING *********************************

    system("clear");

    std::cout << "***********************************************************" << "\n";
    std::cout << "******************  Compass Calibration *******************" << "\n";
    std::cout << "***********************************************************" << "\n\n";

    std::cout << "Compass Axis X:" << "\n";
    std::cout << "- Max: "  << mag_x_max << "\n"; 
    std::cout << "- Min: "  << mag_x_min << "\n"; 
    std::cout << "- Offset : "  << x_mag_offset << "\n"; 

    std::cout << "Compass Axis Y:" << "\n";
    std::cout << "- Max: "  << mag_y_max << "\n"; 
    std::cout << "- Min: "  << mag_y_min << "\n"; 
    std::cout << "- Offset : "  << y_mag_offset << "\n"; 
    
    std::cout << "Compass Axis Z:" << "\n"; 
    std::cout << "- Max: "  << mag_z_max << "\n"; 
    std::cout << "- Min: "  << mag_z_min << "\n"; 
    std::cout << "- Offset : "  << z_mag_offset << "\n"; 

    std::cout << "\n";

    // YX Bar

    std::cout << "X and Y Axis calibration:" << "\n" ;
    for (uint i = 0; i < xy_count.size(); i++) {
      if (xy_count[i] > 5)
        std::cout << "|" ;
      else if (xy_count[i] > 3)
        std::cout << ":" ;
      else if (xy_count[i] > 0)
        std::cout << "." ;
      else 
      std::cout << "_" ;
    }

    // TODO:Add detection of calibration done 

    std::cout << "\n" ;
    for (uint i = 0; i < xy_count.size(); i++) {
      if (xy_angle_index == i) {
        std::cout << "^" ;
      }
      else{
        std::cout << " " ;
      }
    } 
    std::cout << std::endl;

    // Z Calibration
    std::cout << "Z Axis Calibration:"  << "\n" ;
    for (uint i = 0; i < xz_count.size(); i++) {
      if (xz_count[i] > 5)
        std::cout << "|" ;
      else if (xz_count[i] > 3)
        std::cout << ":" ;
      else if (xz_count[i] > 0)
        std::cout << "." ;
      else 
      std::cout << "_" ;
    }
    // TODO: Add detection of calibration done 

    std::cout << "\n" ;
    for (uint i = 0; i < xz_count.size(); i++) {
      if (xz_angle_index == i) {
        std::cout << "^" ;
      }
      else{
        std::cout << " " ;
      }
    } 

    std::cout << "\n";

    std::cout << "Board Orientation: " ; 
    if(orientation == X_AXIS){
      std::cout << "X_AXIS" << "\n";
    }else if(orientation == Y_AXIS){
      std::cout << "Y_AXIS" << "\n";
    }else if(orientation == Z_AXIS){
      std::cout << "Z_AXIS" << "\n";
    }else{
      std::cout << "NONE" << "\n";
    }



    std::cout << std::endl;



    usleep(100000);
  }

  return 0;
}
