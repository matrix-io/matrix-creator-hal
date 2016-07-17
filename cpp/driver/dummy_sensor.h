/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#ifndef CPP_DRIVER_DUMMY_SENSOR_H_
#define CPP_DRIVER_DUMMY_SENSOR_H_

#include <string>
#include <random>
#include "./matrix_driver.h"
#include "./dummy_data.h"

namespace matrix_hal {

class DummySensor : public MatrixDriver {
 public:
  DummySensor();
  bool Read(DummyData* dummy);

 private:
  std::random_device random_device_;
  std::mt19937 random_engine_;
  std::uniform_real_distribution<> random_distribution_;
};
};      // namespace matrix_hal
#endif  // CPP_DRIVER_DUMMY_SENSOR_H_
