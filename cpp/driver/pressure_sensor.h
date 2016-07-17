/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#ifndef CPP_DRIVER_PRESSURE_SENSOR_H_
#define CPP_DRIVER_PRESSURE_SENSOR_H_

#include <string>
#include "./matrix_driver.h"
#include "./pressure_data.h"

namespace matrix_hal {

class PressureSensor : public MatrixDriver {
 public:
  bool Read(PressureData* data);
};
};      // namespace matrix_hal
#endif  // CPP_DRIVER_PRESSURE_SENSOR_H_
