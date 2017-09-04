/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <wiringPi.h>
#include <fstream>
#include <iostream>
#include <string>
#include <valarray>

#include "../cpp/driver/uart_control.h"
#include "../cpp/driver/wishbone_bus.h"

namespace hal = matrix_hal;

int main() {
  hal::WishboneBus bus;
  bus.SpiInit();

  hal::UartControl uart;
  uart.Setup(&bus);

  while (true) {
    uint16_t data;
    data = uart.GetUartValue();
    std::cout << data << std::endl;
    for (char i = 'A'; i < 'Z'; i++) {
      uart.SetUartValue((uint16_t)i);
    }
  }
}