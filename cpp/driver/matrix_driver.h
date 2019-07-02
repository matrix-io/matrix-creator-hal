/*
 * Copyright 2016-2018 <Admobilize>
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

#ifndef CPP_DRIVER_MATRIX_DRIVER_H_
#define CPP_DRIVER_MATRIX_DRIVER_H_

#include <memory>
#include <string>
#include "./matrixio_bus.h"

namespace matrix_hal {

class MatrixDriver {
 public:
  void Setup(MatrixIOBus *bus);

  int MatrixLeds() { return bus_->MatrixLeds(); }

 protected:
  MatrixIOBus *bus_;
};
};      // namespace matrix_hal
#endif  // CPP_DRIVER_MATRIX_DRIVER_H_
