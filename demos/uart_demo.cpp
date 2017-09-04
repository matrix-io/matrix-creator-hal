/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <wiringPi.h>
#include <string>
#include <fstream>
#include <iostream>
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
  }
}