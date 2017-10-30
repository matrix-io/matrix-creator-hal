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
const int kAngleThreshold = 15;

float mag_max_x = std::numeric_limits<float>::min();
float mag_max_y = std::numeric_limits<float>::min();
float mag_max_z = std::numeric_limits<float>::min();

float mag_min_x = std::numeric_limits<float>::max();
float mag_min_y = std::numeric_limits<float>::max();
float mag_min_z = std::numeric_limits<float>::max();

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

float amp_distance_xy = 0;
float offset_distance_xy = 0;
float amp_distance_xz = 0;
float offset_distance_xz = 0;

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

OrientationType CalcOrientation(float acc_x, float acc_y, float acc_z) {
  OrientationType orientation = NONE;
  // Getting distante of g vector thaht points always down
  float g = sqrt(acc_x * acc_x + acc_y * acc_y + acc_z * acc_z);
  // Getting angle from each axis to g vector
  float alpha_x = acos(acc_x / g) * 180 / M_PI;
  float alpha_y = acos(acc_y / g) * 180 / M_PI;
  float alpha_z = acos(acc_z / g) * 180 / M_PI;
  // Converting angle (0-180) to (0-90)
  float diff_alpha_x = 90 - abs(alpha_x - 90);
  float diff_alpha_y = 90 - abs(alpha_y - 90);
  float diff_alpha_z = 90 - abs(alpha_z - 90);

  if (diff_alpha_x < kAngleThreshold) {
    orientation = X_AXIS;
  } else if (diff_alpha_y < kAngleThreshold) {
    orientation = Y_AXIS;
  } else if (diff_alpha_z < kAngleThreshold) {
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

int main() {
  hal::WishboneBus bus;
  bus.SpiInit();

  hal::IMUSensor imu_sensor;
  imu_sensor.Setup(&bus);
  hal::IMUData imu_data;
  hal::IMUCalibrationData calib_data;

  hal::Everloop everloop;
  everloop.Setup(&bus);
  hal::EverloopImage image1d;

  OrientationType orientation = NONE;
  States state = XY_AXIS_CALIB;

  float xy_rot = 0;
  float xz_rot = 0;
  int xy_angle_index = 0;
  int xz_angle_index = 0;

  while (true) {
    imu_sensor.Read(&imu_data);

    // Reading values from PFGA
    acc_x = imu_data.accel_x;
    acc_y = imu_data.accel_y;
    acc_z = imu_data.accel_z;

    mag_x = imu_data.mag_x;
    mag_y = imu_data.mag_y;
    mag_z = imu_data.mag_z;

    orientation = CalcOrientation(acc_x, acc_y, acc_z);

    if (orientation == Z_AXIS) {
      mag_max_x = (mag_x > mag_max_x) ? mag_x : mag_max_x;
      mag_min_x = (mag_x < mag_min_x) ? mag_x : mag_min_x;
      mag_max_y = (mag_y > mag_max_y) ? mag_y : mag_max_y;
      mag_min_y = (mag_y < mag_min_y) ? mag_y : mag_min_y;

      calib_data.mag_offset_x = (mag_max_x + mag_min_x) / 2;
      calib_data.mag_offset_y = (mag_max_y + mag_min_y) / 2;

      // validating : Waiting to have enough values to start calculating angle.
      amp_distance_xy = Distance(mag_max_x, mag_max_y, mag_min_x, mag_min_y);
      offset_distance_xy =
          GetHypotenuse(calib_data.mag_offset_x, calib_data.mag_offset_y);
      xy_valid = offset_distance_xy / amp_distance_xy < 5;

      if (xy_valid) {
        // from rad to 0-360 deg
        xy_rot =
            atan2(mag_y - calib_data.mag_offset_y, mag_x - calib_data.mag_offset_x) *
                180 / M_PI +
            180;
        // saturation in 360 range
        xy_angle_index = fmin(xy_rot, 359);
        // saturation in 0 range
        xy_angle_index = fmax(xy_rot, 0);
        // making 24 slots of 360/24 = 15 deg
        xy_angle_index = xy_angle_index / 10;

        if (xy_angle_index >= 0) xy_count[xy_angle_index]++;
      }

    } else if (orientation == Y_AXIS) {
      mag_max_x = (mag_x > mag_max_x) ? mag_x : mag_max_x;
      mag_min_x = (mag_x < mag_min_x) ? mag_x : mag_min_x;
      mag_max_z = (mag_z > mag_max_z) ? mag_z : mag_max_z;
      mag_min_z = (mag_z < mag_min_z) ? mag_z : mag_min_z;

      calib_data.mag_offset_x = (mag_max_x + mag_min_x) / 2;
      calib_data.mag_offset_z = (mag_max_z + mag_min_z) / 2;

      // validating : Waiting to have enough values to start calculating angle.
      amp_distance_xz = Distance(mag_max_x, mag_max_z, mag_min_x, mag_min_z);
      offset_distance_xz =
          GetHypotenuse(calib_data.mag_offset_x, calib_data.mag_offset_z);
      xz_valid = offset_distance_xz / amp_distance_xz < 5;

      if (xz_valid) {
        // from rad to 0-360 deg
        xz_rot =
            atan2(mag_x - calib_data.mag_offset_x, mag_z - calib_data.mag_offset_z) *
                180 / M_PI +
            180;
        // saturation in 360 range
        xz_angle_index = fmin(xz_rot, 359);
        // saturation in 0 range
        xz_angle_index = fmax(xz_rot, 0);
        // making 24 slots of 360/24 = 15 deg
        xz_angle_index = xz_angle_index / 10;

        if (xz_angle_index >= 0) xz_count[xz_angle_index]++;
      }
    }

    // *********************  PRINTING *********************************

    system("clear");
    std::cerr << "**********************************************************\n";
    std::cerr << "******************  Compass Calibration ******************\n";
    std::cerr << "**********************************************************\n";
    std::cerr << "\n";

    if (state == XY_AXIS_CALIB) {
      std::cerr << "Step 1: Calibrate X and Y axis\n";
      std::cerr << "STATUS BAR: ";

      int calib_done_count = 0;
      // Printing Status Bar
      for (int i = 0; i < static_cast<int>(xy_count.size()); ++i) {
        if (xy_count[i] > 5) {
          std::cerr << "|";
          calib_done_count++;
        } else if (xy_count[i] > 3) {
          std::cerr << ":";
        } else if (xy_count[i] > 0) {
          std::cerr << ".";
        } else
          std::cerr << "_";
      }

      // calibration done detection
      if (calib_done_count == static_cast<int>(xy_count.size())) {
        state = Z_AXIS_CALIB;
      }

      std::cerr << "\n            ";
      for (int i = 0; i < static_cast<int>(xy_count.size()); ++i) {
        if (xy_angle_index == i) {
          std::cerr << "^";
        } else {
          std::cerr << " ";
        }
      }

      std::cerr << "\n\n";
      std::cerr << "Board Orientation: ";
      if (orientation == X_AXIS) {
        std::cerr << "X_AXIS\n";
      } else if (orientation == Y_AXIS) {
        std::cerr << "Y_AXIS\n";
      } else if (orientation == Z_AXIS) {
        std::cerr << "Z_AXIS\n";
      } else {
        std::cerr << "NONE\n";
      }

      std::cerr << "\n";
      std::cerr << "Notes:\n";
      std::cerr << "- Put the MATRIX Creator facing up (Z_AXIS Orientation)\n";
      std::cerr << "- Turn the board slowly to cover 360 deg. Use de status "
                   "bar as a guide.\n";
      std::cerr << "- The Calibration for this axis is completed when all "
                   "slots in the bar are \"|\"\n";

    } else if (state == Z_AXIS_CALIB) {
      std::cerr << "Step 1: Calibrate X and Y axis : Done !\n\n";
      std::cerr << "Step 2: Calibrate Z axis\n\n";

      std::cerr << "STATUS BAR: ";
      int calib_done_count = 0;
      for (int i = 0; i < static_cast<int>(xz_count.size()); i++) {
        if (xz_count[i] > 5) {
          std::cerr << "|";
          calib_done_count++;
        } else if (xz_count[i] > 3) {
          std::cerr << ":";
        } else if (xz_count[i] > 0) {
          std::cerr << ".";
        } else
          std::cerr << "_";
      }

      // calibration done detection
      if (calib_done_count == static_cast<int>(xy_count.size())) {
        state = DONE_CALIB;
      }

      std::cerr << "\n            ";
      for (int i = 0; i < static_cast<int>(xz_count.size()); i++) {
        if (xz_angle_index == i) {
          std::cerr << "^";
        } else {
          std::cerr << " ";
        }
      }

      std::cerr << "\nBoard Orientation: ";
      if (orientation == X_AXIS) {
        std::cerr << "X_AXIS\n";
      } else if (orientation == Y_AXIS) {
        std::cerr << "Y_AXIS\n";
      } else if (orientation == Z_AXIS) {
        std::cerr << "Z_AXIS\n";
      } else {
        std::cerr << "NONE\n";
      }

      std::cerr << "\nNotes:\n";
      std::cerr << "-  Creator in Y_AXIS Orientation\n";
      std::cerr << "- Turn the board slowly to cover 360 deg. Use de status "
                   "bar as a guide.\n";
      std::cerr << "- The Calibration for this axis is completed when all "
                   "slots in the bar are \"|\"\n";

    } else if (state == DONE_CALIB) {
      std::cerr << "CALIBRATION DONE !!!\n\n";

      std::cerr << "Compass Axis X:\n";
      std::cerr << "- Max: " << mag_max_x << "\n";
      std::cerr << "- Min: " << mag_min_x << "\n";
      std::cout << "Offset X: " << calib_data.mag_offset_x << "\n";

      std::cerr << "Compass Axis Y:\n";
      std::cerr << "- Max: " << mag_max_y << "\n";
      std::cerr << "- Min: " << mag_min_y << "\n";
      std::cout << "Offset Y: " << calib_data.mag_offset_y << "\n";

      std::cerr << "Compass Axis Z:\n";
      std::cerr << "- Max: " << mag_max_z << "\n";
      std::cerr << "- Min: " << mag_min_z << "\n";
      std::cout << "Offset Z: " << calib_data.mag_offset_z << "\n";

      std::cerr << "\nDo you want to save this calibration values to the "
                   "MATRIX CREATOR? [y/n]: \n";
      std::cerr << " (Previous calibration data will be lost)\n";

      char choice;
      std::cin >> choice;
      while (tolower(choice) != 'n' && tolower(choice) != 'y') {
        std::cerr << "\n";
        std::cerr << "Please enter Y (Yes) or N (No): ";
        std::cin >> choice;
      }

      if (tolower(choice) == 'y') {
        if (imu_sensor.SetCompassCalibration(&calib_data))
          std::cerr << "Sucessfully saved...\n";
        break;
      } else if (tolower(choice) == 'n') {
        std::cerr << "Calibration not saved.\n";
        break;
      }
      std::cerr << "\n";
    }

    // Just a delay to control the speed of the printed interface
    usleep(100000);
  }

  return 0;
}
