/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#ifndef CPP_DRIVER_HUMIDITY_SENSOR_H_
#define CPP_DRIVER_HUMIDITY_SENSOR_H_

#include <string>
#include "./matrix_driver.h"
#include "./humidity_data.h"

namespace matrix_hal {

class HumiditySensor : public MatrixDriver {
 public:
  bool Read(HumidityData* data);
};
};      // namespace matrix_hal
#endif  // CPP_DRIVER_HUMIDITY_SENSOR_H_
