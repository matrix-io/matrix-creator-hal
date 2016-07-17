/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <string>
#include "cpp/driver/dummy_sensor.h"

namespace matrix_hal {

DummySensor::DummySensor()
    : random_engine_(random_device_()), random_distribution_(-1, 1) {}

bool DummySensor::Read(DummyData* dummy) {
  dummy->value = random_distribution_(random_engine_);
  return true;
}
};  // namespace matrix_hal
