/*
 * Copyright 2016 <Admobilize>
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

#ifndef CPP_DRIVER_CIRCULAR_QUEUE_
#define CPP_DRIVER_CIRCULAR_QUEUE_

namespace matrix_hal {

template <class T>
class CircularQueue {
 public:
  CircularQueue(int N) : N_(N), pointer_(0) { fifo_ = new T[N_]; }
  ~CircularQueue() { delete fifo_; }

  T PushPop(const T& data) {
    fifo_[(pointer_ + N_ - 1) % N_] = data;
    T& ret = fifo_[pointer_];
    pointer_ = (pointer_ + 1) % N_;
    return ret;
  }

 private:
  T* fifo_;
  int N_;
  int pointer_;
};

};      // namespace matrix_hal
#endif  // CPP_DRIVER_CIRCULAR_QUEUE_
