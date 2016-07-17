/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <string>
#include "cpp/driver/matrix_driver.h"

namespace matrix_hal {

void MatrixDriver::Setup(WishboneBus* wishbone) { wishbone_.reset(wishbone); }

};  // namespace matrix_hal
