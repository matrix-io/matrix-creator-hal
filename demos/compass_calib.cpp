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

#include <math.h>
#include <unistd.h>
#include <iomanip>
#include <iostream>
#include <limits>

#include "../cpp/driver/everloop.h"
#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/humidity_data.h"
#include "../cpp/driver/humidity_sensor.h"
#include "../cpp/driver/imu_data.h"
#include "../cpp/driver/imu_sensor.h"
#include "../cpp/driver/pressure_data.h"
#include "../cpp/driver/pressure_sensor.h"
#include "../cpp/driver/wishbone_bus.h"

namespace hal = matrix_hal;

// Max angle permited to continue in flat state. In Degrees.
#define ANGLE_THRESHOLD 15

float mag_max_x = std::numeric_limits<float>::min();
float mag_max_y = std::numeric_limits<float>::min();
float mag_max_z = std::numeric_limits<float>::min();

float mag_min_x = std::numeric_limits<float>::max();
float mag_min_y = std::numeric_limits<float>::max();
float mag_min_z = std::numeric_limits<float>::max();

float mag_off_x;
float mag_off_y;
float mag_off_z;

float acc_x = 0;
float acc_y = 0;
float acc_z = 0;

float mag_x = 0;
float mag_y = 0;
float mag_z = 0;

std::vector<int> xy_count(36);
std::vector<int> xz_count(36);

bool xy_valid = false;
bool xz_valid = false;

float amp_distance_xy;
float offset_distance_xy;
float amp_distance_xz;
float offset_distance_xz;

enum OrientationType {
  NONE,
  X_AXIS,
  Y_AXIS,
  Z_AXIS,
};

enum States {
  XY_AXIS_CALIB,
  Z_AXIS_CALIB,
  DONE_CALIB,
};

// Functions

OrientationType CalcOrientation(float acc_x, float acc_y, float acc_z) {
  OrientationType orientation = NONE;
  float g = sqrt(acc_x * acc_x + acc_y * acc_y + acc_z * acc_z);

  float alpha_x = acos(acc_x / g) * 180 / M_PI;
  float alpha_y = acos(acc_y / g) * 180 / M_PI;
  float alpha_z = acos(acc_z / g) * 180 / M_PI;

  if (90 - abs(alpha_x - 90) < ANGLE_THRESHOLD) {
    orientation = X_AXIS;
  } else if (90 - abs(alpha_y - 90) < ANGLE_THRESHOLD) {
    orientation = Y_AXIS;
  } else if (90 - abs(alpha_z - 90) < ANGLE_THRESHOLD) {
    orientation = Z_AXIS;
  } else {
    orientation = NONE;
  }

  return orientation;
}

float Distance(float ax, float ay, float bx, float by) {
  return sqrt(pow(bx - ax, 2) + pow(by - ay, 2));
}

float GetHypotenuse(float a, float b) { return sqrt(pow(a, 2) + pow(b, 2)); }

