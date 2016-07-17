/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#ifndef CPP_DRIVER_EVERLOOP_IMAGE_H_
#define CPP_DRIVER_EVERLOOP_IMAGE_H_

#include <cstdint>
#include <vector>

namespace matrix_hal {

class LedValue {
 public:
  LedValue() {
    red = 0;
    green = 0;
    blue = 0;
    white = 0;
  }

  uint32_t red;
  uint32_t green;
  uint32_t blue;
  uint32_t white;
};

const int kMatrixCreatorNLeds = 35;

class EverloopImage {
 public:
  EverloopImage(int nleds = kMatrixCreatorNLeds) { leds.resize(nleds); }
  std::vector<LedValue> leds;
};

};      // namespace matrix_hal
#endif  // CPP_DRIVER_EVERLOOP_IMAGE_H_
