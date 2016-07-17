/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#ifndef CPP_DRIVER_EVERLOOP_H_
#define CPP_DRIVER_EVERLOOP_H_

#include <string>
#include "./matrix_driver.h"
#include "./everloop_image.h"

namespace matrix_hal {

class Everloop : public MatrixDriver {
 public:
  Everloop();
  bool Write(const EverloopImage* led_image);
};
};      // namespace matrix_hal
#endif  // CPP_DRIVER_EVERLOOP_H_