// MAIN

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
  States state = XY_AXIS_CALIB;

  float xy_rot = 0;
  float xz_rot = 0;
  uint xy_angle_index = 0;
  uint xz_angle_index = 0;

  // TEST
  imu_sensor.Read(&imu_data);

  printf("------------------- BEFORE --------------------------\n");
  // printf("yaw:\t\t%x\n", *(unsigned int*)&imu_data.yaw);
  // printf("pitch:\t\t%x\n", *(unsigned int*)&imu_data.pitch);
  printf("yaw:\t\t%f\n", imu_data.yaw);
  printf("pitch:\t\t%f\n", imu_data.pitch);
  printf("roll:\t\t%f\n", imu_data.roll);
  printf("accel_x:\t\t%f\n", imu_data.accel_x);
  printf("accel_y:\t\t%f\n", imu_data.accel_y);
  printf("accel_z:\t\t%f\n", imu_data.accel_z);
  printf("gyro_x:\t\t%f\n", imu_data.gyro_x);
  printf("gyro_y:\t\t%f\n", imu_data.gyro_y);
  printf("gyro_z:\t\t%f\n", imu_data.gyro_z);
  printf("mag_x:\t\t%f\n", imu_data.mag_x);
  printf("mag_y:\t\t%f\n", imu_data.mag_y);
  printf("mag_z:\t\t%f\n", imu_data.mag_z);
  printf("mag_offset_x:\t\t%f\n", imu_data.mag_offset_x);
  printf("mag_offset_y:\t\t%f\n", imu_data.mag_offset_y);
  printf("mag_offset_z:\t\t%f\n", imu_data.mag_offset_z);
  
  imu_sensor.SetCompassCalibration(44.4400,55.5500,66.6600,&imu_data);

  printf("------------------- AFTER --------------------------\n");
  printf("yaw:\t\t%f\n", imu_data.yaw);
  printf("pitch:\t\t%f\n", imu_data.pitch);
  printf("roll:\t\t%f\n", imu_data.roll);
  printf("accel_x:\t\t%f\n", imu_data.accel_x);
  printf("accel_y:\t\t%f\n", imu_data.accel_y);
  printf("accel_z:\t\t%f\n", imu_data.accel_z);
  printf("gyro_x:\t\t%f\n", imu_data.gyro_x);
  printf("gyro_y:\t\t%f\n", imu_data.gyro_y);
  printf("gyro_z:\t\t%f\n", imu_data.gyro_z);
  printf("mag_x:\t\t%f\n", imu_data.mag_x);
  printf("mag_y:\t\t%f\n", imu_data.mag_y);
  printf("mag_z:\t\t%f\n", imu_data.mag_z);
  printf("mag_offset_x:\t\t%f\n", imu_data.mag_offset_x);
  printf("mag_offset_y:\t\t%f\n", imu_data.mag_offset_y);
  printf("mag_offset_z:\t\t%f\n", imu_data.mag_offset_z);
  
  acc_x = imu_data.accel_x;
  acc_y = imu_data.accel_y;
  acc_z = imu_data.accel_z;

  mag_x = imu_data.mag_x;
  mag_y = imu_data.mag_y;
  mag_z = imu_data.mag_z;



  // while (true) {
  //   imu_sensor.Read(&imu_data);

  //   // Reading values from PFGA
  //   acc_x = imu_data.accel_x;
  //   acc_y = imu_data.accel_y;
  //   acc_z = imu_data.accel_z;

  //   mag_x = imu_data.mag_x;
  //   mag_y = imu_data.mag_y;
  //   mag_z = imu_data.mag_z;

  //   orientation = CalcOrientation(acc_x, acc_y, acc_z);

  //   if (orientation == Z_AXIS) {
  //     mag_max_x = (mag_x > mag_max_x) ? mag_x : mag_max_x;
  //     mag_min_x = (mag_x < mag_min_x) ? mag_x : mag_min_x;
  //     mag_max_y = (mag_y > mag_max_y) ? mag_y : mag_max_y;
  //     mag_min_y = (mag_y < mag_min_y) ? mag_y : mag_min_y;

  //     mag_off_x = (mag_max_x + mag_min_x) / 2;
  //     mag_off_y = (mag_max_y + mag_min_y) / 2;

  //     // validating : Waiting to have enough values to start calculating angle.
  //     amp_distance_xy = Distance(mag_max_x, mag_max_y, mag_min_x, mag_min_y);
  //     offset_distance_xy = GetHypotenuse(mag_off_x, mag_off_y);
  //     xy_valid = offset_distance_xy / amp_distance_xy < 5;

  //     if (xy_valid) {
  //       // from rad to 0-360 deg
  //       xy_rot = atan2(mag_y - mag_off_y, mag_x - mag_off_x) * 180 / M_PI + 180;
  //       // saturation in 360 range
  //       xy_angle_index = fmin(xy_rot, 359);
  //       // saturation in 0 range
  //       xy_angle_index = fmax(xy_rot, 0);
  //       // making 24 slots of 360/24 = 15 deg
  //       xy_angle_index = xy_angle_index / 10;

  //       xy_count[xy_angle_index]++;
  //     }

  //   } else if (orientation == Y_AXIS) {
  //     mag_max_x = (mag_x > mag_max_x) ? mag_x : mag_max_x;
  //     mag_min_x = (mag_x < mag_min_x) ? mag_x : mag_min_x;
  //     mag_max_z = (mag_z > mag_max_z) ? mag_z : mag_max_z;
  //     mag_min_z = (mag_z < mag_min_z) ? mag_z : mag_min_z;

  //     mag_off_x = (mag_max_x + mag_min_x) / 2;
  //     mag_off_z = (mag_max_z + mag_min_z) / 2;

  //     // validating : Waiting to have enough values to start calculating angle.
  //     amp_distance_xz = Distance(mag_max_x, mag_max_z, mag_min_x, mag_min_z);
  //     offset_distance_xz = GetHypotenuse(mag_off_x, mag_off_z);
  //     xz_valid = offset_distance_xz / amp_distance_xz < 5;

  //     if (xz_valid) {
  //       // from rad to 0-360 deg
  //       xz_rot = atan2(mag_x - mag_off_x, mag_z - mag_off_z) * 180 / M_PI + 180;
  //       // saturation in 360 range
  //       xz_angle_index = fmin(xz_rot, 359);
  //       // saturation in 0 range
  //       xz_angle_index = fmax(xz_rot, 0);
  //       // making 24 slots of 360/24 = 15 deg
  //       xz_angle_index = xz_angle_index / 10;

  //       xz_count[xz_angle_index]++;
  //     }
  //   }

  //   // *********************  PRINTING *********************************

  //   system("clear");
  //   std::cout << "**********************************************************\n";
  //   std::cout << "******************  Compass Calibration ******************\n";
  //   std::cout << "**********************************************************\n";
  //   std::cout << "\n";

  //   if (state == XY_AXIS_CALIB) {
  //     std::cout << "Step 1: Calibrate X and Y axis\n";
  //     std::cout << "STATUS BAR: ";

  //     int calib_done_count = 0;
  //     // Printing Status Bar
  //     for (uint i = 0; i < xy_count.size(); i++) {
  //       if (xy_count[i] > 5) {
  //         std::cout << "|";
  //         calib_done_count++;
  //       } else if (xy_count[i] > 3) {
  //         std::cout << ":";
  //       } else if (xy_count[i] > 0) {
  //         std::cout << ".";
  //       } else
  //         std::cout << "_";
  //     }

  //     // calibration done detection
  //     if (calib_done_count == (int)xy_count.size()) {
  //       state = Z_AXIS_CALIB;
  //     }

  //     std::cout << "\n            ";
  //     for (uint i = 0; i < xy_count.size(); i++) {
  //       if (xy_angle_index == i) {
  //         std::cout << "^";
  //       } else {
  //         std::cout << " ";
  //       }
  //     }

  //     std::cout << "\n\n";
  //     std::cout << "Board Orientation: ";
  //     if (orientation == X_AXIS) {
  //       std::cout << "X_AXIS\n";
  //     } else if (orientation == Y_AXIS) {
  //       std::cout << "Y_AXIS\n";
  //     } else if (orientation == Z_AXIS) {
  //       std::cout << "Z_AXIS\n";
  //     } else {
  //       std::cout << "NONE\n";
  //     }

  //     std::cout << "\n";
  //     std::cout << "Notes:\n";
  //     std::cout << "- Put the MATRIX Creator facing up (Z_AXIS Orientation)\n";
  //     std::cout << "- Turn the board slowly to cover 360 deg. Use de status "
  //                  "bar as a guide.\n";
  //     std::cout << "- The Calibration for this axis is completed when all "
  //                  "slots in the bar are \"|\"\n";

  //   } else if (state == Z_AXIS_CALIB) {
  //     std::cout << "Step 1: Calibrate X and Y axis : Done !\n\n";
  //     std::cout << "Step 2: Calibrate Z axis\n\n";

  //     std::cout << "STATUS BAR: ";
  //     int calib_done_count = 0;
  //     for (uint i = 0; i < xz_count.size(); i++) {
  //       if (xz_count[i] > 5) {
  //         std::cout << "|";
  //         calib_done_count++;
  //       } else if (xz_count[i] > 3) {
  //         std::cout << ":";
  //       } else if (xz_count[i] > 0) {
  //         std::cout << ".";
  //       } else
  //         std::cout << "_";
  //     }

  //     // calibration done detection
  //     if (calib_done_count == (int)xy_count.size()) {
  //       state = DONE_CALIB;
  //     }

  //     std::cout << "\n            ";
  //     for (uint i = 0; i < xz_count.size(); i++) {
  //       if (xz_angle_index == i) {
  //         std::cout << "^";
  //       } else {
  //         std::cout << " ";
  //       }
  //     }

  //     std::cout << "\nBoard Orientation: ";
  //     if (orientation == X_AXIS) {
  //       std::cout << "X_AXIS\n";
  //     } else if (orientation == Y_AXIS) {
  //       std::cout << "Y_AXIS\n";
  //     } else if (orientation == Z_AXIS) {
  //       std::cout << "Z_AXIS\n";
  //     } else {
  //       std::cout << "NONE\n";
  //     }

  //     std::cout << "\nNotes:\n";
  //     std::cout << "-  Creator in Y_AXIS Orientation\n";
  //     std::cout << "- Turn the board slowly to cover 360 deg. Use de status "
  //                  "bar as a guide.\n";
  //     std::cout << "- The Calibration for this axis is completed when all "
  //                  "slots in the bar are \"|\"\n";

  //   } else if (state == DONE_CALIB) {
  //     std::cout << "CALIBRATION DONE !!!\n\n";

  //     std::cout << "Compass Axis X:\n";
  //     std::cout << "- Max: " << mag_max_x << "\n";
  //     std::cout << "- Min: " << mag_min_x << "\n";
  //     std::cout << "- Offset : " << mag_off_x << "\n";

  //     std::cout << "Compass Axis Y:\n";
  //     std::cout << "- Max: " << mag_max_y << "\n";
  //     std::cout << "- Min: " << mag_min_y << "\n";
  //     std::cout << "- Offset : " << mag_off_y << "\n";

  //     std::cout << "Compass Axis Z:\n";
  //     std::cout << "- Max: " << mag_max_z << "\n";
  //     std::cout << "- Min: " << mag_min_z << "\n";
  //     std::cout << "- Offset : " << mag_off_z << "\n";

  //     std::cout << "\nDo you want to save this calibration values to the "
  //                  "MATRIX CREATOR? [y/n]: \n";
  //     std::cout << " (Previous calibration data will be lost)\n";

  //     char choice;
  //     std::cin >> choice;
  //     while (tolower(choice) != 'n' && tolower(choice) != 'y') {
  //       std::cout << "\n";
  //       std::cout << "Please enter Y (Yes) or N (No): ";
  //       std::cin >> choice;
  //     }

  //     if (tolower(choice) == 'y') {
  //       // if (imu_sensor.SetCompassCalibration(mag_off_x, mag_off_y, mag_off_z))
  //         std::cout << "Sucessfully saved...\n";
  //       break;
  //     } else if (tolower(choice) == 'n') {
  //       std::cout << "Calibration not saved.\n";
  //       break;
  //     }
  //     std::cout << "\n";
  //   }

  //   usleep(50000);
  // }

  return 0;
}
