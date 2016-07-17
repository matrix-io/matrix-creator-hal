/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#ifndef CPP_DRIVER_GPIO_CONTROL_H_
#define CPP_DRIVER_GPIO_CONTROL_H_
#include <cstdint>
#include <vector>
#include "./matrix_driver.h"

namespace matrix_hal {

class GPIO : public MatrixDriver {
 public:
  uint16_t mem_offset_;
  uint16_t control_;
  uint16_t prescaler_;
  uint16_t counter_;

  bool EnablePWM(int prescaler);
  bool SetPeriod(int period);
  bool SetPrescaler(int prescaler);
};

class GPIOControl : public MatrixDriver {
 public:
  void Setup(WishboneBus* wishbone);

  std::vector<GPIO> gpios_;
};

};      // namespace matrix_hal
#endif  // CPP_DRIVER_GPIO_CONTROL_H_
