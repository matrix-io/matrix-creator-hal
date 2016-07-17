/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#ifndef CPP_DRIVER_MATRIX_DRIVER_H_
#define CPP_DRIVER_MATRIX_DRIVER_H_

#include <string>
#include <memory>
#include "./wishbone_bus.h"

namespace matrix_hal {

class MatrixDriver {
 public:
  void Setup(WishboneBus* wishbone);

 protected:
  std::unique_ptr<WishboneBus> wishbone_;
};
};      // namespace matrix_hal
#endif  // CPP_DRIVER_MATRIX_DRIVER_H_
