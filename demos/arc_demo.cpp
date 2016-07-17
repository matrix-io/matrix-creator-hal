/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <unistd.h>
#include <cmath>

#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/everloop.h"
#include "../cpp/driver/wishbone_bus.h"

namespace hal = matrix_hal;

int main() {
  hal::WishboneBus* bus = new hal::WishboneBus();

  bus->SpiInit();

  hal::Everloop everloop;
  hal::EverloopImage image1d;

  everloop.Setup(bus);

  unsigned counter = 0;

  while (1) {
    for (hal::LedValue& led : image1d.leds) {
      led.red = 0;
      led.green = 0;
      led.blue = static_cast<int>(std::sin(counter / 128.0) * 127.0) + 128;
      led.white = led.blue / 4;
    }

    everloop.Write(&image1d);
    ++counter;
    usleep(1000);
  }

  return 0;
}
