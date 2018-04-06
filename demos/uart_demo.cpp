/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <wiringPi.h>
#include <fstream>
#include <iostream>
#include <string>
#include <valarray>

#include "../cpp/driver/matrixio_bus.h"
#include "../cpp/driver/uart_control.h"

namespace hal = matrix_hal;

int main() {
  hal::MatrixIOBus bus;
  bus.Init();

  hal::UartControl uart;
  uart.Setup(&bus);

  uart.SetUartValue(0x0D);
  uart.SetUartValue(0x0D);
  uart.SetUartValue(0x0D);

  while (true) {
    uint16_t data;
    data = uart.GetUartValue();
    std::cout << char(data);
  }
}
